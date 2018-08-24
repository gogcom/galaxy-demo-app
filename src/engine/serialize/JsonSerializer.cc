#include "JsonSerializer.h"

using namespace galaxy::demo::serialize;

bool JsonSerializer::Serialize(JsonSerializable* object, std::string& output)
{
	if (object == nullptr)
		return false;

	Json::Value root;
	object->Serialize(root);
	Json::StyledWriter writer;
	output = writer.write(root);
	return true;
}

bool JsonSerializer::Deserialize(JsonSerializable* object, const std::string& input)
{
	if (object == nullptr)
		return false;

	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(input, root))
		return false;

	return object->Deserialize(root);
}