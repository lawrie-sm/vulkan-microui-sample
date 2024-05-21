@echo off

set ShaderDir="..\src\shaders"
set CompileFlags="--target-env=vulkan1.3"

pushd bin
glslc %CompileFlags% -fshader-stage=vertex -o ui_vert.spv %ShaderDir%\ui_vert.glsl
glslc %CompileFlags% -fshader-stage=fragment -o ui_frag.spv %ShaderDir%\ui_frag.glsl
popd

