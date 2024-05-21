@echo off

echo --- Shaders
call build_shaders.bat

echo --- Program
set CompilerFlags=/std:c17 /Fe:microui_sample.exe /nologo /FC /GR- /D_HAS_EXCEPTIONS=0 /fsanitize=address /Zi
set WarningFlags=/Wall /wd5045
set IncludePaths=/external:I"..\external\include"  /external:W0
set LinkerFlags=/link /LIBPATH:"..\external\lib\SDL2" /LIBPATH:"..\external\lib\vulkan" /SUBSYSTEM:WINDOWS /DEBUG
set Libraries=SDL2.lib SDL2main.lib shell32.lib vulkan-1.lib

pushd bin
cl "..\src\all.c" %CompilerFlags% %WarningFlags% %IncludePaths% %LinkerFlags% %Libraries%
popd
