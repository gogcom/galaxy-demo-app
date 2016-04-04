#ifndef GOGTRON_STATISTICS_H
#define GOGTRON_STATISTICS_H

#include <map>
#include <string>
#include <cassert>
#include <stdint.h>
#include <galaxy/GalaxyID.h>

namespace gogtron
{

    class Statistic
    {
    public:

        enum ValueType
        {
            NONE,
            INT,
            FLOAT,
            AVG
        };

        Statistic(const std::string& _name = "");

        Statistic(const std::string& _name, ValueType type);

        const std::string& GetName() const;
        int32_t GetInt() const;
        float GetFloat() const;
        float GetAvg() const;
        float GetWindow() const;
        ValueType GetType() const;

        void SetName(const std::string& _name);
        void SetInt(int32_t _value);
        void SetFloat(float _value);
        void SetAvg(float _value);
        void SetWindow(float _window);
        void SetType(ValueType _type);

    private:

        union Value
        {
            int32_t intValue;
            float floatValue;

            Value(int32_t value)
                : intValue(value)
            {
            }

            Value(float value)
                : floatValue(value)
            {
            }
        };

        Value value;
        ValueType valueType;
        std::string name;
        float window;
    };

    typedef std::map<std::string, Statistic> Statistics;

    typedef std::map<galaxy::api::GalaxyID, Statistics> UsersStatistics;

    class StatisticsFactory
    {
    public:

        static Statistics CreateDefaultStatistics();
    };

}

#endif