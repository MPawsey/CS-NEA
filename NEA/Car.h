#pragma once

#include <sfml/Graphics.hpp>
#include "NeuralNetwork.h"
#include <fstream>

namespace Machine
{



	class Car : public sf::Drawable
	{
	private:
		// The rays (static as the are transformed per update)
		static inline std::vector<sf::Vector2f> raysEnds;
		static inline double raySize;

		// The car
		sf::RectangleShape m_body;
		unsigned int m_nextCheckpoint, m_frameCount;
		NeuralNetwork m_network;
		bool m_alive, m_complete;
		std::vector<sf::Vertex> m_rays;
		float m_fitness;

		// Gets the forward vector for the car
		sf::Vector2f GetForward();

		// Draws to the car to the target
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		// Settings used between cars
		static inline float enginePower, rotationPower;
		
		// Splices the cars together
		// c1 and c2 transformed into cars with randomly swapped values
		static void SpliceCars(Car& c1, Car& c2);

		// Creates the rays the cars use
		static void CreateRays(unsigned int rayCount, double raySize, float width, float height);

		// Constructors
		
		// Creates and initialises a car with random values
		Car(float width, float height, std::vector<unsigned int> sizes);
		// Creates and initialises a car with preset values
		Car(float width, float height, std::vector<unsigned int> sizes, std::vector<std::vector<std::vector<double>>> networkWeights, std::vector<std::vector<double>> networkBiases);
		// Creates and initialises a car from another car (copy constructor)
		Car(const Car& car);

		// Friend function to append the details of the car to an output stream
		friend std::ostream& operator<<(std::ostream& stream, const Car& car);

		// Updates the car for a given frame
		// Returns whether the car should still be simulated next frame
		bool Update();

		// Resets the car to the start of the track
		void Reset();

		// Mutates the neurons in the car
		void Mutate();

		// Sets the outline of the car
		void SetBestCar(bool isBest);

		// Calculates and returns the fitness of the car
		const float CalcFitness();

		// Returns the fitness of the car (must be calculated first)
		const float GetFitness() const;

		// Returns whether the car is alive
		const bool IsAlive() const;

		// Returns the neural network of the car
		NeuralNetwork& GetNeuralNetwork();

		// Returns the position of the car
		sf::Vector2f GetPos() const;

	};





}