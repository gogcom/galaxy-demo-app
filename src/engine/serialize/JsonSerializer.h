#ifndef GALAXY_DEMO_SERIALIZE_JSON_SERIALIZER_H
#define GALAXY_DEMO_SERIALIZE_JSON_SERIALIZER_H

#include "JsonSerializable.h"
#include <string>

namespace galaxy::demo
{
	namespace serialize
	{

		class JsonSerializer
		{
		public:

			JsonSerializer() = delete;
			JsonSerializer(const JsonSerializer&) = delete;

			static bool Serialize(JsonSerializable* object, std::string& output);
			static bool Deserialize(JsonSerializable* object, const std::string& input);
		};

	}
}

#endif