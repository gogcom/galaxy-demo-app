#ifndef GOGTRON_SERIALIZE_JSON_SERIALIZABLE_H
#define GOGTRON_SERIALIZE_JSON_SERIALIZABLE_H

#include <json/json.h>

namespace gogtron
{
	namespace serialize
	{

		class JsonSerializable
		{
		public:

			virtual ~JsonSerializable() = default;
			virtual bool Serialize(Json::Value& root) = 0;
			virtual bool Deserialize(Json::Value& root) = 0;
		};

	}
}

#endif