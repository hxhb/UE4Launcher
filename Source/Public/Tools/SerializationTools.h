#pragma once

#include "UnrealString.h"
#include "Json.h"

struct FToolInfo;
class SerializationTools
{
public:
	static FString SerializationConf(const FLaunchConf& SaveConfig);
	static FLaunchConf DeSerializationConf(const FString& jsonConf);

	static FString SerializeToolsAsString(const TArray<struct FToolInfo>& InTools);
	static TArray<struct FToolInfo> DeSerializeToolsByString(const FString& InToolsStr);

	static TSharedPtr<FJsonObject> SerializeToolInfoAsJsonObj(const struct FToolInfo& InToolInfo);
	static struct FToolInfo DeSerializeToolInfo(const TSharedPtr<FJsonObject>& InJsonObject);
};