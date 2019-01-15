#pragma once

#include "NeuralNetwork.h"

namespace Machine
{


	NeuralNetwork::NeuralNetwork(std::vector<unsigned int> sizes)
	{
		m_network.reserve(sizes.size());

		for (unsigned int i = 1; i < sizes.size(); i++)
		{
			m_network.push_back(std::vector<Neuron>());
			m_network.back().reserve(sizes[i]);

			for (unsigned int j = 0; j < sizes[i]; j++)
			{
				m_network.back().emplace_back(sizes[i - 1]);
			}
		}

	}

	NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn)
	{
		m_network.reserve(nn.m_network.size());

		for (unsigned int i = 0; i < nn.m_network.size(); i++)
		{
			m_network.push_back(std::vector<Neuron>());

			for (unsigned int j = 0; j < nn.m_network[i].size(); j++)
			{
				m_network[i].emplace_back( nn.m_network[i][j] );
			}
		}

	}

	void NeuralNetwork::SpliceNeuralNetworks(NeuralNetwork& nn1, NeuralNetwork& nn2)
	{
		for (unsigned int i = 0; i < nn1.m_network.size(); i++)
		{
			for (unsigned int j = 0; j < nn1.m_network[i].size(); j++)
			{
				Neuron::SpliceNeurons(nn1.m_network[i][j], nn2.m_network[i][j]);
			}
		}
	}

	std::vector<double> NeuralNetwork::GetOutput(std::vector<double> inputs, unsigned int layer)
	{
		if (layer == m_network.size())
			return inputs;

		std::vector<double> outputs;
		outputs.reserve(m_network[layer].size());

		for (Neuron& neuron : m_network[layer])
		{
			outputs.push_back(neuron.GetOutput(inputs));
		}

		return GetOutput(outputs, layer + 1);
	}

	void NeuralNetwork::Mutate()
	{
		for (unsigned int i = 0; i < m_network.size(); i++)
		{
			for (unsigned int j = 0; j < m_network[i].size(); j++)
			{
				m_network[i][j].Mutate();
			}
		}
	}



}