#ifndef GOGTRON_SERIALIZE_JSON_SERIALIZER_H
#define GOGTRON_SERIALIZE_JSON_SERIALIZER_H

#include "JsonSerializable.h"
#include <string>

namespace gogtron
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