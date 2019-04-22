#include "Car.h"

#include "RaceTrack.h"
#include "Window.h"
#include <array>
#include <algorithm>
#include "Simulation.h"

// Constants for the file
constexpr float DEGTORAD = 3.14159265359f / 180.f;
constexpr unsigned int IDLETIME = 3; // In seconds

namespace Machine
{
	// Creates a car from a series of weights and biases
	Car::Car(float width, float height, std::vector<unsigned int> sizes, std::vector<std::vector<std::vector<double>>> networkWeights, std::vector<std::vector<double>> networkBiases)
		: m_network{ sizes, networkWeights, networkBiases }
	{
		// Initialises the body
		m_body.setSize(sf::Vector2f{ width, height });
		m_body.setOrigin(m_body.getSize() / 2.f);
		m_body.setOutlineColor(sf::Color::White);
		m_body.setOutlineThickness(1.f);
		m_body.setFillColor(sf::Color::Transparent);

		// Creates the rays
		m_rays.reserve(sizes[0]);
		for (unsigned int i = 0; i < sizes[0]; i++)
			m_rays.push_back(sf::Vertex{ sf::Vector2f{0.f, 0.f}, sf::Color::White });
	}

	// Creates a car with random weights and biases
	Car::Car(float width, float height, std::vector<unsigned int> sizes)
		: m_network{sizes}
	{
		// Initialises the body
		m_body.setSize(sf::Vector2f{width, height});
		m_body.setOrigin(m_body.getSize() / 2.f);
		m_body.setOutlineColor(sf::Color::White);
		m_body.setOutlineThickness(1.f);
		m_body.setFillColor(sf::Color::Transparent);

		// Creates the rays
		m_rays.reserve(sizes[0]);
		for (unsigned int i = 0; i < sizes[0]; i++)
			m_rays.push_back(sf::Vertex{ sf::Vector2f{0.f, 0.f}, sf::Color::White });
	}

	// Creates a car from a copy of another car
	Car::Car(const Car& car)
		: m_network{car.m_network}
	{
		m_body = car.m_body;
		m_rays = car.m_rays;
	}

	// Writes the car to an output file stream
	std::ostream& operator<<(std::ostream& stream, const Car& car)
	{
		stream << 'c' << std::endl << car.m_network;

		// Returns the stream to allow chaining
		return stream;
	}

	// Splices two cars together
	// Values from each car will be swapped randomly
	void Car::SpliceCars(Car& c1, Car& c2)
	{
		NeuralNetwork::SpliceNeuralNetworks(c1.m_network, c2.m_network);
	}

	// Creates the rays for the cars
	void Car::CreateRays(unsigned int rayCount, double raySize, float width, float height)
	{
		// Clears the ray ends list and reinitialises it
		raysEnds.clear();
		raysEnds.reserve(rayCount);

		// Sets the size of the ray
		Car::raySize = raySize;

		// Creates a vector2f for the origin of the rays
		sf::Vector2f rayOrigin = sf::Vector2f{width / 2.f, 0.f};

		// Calculates the angle interval for the rays
		float angle = 90.f / (rayCount - 1);

		// Creates a ray end of length raySize every interval from the origin
		for (unsigned int i = 0; i < rayCount; i++)
		{
			// Calculates the angle for the ray in radians
			float a = ((angle * i) - 45) * DEGTORAD;

			// Adds the ray to the ray ends list
			raysEnds.push_back(sf::Vector2f{ sinf(a), -cosf(a) } * (float)raySize + rayOrigin);
		}
	}

