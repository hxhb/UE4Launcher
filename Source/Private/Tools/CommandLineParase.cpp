#include "Tools/CommandLineParase.h"

TMap<FString,FString> CommandLineParase::GetCommandLineParamsMap(const FString& CommandLine)
{
	TMap<FString,FString> resault;
	TArray<FString> ParamsSwitchs,ParamsTokens;
	FCommandLine::Parse(*CommandLine, ParamsTokens, ParamsSwitchs);

	for(const auto& SwitchItem:ParamsSwitchs)
	{
		if (!SwitchItem.IsEmpty() && ParamsTokens.Num() && ParamsTokens.Find(SwitchItem) != INDEX_NONE)
		{
			FString ItemToken(TEXT(""));
			for (int32 index = ParamsTokens.Find(SwitchItem)+1;
				index < ParamsTokens.Num() && ParamsSwitchs.Find(ParamsTokens[index]) == INDEX_NONE;
				++index)
			{
				ItemToken.Append(ParamsTokens[index]).Append(TEXT(" "));
			}
			if (!ItemToken.IsEmpty())
			{
				// remove all end point space
				while (std::isspace(ItemToken[ItemToken.Len() - 1]))
					ItemToken.RemoveAt(ItemToken.Len() - 1);
			}
			resault.Add(SwitchItem,ItemToken);
		}
	}

	return resault;
}