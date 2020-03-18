#include "Tools/SerializationTools.h"
#include "Tools/EngineLaunchTools.h"

FString SerializationTools::SerializationConf(const FLaunchConf& SaveConfig)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("Engine"), SaveConfig.Engine);
	JsonObject->SetStringField(TEXT("Project"), SaveConfig.Project);
	JsonObject->SetStringField(TEXT("Tool"), SaveConfig.Tool);
	JsonObject->SetStringField(TEXT("ToolPreArgs"), SaveConfig.ToolPreArgs);

	TArray<TSharedPtr<FJsonValue>> ParamsJonsObjList;
	for (const auto& ParamItem : SaveConfig.Params)
	{
		ParamsJonsObjList.Add(MakeShareable(new FJsonValueString(ParamItem)));
	}

	JsonObject->SetArrayField(TEXT("Params"), ParamsJonsObjList);
	FString JsonStr;
	auto JsonWriter = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	return JsonStr;
}
FLaunchConf SerializationTools::DeSerializationConf(const FString& jsonConf)
{
	FLaunchConf loadConf;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonConf);
	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		loadConf.Engine = JsonObject->GetStringField(TEXT("Engine"));
		loadConf.Tool = JsonObject->GetStringField(TEXT("Tool"));
		loadConf.ToolPreArgs = JsonObject->GetStringField(TEXT("ToolPreArgs"));
		loadConf.Project = JsonObject->GetStringField(TEXT("Project"));
		TArray<TSharedPtr<FJsonValue>> LaunchParamsData = JsonObject->GetArrayField("Params");
		for (const auto& DataItem : LaunchParamsData)
		{
			loadConf.Params.Add(DataItem->AsString());
		}
	}
	return loadConf;
}

FString SerializationTools::SerializeToolsAsString(const TArray<FToolInfo>& InTools)
{
	FString resultStr;

	auto JsonWriter = TJsonWriterFactory<>::Create(&resultStr);

	TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> ToolsJsonValueObjectList;
	for (const auto& Tool : InTools)
	{
		ToolsJsonValueObjectList.Add(MakeShareable(new FJsonValueObject(SerializationTools::SerializeToolInfoAsJsonObj(Tool))));
	}
	RootJsonObject->SetArrayField(TEXT("LaunchTools"), ToolsJsonValueObjectList);

	FJsonSerializer::Serialize(RootJsonObject.ToSharedRef(), JsonWriter);
	return resultStr;
}

TArray<FToolInfo> SerializationTools::DeSerializeToolsByString(const FString& InToolsStr)
{
	TArray<FToolInfo> result;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(InToolsStr);
	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		TArray<TSharedPtr<FJsonValue>> ToolsJsonObjectList =  JsonObject->GetArrayField(TEXT("LaunchTools"));

		for (const auto& ToolJsonObj : ToolsJsonObjectList)
		{
			result.Add(SerializationTools::DeSerializeToolInfo(ToolJsonObj->AsObject()));
		}
	}
	return result;
}

TSharedPtr<FJsonObject> SerializationTools::SerializeToolInfoAsJsonObj(const FToolInfo& InToolInfo)
{
	TSharedPtr<FJsonObject> ToolInfoJsonObj = MakeShareable(new FJsonObject);

	ToolInfoJsonObj->SetStringField(TEXT("ToolName"), InToolInfo.ToolName);
	ToolInfoJsonObj->SetStringField(TEXT("PreArgs"), InToolInfo.PreArgs);
	ToolInfoJsonObj->SetStringField(TEXT("BinPath"), InToolInfo.BinPath);

	return ToolInfoJsonObj;
}

FToolInfo SerializationTools::DeSerializeToolInfo(const TSharedPtr<FJsonObject>& InJsonObject)
{
	FToolInfo ToolInfo;
	ToolInfo.ToolName = InJsonObject->GetStringField(TEXT("ToolName"));
	ToolInfo.PreArgs = InJsonObject->GetStringField(TEXT("PreArgs"));
	ToolInfo.BinPath = InJsonObject->GetStringField(TEXT("BinPath"));
	return ToolInfo;
}
