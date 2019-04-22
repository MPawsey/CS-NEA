#include "Neuron.h"

#include <random>
#include <functional>
#include <numeric>
#include "EvolutionManager.h"

namespace Machine
{
	// The function to be used as the activation function for the neuron
	double SigmoidFunction(double z)
	{
		return 1.0 / (1.0 + exp(-z));
	}

	// Creates a neuron from a list of weights and a bias
	Neuron::Neuron(std::vector<double> weights, double bias)
	{
		m_weights = weights;
		m_bias = bias;
	}

	// Creates a neuron with random values
	Neuron::Neuron(unsigned int prevLayerSize)
	{
		// Creates a lambda expression that returns a normally distributed random value
		auto rand = std::bind(std::normal_distribution{}, std::ref(Evolution::EvolutionManager::GetEvolutionManager().GetRandomEngine()));

		// Adds weights to the weights vector
		m_weights.reserve(prevLayerSize);
		for (unsigned int i = 0; i < prevLayerSize; i++)
		{
			m_weights.push_back(rand());
		}
		// Sets the bias of the neuron
		m_bias = rand();
	}

	// Creates a neuron from another neuron
	Neuron::Neuron(const Neuron& neuron)
	{
		// Copies the weights of the neuron to this neuron
		for (unsigned int i = 0; i < neuron.m_weights.size(); i++)
			m_weights.push_back(neuron.m_weights[i]);
		// Copies the bias of the neuron to this neuron
		m_bias = neuron.m_bias;
	}

	// Writes the neuron to an output file stream
	std::ostream& operator<<(std::ostream& stream, const Neuron& neuron)
	{
		// Writes an n followed by a space to signify a neuron
		stream << "n ";

		// Writes each weight seperated by a space
		for (double weight : neuron.m_weights)
		{
			stream << weight << ' ';
		}
		// Write the neuron
		stream << neuron.m_bias;

		// Returns the stream so calls can be chained
		return stream;
	}

	// Splices two neurons
	// Randomly swaps values between the two neurons
	void Neuron::SpliceNeurons(Neuron& n1, Neuron& n2)
	{
		// Creates a lambda expression that returns a uniformly random value between 0 and 1
		auto pc = std::bind(std::uniform_real_distribution{}, std::ref(Evolution::EvolutionManager::GetEvolutionManager().GetRandomEngine()));

		// Iterates through all of the weights in the networks
		for (unsigned int i = 0; i < n1.m_weights.size(); i++)
		{
			// Checks whether it should splice the weights
			if (pc() < splicePC)
			{
				// Swaps the weights
				double temp = n2.m_weights[i];
				n2.m_weights[i] = n1.m_weights[i];
				n1.m_weights[i] = temp;
			}
		}

		// Checks whether it should splice the biases
		if (pc() < splicePC)
		{
			// Swaps the biases
			double temp = n2.m_bias;
			n2.m_bias = n1.m_bias;
			n1.m_bias = temp;
		}
	}

	// Returns the output of the neuron
	double Neuron::GetOutput(std::vector<double> inputs)
	{
		// Creates the starting value for the neuron
		double sum = 0.0;

		// Iterates through the inputs applying their respective weights and adding it to sum
		for (int i = 0; i < inputs.size(); i++)
		{
			sum += (inputs[i] * m_weights[i]);
		}
		// Adds the bias to the sum
		sum += m_bias;

		// Returns the activation function (Sigmoid function) value of the sum
		return SigmoidFunction(sum);
	}

	// Mutates the neuron
	void Neuron::Mutate()
	{
		// Creates a lambda expression that returns a uniformly random value between 0 and 1 and a normally distributed random value with S.D of mutateSize
		auto pc = std::bind(std::uniform_real_distribution{}, std::ref(Evolution::EvolutionManager::GetEvolutionManager().GetRandomEngine()));
		auto rand = std::bind(std::normal_distribution{ 0.0, mutateSize }, std::ref(Evolution::EvolutionManager::GetEvolutionManager().GetRandomEngine()));

		// Iterates through each of the weights
		for (unsigned int i = 0; i < m_weights.size(); i++)
		{
			// Adds a uniformly random value to the weight if needed
			if (pc() < mutatePC)
			{
				m_weights[i] += rand();
			}
		}

		// Adds a uniformly random value to the bias if needed
		if (pc() < mutatePC)
		{
			m_bias += rand();
		}
	}

	// Returns the weight at a given index
	double Neuron::GetWeight(size_t index) const
	{
		return m_weights[index];
	}

	// Returns the bias
	double Neuron::GetBias() const
	{
		return m_bias;
	}

}