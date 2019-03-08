#include "Tools/SerializationTools.h"

FString SerializationTools::SerializationConf(const FLaunchConf& SaveConfig)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("Engine"), SaveConfig.Engine);
	JsonWriter->WriteValue(TEXT("Project"), SaveConfig.Project);
	JsonWriter->WriteValue(TEXT("Tool"), SaveConfig.Tool);
	JsonWriter->WriteValue(TEXT("ToolPreArgs"), SaveConfig.ToolPreArgs);
	JsonWriter->WriteArrayStart("Params");
	for (const auto& ParamItem : SaveConfig.Params)
	{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue(TEXT("ParamItem"), ParamItem);
		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
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
			loadConf.Params.Add(DataItem->AsObject()->GetStringField(TEXT("ParamItem")));
		}
	}
	return loadConf;
}