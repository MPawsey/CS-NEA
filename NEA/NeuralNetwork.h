#pragma once

#include <vector>
#include "Neuron.h"
#include <sfml/Graphics.hpp>
#include "LineShape.h"
#include <fstream>

namespace Machine
{
	class NeuralNetwork : public sf::Drawable
	{
	private:
		// Network counts
		unsigned int m_nodeCount;
		unsigned int m_connectionCount;

		// Network Rendering
		sf::CircleShape* m_nodes;
		LineShape* m_connections;

		// Other
		const std::vector<unsigned int> m_sizes;
		std::vector<std::vector<Neuron*>> m_network;

		// Draws the neural network to the target
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		// Splices the neural networks together
		// n1 and n2 transformed into networks with randomly swapped values
		static void SpliceNeuralNetworks(NeuralNetwork& n1, NeuralNetwork& n2);

		// Constructors

		// Creates and initialises a network with random values
		NeuralNetwork(std::vector<unsigned int> sizes);
		// Creates and initialises a network with preset values
		NeuralNetwork(std::vector<unsigned int> sizes, std::vector<std::vector<std::vector<double>>> networkWeights, std::vector<std::vector<double>> networkBiases);
		// Creates and initialises a network from another network (copy constructor)
		NeuralNetwork(const NeuralNetwork& n);
		
		// Destructor
		~NeuralNetwork();

		// Friend function to append the details of the network to an output stream
		friend std::ostream& operator<<(std::ostream& stream, const NeuralNetwork& nn);

		// Gets the output of the network
		// layer should not be set to anything other than 0
		// Returns the output of the network
		std::vector<double> GetOutput(std::vector<double> inputs, unsigned int layer = 0);

		// Mutates the neurons of the network
		void Mutate();

		// Creates a visual diagram of the network
		void CreateNetworkDiagram();
	};


}