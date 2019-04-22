#pragma once

#include "NeuralNetwork.h"
#include <numeric>
#include <functional>

namespace Machine
{
	// Creates a neural network given a series of weights and biases
	NeuralNetwork::NeuralNetwork(std::vector<unsigned int> sizes, std::vector<std::vector<std::vector<double>>> networkWeights, std::vector<std::vector<double>> networkBiases)
		: m_sizes{sizes}
	{
		// Reserves the size of the network vector so that it isn't allocated as it is adding
		m_network.reserve(sizes.size());

		// Loops through the network sizes (ignoring the first as they are the input layer)
		for (unsigned int i = 1; i < sizes.size(); i++)
		{
			// Adds a new vector of neurons to the network and reserve the size of it
			m_network.push_back(std::vector<Neuron*>());
			m_network.back().reserve(sizes[i]);

			// Adds neurons to the layer
			for (unsigned int j = 0; j < sizes[i]; j++)
			{
				m_network.back().push_back(new Neuron(networkWeights[i - 1][j], networkBiases[i - 1][j]));
			}
		}
	}

	// Creates a neural network with random values
	NeuralNetwork::NeuralNetwork(std::vector<unsigned int> sizes)
		: m_sizes{sizes}
	{
		// Reserves the size of the network vector so that it isn't allocated as it is adding
		m_network.reserve(sizes.size());

		// Loops through the network sizes (ignoring the first as they are the input layer)
		for (unsigned int i = 1; i < sizes.size(); i++)
		{
			// Adds a new vector of neurons to the network and reserve the size of it
			m_network.push_back(std::vector<Neuron*>());
			m_network.back().reserve(sizes[i]);

			// Adds neurons to the layer
			for (unsigned int j = 0; j < sizes[i]; j++)
			{
				m_network.back().push_back(new Neuron(sizes[i - 1]));
			}
		}

	}

