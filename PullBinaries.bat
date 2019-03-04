@echo off
md UE4Launcher\Engine\Binaries\Win64
md UE4Launcher\Engine\Content\Internationalization\icudt53l
md UE4Launcher\Engine\Content\Slate 
md UE4Launcher\Engine\Shaders\StandaloneRenderer
copy ..\..\..\Binaries\Win64\UE4Launcher.exe UE4Launcher\Engine\Binaries\Win64
xcopy /y/i/s/e ..\..\..\Content\Internationalization\English\icudt53l UE4Launcher\Engine\Content\Internationalization\icudt53l
xcopy /y/i/s/e ..\..\..\Content\Slate UE4Launcher\Engine\Content\Slate
xcopy /y/i/s/e ..\..\..\Shaders\StandaloneRenderer UE4Launcher\Engine\Shaders\StandaloneRenderer
echo "start .\Engine\Binaries\Win64\UE4Launcher.exe">UE4Launcher\UE4Launcher.bat

