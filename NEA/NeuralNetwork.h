#pragma once

#include <vector>
#include "Neuron.h"
#include <sfml/Graphics.hpp>
#include "LineShape.h"

namespace Machine
{
	class NeuralNetwork : public sf::Drawable
	{
	private:
		const std::vector<unsigned int> m_sizes;

		std::vector<std::vector<Neuron*>> m_network;
		unsigned int m_nodeCount;
		unsigned int m_connectionCount;

		// Stuff to do with the rendering of the network
		sf::CircleShape* m_nodes;
		LineShape* m_connections;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
		static void SpliceNeuralNetworks(NeuralNetwork& n1, NeuralNetwork& n2);

		NeuralNetwork(std::vector<unsigned int> sizes, std::vector<std::vector<std::vector<double>>> networkWeights, std::vector<std::vector<double>> networkBiases);
		NeuralNetwork(std::vector<unsigned int> sizes);
		NeuralNetwork(const NeuralNetwork& n);
		~NeuralNetwork();

		std::vector<double> GetOutput(std::vector<double> inputs, unsigned int layer = 0);
		void Mutate();
		void CreateNetworkDiagram();
	};


}