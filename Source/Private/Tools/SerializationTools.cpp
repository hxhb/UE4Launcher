#include "Tools/SerializationTools.h"

FString SerializationTools::SerializationConf(const FUELaunchConf& SaveConfig)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("LaunchEngine"), SaveConfig.LaunchEngine);
	JsonWriter->WriteValue(TEXT("LaunchPlatfrom"), SaveConfig.LaunchPlatfrom);
	JsonWriter->WriteValue(TEXT("LaunchProject"), SaveConfig.LaunchProject);
	JsonWriter->WriteValue(TEXT("bUseCmdEngine"), SaveConfig.bUseCmdEngine);
	JsonWriter->WriteArrayStart("LaunchParams");
	for (const auto& ParamItem : SaveConfig.LaunchParams)
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
FUELaunchConf SerializationTools::DeSerializationConf(const FString& jsonConf)
{
	FUELaunchConf loadConf;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonConf);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		loadConf.LaunchEngine = JsonObject->GetStringField(TEXT("LaunchEngine"));
		loadConf.LaunchPlatfrom = JsonObject->GetStringField(TEXT("LaunchPlatfrom"));
		loadConf.LaunchProject = JsonObject->GetStringField(TEXT("LaunchProject"));
		loadConf.bUseCmdEngine = JsonObject->GetBoolField(TEXT("bUseCmdEngine"));
		TArray<TSharedPtr<FJsonValue>> LaunchParamsData = JsonObject->GetArrayField("LaunchParams");
		for (const auto& DataItem : LaunchParamsData)
		{
			loadConf.LaunchParams.Add(DataItem->AsObject()->GetStringField(TEXT("ParamItem")));
		}
	}
	return loadConf;
}