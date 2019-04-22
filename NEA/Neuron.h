#pragma once

#include <vector>
#include <fstream>

namespace Machine
{


	class Neuron
	{
	private:
		// Values for the neuron
		std::vector<double> m_weights;
		double m_bias;


	public:
		// Settings applied to all neurons
		static inline double mutatePC = 0.25, mutateSize = 0.3, splicePC = 0.6;

		// Splices the neurons together
		// n1 and n2 transformed into neurons with randomly swapped values
		static void SpliceNeurons(Neuron& n1, Neuron& n2);

		// Constructors

		// Creates and initialises a neuron with random values
		Neuron(unsigned int prevLayerSize);
		// Creates and initialises a neuron with preset values
		Neuron(std::vector<double> weights, double bias);
		// Creates and initialises a neuron from another neuron (copy constructor)
		Neuron(const Neuron& neuron);

		// Friend function to append the details of the neuron to an output stream
		friend std::ostream& operator<<(std::ostream& stream, const Neuron& neuron);

		// Returns the output of the neuron given a vector of inputs
		double GetOutput(std::vector<double> inputs);

		// Mutates the neuron
		void Mutate();

		// Returns the weight of the neuron at a given index
		double GetWeight(size_t index) const;
		// Returns the bias of the neuron
		double GetBias() const;
	};


}