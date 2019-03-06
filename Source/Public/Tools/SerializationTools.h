#pragma once

#include "UnrealString.h"
#include "Json.h"

class SerializationTools
{
public:
	static FString SerializationConf(const FUELaunchConf& SaveConfig);
	static FUELaunchConf DeSerializationConf(const FString& jsonConf);
};