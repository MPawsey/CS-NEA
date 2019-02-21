#pragma once

#include <vector>
#include <fstream>

namespace Machine
{


	class Neuron
	{
	private:
		std::vector<double> m_weights;
		double m_bias;


	public:
		static inline double mutatePC = 0.25, mutateSize = 0.3, splicePC = 0.6;

		static void SpliceNeurons(Neuron& n1, Neuron& n2);

		Neuron(std::vector<double> weights, double bias);
		Neuron(unsigned int prevLayerSize);
		Neuron(const Neuron& neuron);

		friend std::ostream& operator<<(std::ostream& stream, const Neuron& neuron);

		double GetOutput(std::vector<double> inputs);
		void Mutate();

		double GetWeight(size_t index) const;
		double GetBias() const;
	};


}