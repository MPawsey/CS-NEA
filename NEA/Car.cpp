#include "Car.h"

#include "RaceTrack.h"
#include "Window.h"
#include <array>
#include <algorithm>

constexpr float DEGTORAD = 3.14159265359f / 180.f;

namespace Machine
{
	Car::Car(float width, float height, std::vector<unsigned int> sizes)
		: m_network{sizes}
	{
		m_body.setSize(sf::Vector2f{width, height});
		m_body.setOrigin(m_body.getSize() / 2.f);
		m_body.setOutlineColor(sf::Color::White);
		m_body.setOutlineThickness(1.f);
		m_body.setFillColor(sf::Color::Transparent);

		m_rays.reserve(sizes[0]);
		for (unsigned int i = 0; i < sizes[0]; i++)
			m_rays.push_back(sf::Vertex{ sf::Vector2f{0.f, 0.f}, sf::Color::White });
	}

	Car::Car(const Car& car)
		: m_network{car.m_network}
	{
		m_body = car.m_body;
		m_rays = car.m_rays;
	}

	void Car::SpliceCars(Car& c1, Car& c2)
	{
		NeuralNetwork::SpliceNeuralNetworks(c1.m_network, c2.m_network);
	}

	void Car::CreateRays(unsigned int rayCount, double raySize, float width, float height)
	{
		raysEnds.reserve(rayCount);
		Car::raySize = raySize;

		// TODO actually check this
		sf::Vector2f rayOrigin = sf::Vector2f{width / 2.f, 0.f};

		float angle = 90.f / (rayCount - 1);
		for (unsigned int i = 0; i < rayCount; i++)
		{
			float a = ((angle * i) - 45) * DEGTORAD;
			raysEnds.push_back(sf::Vector2f{ sinf(a), -cosf(a) } * (float)raySize + rayOrigin);
		}
	}


	bool Car::Update()
	{
		if (m_frameCount++ > 4000)
		{
			m_alive = false;
			return true;
		}

		std::vector<double> inputs;
		inputs.reserve(raysEnds.size());

		sf::Vertex v;

		for (unsigned int i = 0; i < raysEnds.size(); i++)
		{
			m_rays[i].position = m_body.getTransform().transformPoint(raysEnds[i]);
		}

		for (sf::Vertex& point : m_rays)
		{
			sf::Vector2f oldPos = point.position;

			if (RaceTrack::CheckCollisions(m_body.getPosition(), point.position, point.position))
			{
				point.color = sf::Color::Red;
			}
			else
			{
				point.color = sf::Color::Green;
			}

			inputs.push_back((double)(sqrtf(powf(point.position.x - oldPos.x, 2) + powf(point.position.y - oldPos.y, 2)) / raySize));

		}

		// MOVE
		std::vector<double> output = m_network.GetOutput(inputs);
		float engine = output[0] * enginePower;
		m_body.rotate((output[1] - 0.5) * rotationPower);
		m_body.move(GetForward() * engine * 0.1f);

		// Check collisions
		sf::Transform t = m_body.getTransform();
		if (RaceTrack::CheckCollisions(t.transformPoint(m_body.getPoint(0)), t.transformPoint(m_body.getPoint(1)),
			t.transformPoint(m_body.getPoint(2)), t.transformPoint(m_body.getPoint(3))))
		{
			m_body.setOutlineColor(sf::Color::Red);
			m_alive = false;
		}
		else
		{
			m_body.setOutlineColor(sf::Color::White);
		}

		if (RaceTrack::CheckCheckpoints(m_body.getPosition(), m_nextCheckpoint))
		{
			if (++m_nextCheckpoint == RaceTrack::GetCheckpointCount() /* && !EvolutionManager::complete */)
			{
				// TODO
				//EvolutionManager::complete = true; 
				//logger.Info("Car has completed course in " + std::to_string(EvolutionManager::iterations) + " iterations!");
			}
			m_frameCount = 0;
		}

		if (!m_alive)
		{
			m_fitness = RaceTrack::CalcFitness(m_body.getPosition(), m_nextCheckpoint);
		}

		return !m_alive;
	}

	void Car::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_body);
		
		if (m_alive)
		{
			std::for_each(m_rays.begin(), m_rays.end(), [&](sf::Vertex ray)
			{
				target.draw(&ray, 1, sf::Points);
			});
		}
	}

	sf::Vector2f Car::GetForward()
	{
		float rot = m_body.getRotation();
		return sf::Vector2f{ sinf(rot * DEGTORAD), -cosf(rot * DEGTORAD) };
	}

	const float Car::CalcFitness()
	{
		if (m_alive)
			return m_fitness = RaceTrack::CalcFitness(m_body.getPosition(), m_nextCheckpoint);
		else
			return m_fitness;
	}

	const float Car::GetFitness() const
	{
		return m_fitness;
	}

	const bool Car::IsAlive() const
	{
		return m_alive;
	}

	void Car::Reset()
	{
		m_body.setPosition(RaceTrack::GetStartPos());
		m_body.setRotation(RaceTrack::GetStartRot());
		m_alive = true;
		m_nextCheckpoint = 1;
		m_fitness = 0.f;
	}

	void Car::Mutate()
	{
		m_network.Mutate();
	}

	sf::Vector2f Car::GetPos() const
	{
		return m_body.getPosition();
	}


}