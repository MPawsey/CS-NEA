#include "Neuron.h"

#include <random>
#include <functional>
#include <numeric>
#include "EvolutionManager.h"

namespace Machine
{
	double SigmoidFunction(double z)
	{
		return 1.0 / (1.0 + exp(-z));
	}


	Neuron::Neuron(std::vector<double> weights, double bias)
	{
		m_weights = weights;
		m_bias = bias;
	}

	Neuron::Neuron(unsigned int prevLayerSize)
	{
		auto rand = std::bind(std::normal_distribution{}, std::ref(Evolution::EvolutionManager::GetEvolutionManager().GetRandomEngine()));

		m_weights.reserve(prevLayerSize);
		for (unsigned int i = 0; i < prevLayerSize; i++)
		{
			m_weights.push_back(rand());
		}
		m_bias = rand();
	}

	Neuron::Neuron(const Neuron& neuron)
	{
		for (unsigned int i = 0; i < neuron.m_weights.size(); i++)
			m_weights.push_back(neuron.m_weights[i]);
		m_bias = neuron.m_bias;
	}

	std::ostream& operator<<(std::ostream& stream, const Neuron& neuron)
	{
		stream << "n ";
		for (double weight : neuron.m_weights)
		{
			stream << weight << ' ';
		}
		stream << neuron.m_bias;

		return stream;
	}

	void Neuron::SpliceNeurons(Neuron& n1, Neuron& n2)
	{
		auto pc = std::bind(std::uniform_real_distribution{}, std::ref(Evolution::EvolutionManager::GetEvolutionManager().GetRandomEngine()));

		for (unsigned int i = 0; i < n1.m_weights.size(); i++)
		{
			if (pc() < splicePC)
			{
				double temp = n2.m_weights[i];
				n2.m_weights[i] = n1.m_weights[i];
				n1.m_weights[i] = temp;
			}
		}

		if (pc() < splicePC)
		{
			double temp = n2.m_bias;
			n2.m_bias = n1.m_bias;
			n1.m_bias = temp;
		}
	}

	double Neuron::GetOutput(std::vector<double> inputs)
	{
		std::vector<double> list;
		list.reserve(inputs.size());

		std::transform(inputs.begin(), inputs.end(), m_weights.begin(), std::back_inserter(list), [](double a, double b)
		{
			return a * b;
		});

		list.push_back(m_bias);

		return SigmoidFunction(std::accumulate(list.begin(), list.end(), 0.0));
	}

	void Neuron::Mutate()
	{
		auto pc = std::bind(std::uniform_real_distribution{}, std::ref(Evolution::EvolutionManager::GetEvolutionManager().GetRandomEngine()));
		auto rand = std::bind(std::normal_distribution{ 0.0, mutateSize }, std::ref(Evolution::EvolutionManager::GetEvolutionManager().GetRandomEngine()));

		for (unsigned int i = 0; i < m_weights.size(); i++)
		{
			if (pc() < mutatePC)
			{
				m_weights[i] += rand();
			}
		}

		if (pc() < mutatePC)
		{
			m_bias += rand();
		}
	}

	double Neuron::GetWeight(size_t index) const
	{
		return m_weights[index];
	}

	double Neuron::GetBias() const
	{
		return m_bias;
	}

}