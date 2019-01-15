#pragma once

#include <vector>
#include "Neuron.h"

namespace Machine
{

	class NeuralNetwork
	{
	private:
		std::vector<std::vector<Neuron>> m_network;


	public:
		static void SpliceNeuralNetworks(NeuralNetwork& n1, NeuralNetwork& n2);

		NeuralNetwork(std::vector<unsigned int> sizes);
		NeuralNetwork(const NeuralNetwork& n);

		std::vector<double> GetOutput(std::vector<double> inputs, unsigned int layer = 0);
		void Mutate();
	};


}