	// Creates a neural network from another neural network
	NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn)
		: m_sizes{ nn.m_sizes }
	{
		// Reserves the size of the network vector so that it isn't allocated as it is adding
		m_network.reserve(nn.m_network.size());

		// Loops through the network in the other network
		for (unsigned int i = 0; i < nn.m_network.size(); i++)
		{
			// Adds a new vector of neurons to the network and reserve the size of it
			m_network.push_back(std::vector<Neuron*>());
			m_network.back().reserve(nn.m_sizes[i]);

			// Adds a copy of each neuron in the layer to this network
			for (unsigned int j = 0; j < nn.m_network[i].size(); j++)
			{
				m_network[i].push_back( new Neuron(*nn.m_network[i][j]) );
			}
		}

		// Copies the connections from the nn to this network
		m_connections = new LineShape[m_connectionCount = nn.m_connectionCount];
		for (size_t i = 0; i < nn.m_connectionCount; i++)
		{
			m_connections[i] = nn.m_connections[i];
		}

		// Copies the nodes from the nn to this network
		m_nodes = new sf::CircleShape[m_nodeCount = nn.m_nodeCount];
		for (size_t i = 0; i < nn.m_nodeCount; i++)
		{
			m_nodes[i] = nn.m_nodes[i];
		}
	}

	// Destructor
	NeuralNetwork::~NeuralNetwork()
	{
		// Deletes all of the neurons in the network
		for (auto& layer : m_network)
		{
			for (Neuron* n : layer)
				delete n;
		}

		// Deletes the connections and nodes
		delete[] m_connections;
		delete[] m_nodes;
	}

	// Writes the network to an output file stream
	std::ostream& operator<<(std::ostream& stream, const NeuralNetwork& nn)
	{
		// Iterates through each later in the network
		for (auto layer : nn.m_network)
		{
			// Adds the character 'l' to the a line and move to the next line
			stream << 'l' << std::endl;

			// Writes each neuron to a new line
			for (Neuron* n : layer)
			{
				stream << *n << std::endl;
			}
		}

		// Returns the stream so that it can be chained
		return stream;
	}

	// Draws the network to the target
	void NeuralNetwork::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Draws the connections of the network
		for (size_t i = 0; i < m_connectionCount; i++)
		{
			target.draw(m_connections[i]);
		}
		
		// Draws the nodes of the network (ontop of the connections)
		for (size_t i = 0; i < m_nodeCount; i++)
		{
			target.draw(m_nodes[i]);
		}
	}

	// Splices two networks together
	// Randomly swaps values between the two networks
	void NeuralNetwork::SpliceNeuralNetworks(NeuralNetwork& nn1, NeuralNetwork& nn2)
	{
		// Iterates through each layer in the networks
		for (unsigned int i = 0; i < nn1.m_network.size(); i++)
		{
			// Iterates through each neuron in the networks
			for (unsigned int j = 0; j < nn1.m_network[i].size(); j++)
			{
				// Splices the neurons
				Neuron::SpliceNeurons(*nn1.m_network[i][j], *nn2.m_network[i][j]);
			}
		}
	}

	// Recursive algorithm that takes an input and returns the output of the network
	// Layer should be 0 unless called from inside of the function
	std::vector<double> NeuralNetwork::GetOutput(std::vector<double> inputs, unsigned int layer)
	{
		// Checks to see if the network has been traversed
		// Returns the input if it has
		if (layer == m_network.size())
			return inputs;

		// Creates a vector for the output
		std::vector<double> outputs;
		outputs.reserve(m_network[layer].size());

		// Gets the output of each neuron given its respective neuron and adds it to the output
		for (Neuron* neuron : m_network[layer])
		{
			outputs.push_back(neuron->GetOutput(inputs));
		}

		// Returns GetOutput for the next layer
		return GetOutput(outputs, layer + 1);
	}

	// Mutates the neural network
	void NeuralNetwork::Mutate()
	{
		// Iterates through each layer of the network
		for (unsigned int i = 0; i < m_network.size(); i++)
		{
			// Iterates through each neuron of the layer
			for (unsigned int j = 0; j < m_network[i].size(); j++)
			{
				// Mutates the neuron
				m_network[i][j]->Mutate();
			}
		}
	}

	// Creates a diagram for the network
	void NeuralNetwork::CreateNetworkDiagram()
	{
		// Deletes the nodes and connections if they exists to free up memory
		if (m_nodes)
			delete[] m_nodes;
		if (m_connections)
			delete[] m_connections;

		// The network diagram is created in normalised space
		// i.e. all values range from [0.0f-1.0f]

		// Calculates the positioning values of the nodes
		float padding = 0.1f;
		float xSpacing = (1.f - (2.f * padding)) / (m_sizes.size()-1);
		float ySpacing = (1.f - (2.f * padding)) / *std::max_element(m_sizes.begin(), m_sizes.end());

		// Creates a template node
		sf::CircleShape base{ 0.05f };
		base.setOrigin(0.05f, 0.05f);
		base.setFillColor(sf::Color{ 40, 50, 70 });
		base.setOutlineThickness(0.005f);
		base.setOutlineColor(sf::Color::White);

		// Creates a new array of nodes
		m_nodes = new sf::CircleShape[m_nodeCount = std::accumulate(m_sizes.begin(), m_sizes.end(), 0)];
		size_t nodeIndex = 0;

		// Calculates the number of connections in the diagram and creates the array of connections
		m_connectionCount = 0;
		for (size_t i = 1; i < m_sizes.size(); i++)
		{
			m_connectionCount += m_sizes[i] * m_sizes[i - 1];
		}
		m_connections = new LineShape[m_connectionCount];
		size_t connectionIndex = 0;

		// Values for the min/max thickness of the lines
		float minThickness = 0.005f;
		float maxThickness = 0.01f;

		// Calculates the starting y position for the nodes
		float yStart = 0.5f - (((m_sizes[0] - 1) / 2.f) * ySpacing);

		// Creates the input nodes from the template and adds it to the nodes array
		for (size_t i = 0; i < m_sizes[0]; i++)
		{
			base.setPosition(padding, yStart + ySpacing * i);
			m_nodes[nodeIndex++] = sf::CircleShape{base};
		}

		// Value that indicates the start of the previous layer in the nodes array
		size_t layerStart = 0;

		// Iterates through all of the layers in the network
		for (size_t i = 0; i < m_network.size(); i++)
		{
			// Calculates the starting y position for the nodes
			float yStart = 0.5f - (((m_network[i].size() - 1) / 2.f) * ySpacing);

			// Iterates through the neurons in the layer
			for (size_t j = 0; j < m_network[i].size(); j++)
			{
				// Gets the neuron from the network
				Neuron* n = m_network[i][j];

				// Creates the node from the neuron and node template and adds it to the nodes array
				float bias = n->GetBias();
				base.setOutlineColor(bias > 0 ? sf::Color::Green : sf::Color::Red);
				base.setOutlineThickness(std::clamp(fabsf(bias / 100.f), minThickness, maxThickness));
				base.setPosition(padding + xSpacing * (i+1), yStart + ySpacing * j);
				m_nodes[nodeIndex++] = sf::CircleShape{ base };

				// Iterates through the previous layer of neuron nodes
				for (size_t k = 0; k < m_sizes[i]; k++)
				{
					// Gets the weight from the current neuron
					float weight = n->GetWeight(k);

					// Creates a line connecting the current node to the node at position k
					LineShape line{ m_nodes[layerStart + k].getPosition(), base.getPosition() };

					// Sets the line colour and thickness and adds it to the connections array
					line.SetLineColour(weight > 0 ? sf::Color::Green : sf::Color::Red);
					line.SetLineThickness(std::clamp(fabsf(weight / 100.f), minThickness, maxThickness));
					m_connections[connectionIndex++] = line;
				}
			}

			// Updates the index of the start of the previous layer of nodes
			layerStart += m_sizes[i];
		}
	}
}