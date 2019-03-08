#pragma once

#include "UnrealString.h"
#include "Json.h"

class SerializationTools
{
public:
	static FString SerializationConf(const FLaunchConf& SaveConfig);
	static FLaunchConf DeSerializationConf(const FString& jsonConf);
};