	// Updates the car for the given frame
	// Returns whether the car has been killed (or completed the track)
	bool Car::Update()
	{
		// Checks whether the car has died from exhaustion (living too long)
		if (m_frameCount++ > IDLETIME * Evolution::Simulation::SIMULATION_FRAMERATE)
		{
			// Sets the body outline to red and kill the car
			m_body.setOutlineColor(sf::Color::Red);
			m_alive = false;

			// Calculates the final fitness of the car
			m_fitness = RaceTrack::CalcFitness(m_body.getPosition(), m_nextCheckpoint);

			// Return true as update not needed and car has been killed
			return true;
		}

		// Creates a vector for the inputs
		std::vector<double> inputs;
		inputs.reserve(raysEnds.size());

		// Iterates through the ray ends
		for (unsigned int i = 0; i < raysEnds.size(); i++)
		{
			// Gets the position of the ray relative to the car
			m_rays[i].position = m_body.getTransform().transformPoint(raysEnds[i]);

			// Sets the old position of the ray to the current ray position
			sf::Vector2f oldPos = m_rays[i].position;

			// Checks to see if the a collision occured with the ray and a wall
			// Colours the ray accordingly
			if (RaceTrack::CheckCollisions(m_body.getPosition(), m_rays[i].position, m_rays[i].position))
			{
				m_rays[i].color = sf::Color::Red;
			}
			else
			{
				m_rays[i].color = sf::Color::Green;
			}

			// Adds the percentage of the ray that exists after the collision
			inputs.push_back((double)(sqrtf(powf(m_rays[i].position.x - oldPos.x, 2) + powf(m_rays[i].position.y - oldPos.y, 2)) / raySize));
		}

		// Gets the output of the cars neural network
		std::vector<double> output = m_network.GetOutput(inputs);

		// Moves the car
		m_body.rotate((output[1] - 0.5) * rotationPower);
		m_body.move(GetForward() * ((float)output[0] * enginePower) * 0.1f);

		// Check collisions between transformed position of the car and wall
		sf::Transform t = m_body.getTransform();
		if (RaceTrack::CheckCollisions(t.transformPoint(m_body.getPoint(0)), t.transformPoint(m_body.getPoint(1)),
			t.transformPoint(m_body.getPoint(2)), t.transformPoint(m_body.getPoint(3))))
		{
			// Kills the car
			m_body.setOutlineColor(sf::Color::Red);
			m_alive = false;
		}

		// Checks to see if the car has entered a checkpoint
		if (RaceTrack::CheckCheckpoints(m_body.getPosition(), m_nextCheckpoint))
		{
			// Increments the checkpoint counter and sees whether it has reached the finish
			if (++m_nextCheckpoint == RaceTrack::GetCheckpointCount())
			{
				// Sets the car to be completed (overrides death)
				m_body.setOutlineColor(sf::Color::Green);
				m_alive = false;
				m_complete = true;
			}
			// Resets the exhaustion timer
			m_frameCount = 0;
		}

		// Update the fitness if the car is no longer alive
		if (!m_alive)
		{
			m_fitness = RaceTrack::CalcFitness(m_body.getPosition(), m_nextCheckpoint);
		}

		// Returns whether the car has died
		return !m_alive;
	}

	// Draws the car to the target
	void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Draws the body of the car
		target.draw(m_body);
		
		// Draws the rays of the car if the car is alive
		if (m_alive)
		{
			std::for_each(m_rays.begin(), m_rays.end(), [&](sf::Vertex ray)
			{
				target.draw(&ray, 1, sf::Points);
			});
		}
	}

	// Returns the forward direction of the car
	sf::Vector2f Car::GetForward()
	{
		float rot = m_body.getRotation();
		return sf::Vector2f{ sinf(rot * DEGTORAD), -cosf(rot * DEGTORAD) };
	}

	// Returns the calculated fitness of the car
	const float Car::CalcFitness()
	{
		// Only recalculates the fitness if the car is alive (dead car has fitness finalised)
		if (m_alive)
			return m_fitness = RaceTrack::CalcFitness(m_body.getPosition(), m_nextCheckpoint);
		else
			return m_fitness;
	}

	// Returns the fitness of the car
	const float Car::GetFitness() const
	{
		return m_fitness;
	}

	// Returns whether the car is alive or not
	const bool Car::IsAlive() const
	{
		return m_alive;
	}

	// Resets the car back to its initial state
	void Car::Reset()
	{
		// Resets the body
		m_body.setPosition(RaceTrack::GetStartPos());
		m_body.setRotation(RaceTrack::GetStartRot());
		m_body.setOutlineColor(sf::Color::White);

		// Resets the alive and complete bools
		m_alive = true;
		m_complete = false;
		
		// Resets the next checkpoint and fitness values
		m_nextCheckpoint = 1;
		m_fitness = 0.f;
	}

	// Mutates the cars neural network
	void Car::Mutate()
	{
		m_network.Mutate();
	}

	// Sets whether this car is the best or not
	void Car::SetBestCar(bool isBest)
	{
		// Sets the outline to blue if the car is the best
		// Otherwise set the outline to what the outline of the car should be
		if (isBest)
			m_body.setOutlineColor(sf::Color::Cyan);
		else if (m_alive)
			m_body.setOutlineColor(sf::Color::White);
		else if (m_complete)
			m_body.setOutlineColor(sf::Color::Green);
		else
			m_body.setOutlineColor(sf::Color::Red);
	}

	// Returns the neural network of the car
	NeuralNetwork& Car::GetNeuralNetwork()
	{
		return m_network;
	}

	// Returns the position of the car
	sf::Vector2f Car::GetPos() const
	{
		return m_body.getPosition();
	}
}