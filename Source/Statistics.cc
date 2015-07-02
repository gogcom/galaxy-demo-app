#include "Statistics.h"

namespace game
{
	Statistic::Statistic(const std::string& _name)
		: value(0)
		, valueType(NONE)
		, name(_name)
		, window(0)
	{
	}

	Statistic::Statistic(const std::string& _name, ValueType type)
		: value(0)
		, valueType(type)
		, name(_name)
		, window(0)
	{
	}

	const std::string& Statistic::GetName() const
	{
		return name;
	}

	int32_t Statistic::GetInt() const
	{
		assert(valueType == INT);
		return value.intValue;
	}

	float Statistic::GetFloat() const
	{
		assert(valueType == FLOAT || valueType == AVG);
		return value.floatValue;
	}

	float Statistic::GetAvg() const
	{
		assert(valueType == AVG);
		return value.floatValue;
	}

	float Statistic::GetWindow() const
	{
		assert(valueType == AVG);
		return window;
	}

	Statistic::ValueType Statistic::GetType() const
	{
		return valueType;
	}

	void Statistic::SetName(const std::string& _name)
	{
		name = _name;
	}

	void Statistic::SetInt(int32_t _value)
	{
		assert(valueType == INT || valueType == NONE);
		valueType = INT;
		value.intValue = _value;
	}

	void Statistic::SetFloat(float _value)
	{
		assert(valueType == FLOAT || valueType == NONE);
		valueType = FLOAT;
		value.floatValue = _value;
	}

	void Statistic::SetAvg(float _value)
	{
		assert(valueType == AVG || valueType == NONE);
		valueType = AVG;
		value.floatValue = _value;
	}

	void Statistic::SetWindow(float _value)
	{
		assert(valueType == AVG);
		window = _value;
	}

	void Statistic::SetType(ValueType type)
	{
		valueType = type;
	}

	Statistics StatisticsFactory::CreateDefaultStatistics()
	{
		Statistics statistics;
		Statistic intStatistic;
		intStatistic.SetInt(0);
		intStatistic.SetName("int_statistic");
		statistics[intStatistic.GetName()] = intStatistic;

		Statistic floatStatistic;
		floatStatistic.SetFloat(0);
		floatStatistic.SetName("float_statistic");
		statistics[floatStatistic.GetName()] = floatStatistic;

		Statistic avgStatistic;
		avgStatistic.SetAvg(0);
		avgStatistic.SetWindow(20);
		avgStatistic.SetName("avg_statistic");
		statistics[avgStatistic.GetName()] = avgStatistic;
		return statistics;
	}

}