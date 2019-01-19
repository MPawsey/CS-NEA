#pragma once

#include <vector>


namespace Machine
{


	class Neuron
	{
	private:
		std::vector<double> m_weights;
		double m_bias;


	public:
		static inline double mutatePC = 0.25, splicePC = 0.6;

		static void SpliceNeurons(Neuron& n1, Neuron& n2);

		Neuron(unsigned int prevLayerSize);
		Neuron(const Neuron& neuron);

		double GetOutput(std::vector<double> inputs);
		void Mutate();


	};


}