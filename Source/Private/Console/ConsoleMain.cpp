
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RealExecutionMain.h"
#include "Tools/EngineLaunchTools.h"
#include "Tools/StringTools.h"

int main(int argc,char *argv[])
{
	return RealExecutionMain(*StringTools::Conv_CharArrayToFString(argc, argv));
}
