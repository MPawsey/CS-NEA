#pragma once

#include <sfml/Graphics.hpp>
#include "NeuralNetwork.h"

namespace Machine
{



	class Car : public sf::Drawable
	{
	private:
		static inline std::vector<sf::Vector2f> raysEnds;
		static inline double raySize;

		float m_fitness;
		sf::RectangleShape m_body;
		unsigned int m_nextCheckpoint;
		NeuralNetwork m_network;
		unsigned int m_frameCount;
		bool m_alive;
		std::vector<sf::Vertex> m_rays;

		sf::Vector2f GetForward();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
		static inline float enginePower, rotationPower;
		
		static void CreateRays(unsigned int rayCount, double raySize, float width, float height);

		Car(float width, float height, std::vector<unsigned int> sizes);
		Car(const Car& car);

		bool operator==(const Car& car)
		{
			return m_fitness == car.m_fitness;
		}

		bool Update();
		void Reset();
		void Mutate();

		const float CalcFitness();
		const float GetFitness() const;
		const bool IsAlive() const;

		sf::Vector2f GetPos() const;

	};





}