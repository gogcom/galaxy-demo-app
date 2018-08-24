#ifndef GALAXY_DEMO_SERIALIZE_JSON_SERIALIZABLE_H
#define GALAXY_DEMO_SERIALIZE_JSON_SERIALIZABLE_H

#include <json/json.h>

namespace galaxy::demo
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