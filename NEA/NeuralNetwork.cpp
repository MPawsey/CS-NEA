#pragma once

#include "NeuralNetwork.h"
#include <numeric>
#include <functional>

namespace Machine
{

	NeuralNetwork::NeuralNetwork(std::vector<unsigned int> sizes)
		: m_sizes{sizes}
	{
		m_network.reserve(sizes.size());

		for (unsigned int i = 1; i < sizes.size(); i++)
		{
			m_network.push_back(std::vector<Neuron*>());
			m_network.back().reserve(sizes[i]);

			for (unsigned int j = 0; j < sizes[i]; j++)
			{
				m_network.back().push_back(new Neuron(sizes[i - 1]));
			}
		}

	}

	NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn)
		: m_sizes{ nn.m_sizes }
	{
		m_network.reserve(nn.m_network.size());

		for (unsigned int i = 0; i < nn.m_network.size(); i++)
		{
			m_network.push_back(std::vector<Neuron*>());

			for (unsigned int j = 0; j < nn.m_network[i].size(); j++)
			{
				m_network[i].push_back( new Neuron(*nn.m_network[i][j]) );
			}
		}

		m_connections = new LineShape[m_connectionCount = nn.m_connectionCount];
		for (size_t i = 0; i < nn.m_connectionCount; i++)
		{
			m_connections[i] = nn.m_connections[i];
		}

		m_nodes = new sf::CircleShape[m_nodeCount = nn.m_nodeCount];
		for (size_t i = 0; i < nn.m_nodeCount; i++)
		{
			m_nodes[i] = nn.m_nodes[i];
		}

	}

	NeuralNetwork::~NeuralNetwork()
	{
		for (auto& layer : m_network)
		{
			for (Neuron* n : layer)
				delete n;
		}

		delete[] m_connections;
		delete[] m_nodes;
	}

	void NeuralNetwork::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (size_t i = 0; i < m_connectionCount; i++)
		{
			target.draw(m_connections[i]);
		}
		
		for (size_t i = 0; i < m_nodeCount; i++)
		{
			target.draw(m_nodes[i]);
		}
	}

	void NeuralNetwork::SpliceNeuralNetworks(NeuralNetwork& nn1, NeuralNetwork& nn2)
	{
		for (unsigned int i = 0; i < nn1.m_network.size(); i++)
		{
			for (unsigned int j = 0; j < nn1.m_network[i].size(); j++)
			{
				Neuron::SpliceNeurons(*nn1.m_network[i][j], *nn2.m_network[i][j]);
			}
		}
	}

	std::vector<double> NeuralNetwork::GetOutput(std::vector<double> inputs, unsigned int layer)
	{
		if (layer == m_network.size())
			return inputs;

		std::vector<double> outputs;
		outputs.reserve(m_network[layer].size());

		for (Neuron* neuron : m_network[layer])
		{
			outputs.push_back(neuron->GetOutput(inputs));
		}

		return GetOutput(outputs, layer + 1);
	}

	void NeuralNetwork::Mutate()
	{
		for (unsigned int i = 0; i < m_network.size(); i++)
		{
			for (unsigned int j = 0; j < m_network[i].size(); j++)
			{
				m_network[i][j]->Mutate();
			}
		}
	}

	void NeuralNetwork::CreateNetworkDiagram()
	{
		if (m_nodes)
			delete[] m_nodes;
		if (m_connections)
			delete[] m_connections;

		// The network diagram is created in normalised space
		// i.e. all values range from [0.0f-1.0f]

		float padding = 0.1f;
		float xSpacing = (1.f - (2.f * padding)) / (m_sizes.size()-1);
		float ySpacing = (1.f - (2.f * padding)) / *std::max_element(m_sizes.begin(), m_sizes.end());
		sf::CircleShape base{ 0.05f };
		base.setOrigin(0.05f, 0.05f);
		base.setFillColor(sf::Color{ 40, 50, 70 });
		base.setOutlineThickness(0.005f);

		m_nodes = new sf::CircleShape[m_nodeCount = std::accumulate(m_sizes.begin(), m_sizes.end(), 0)];
		size_t nodeIndex = 0;

		m_connectionCount = 0;
		for (size_t i = 1; i < m_sizes.size(); i++)
		{
			m_connectionCount += m_sizes[i] * m_sizes[i - 1];
		}
		m_connections = new LineShape[m_connectionCount];
		size_t connectionIndex = 0;
		float minThickness = 0.005f;
		float maxThickness = 0.01f;


		base.setOutlineColor(sf::Color::White);
		float yStart = 0.5f - (((m_sizes[0] - 1) / 2.f) * ySpacing);
		for (size_t i = 0; i < m_sizes[0]; i++)
		{
			base.setPosition(padding, yStart + ySpacing * i);
			m_nodes[nodeIndex++] = sf::CircleShape{base};
		}

		size_t layerStart = 0;
		for (size_t i = 0; i < m_network.size(); i++)
		{
			float yStart = 0.5f - (((m_network[i].size() - 1) / 2.f) * ySpacing);

			for (size_t j = 0; j < m_network[i].size(); j++)
			{
				Neuron* n = m_network[i][j];
				float bias = n->GetBias();
				base.setOutlineColor(bias > 0 ? sf::Color::Green : sf::Color::Red);
				base.setOutlineThickness(std::clamp(fabsf(bias / 100.f), minThickness, maxThickness));
				base.setPosition(padding + xSpacing * (i+1), yStart + ySpacing * j);
				m_nodes[nodeIndex++] = sf::CircleShape{ base };

				for (size_t k = 0; k < m_sizes[i]; k++)
				{
					LineShape line{ m_nodes[layerStart + k].getPosition(), base.getPosition() };
					float weight = n->GetWeight(k);
					line.SetLineColour(weight > 0 ? sf::Color::Green : sf::Color::Red);
					float thickness = std::clamp(fabsf(weight / 100.f), minThickness, maxThickness);
					line.SetLineThickness(thickness);
					m_connections[connectionIndex++] = line;
				}
			}

			layerStart += m_sizes[i];
		}


	}


}