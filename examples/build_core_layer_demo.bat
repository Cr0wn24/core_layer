@echo off
set ft_root=..\external\freetype\

set disabled_warnings=-wd4201 -wd4100 -wd4152 -wd4189 -wd4101 -wd4456 -W4 -WX 
set common_defines=-D_CRT_SECURE_NO_WARNINGS
set common_compiler_flags=%common_defines% %disabled_warnings% -Zi -FC -Od -nologo

if not exist ..\build mkdir ..\build
pushd ..\build

if not exist freetype.lib (
	echo -- freetype.lib was not found. Building freetype --
	call ..\misc\build_freetype.bat ..\
)

set shader_path=..\res\shaders
set shader_output_path=..\renderer\backends\d3d11
fxc.exe /nologo /T vs_5_0 /E vs /O3 /WX /Zpc /Ges /Fh %shader_output_path%\d3d11_vshader.h /Vn d3d11_vshader /Qstrip_reflect /Qstrip_debug /Qstrip_priv %shader_path%\shader.hlsl
fxc.exe /nologo /T ps_5_0 /E ps /O3 /WX /Zpc /Ges /Fh %shader_output_path%\d3d11_pshader.h /Vn d3d11_pshader /Qstrip_reflect /Qstrip_debug /Qstrip_priv %shader_path%\shader.hlsl

echo -- Building core layer demo --
cl ..\examples\core_layer_demo.c -I%ft_root%include\ %common_compiler_flags% -DEBUG:FULL -link -incremental:no

del *.obj

popd