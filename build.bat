@echo off
echo Building Voxel Engine...
cl /EHsc main.cpp Mesh.cpp Shader.cpp /I "C:\SDL2\include" /I "C:\GLEW\include" /I "C:\Kybus Engine\glm" /link /LIBPATH:"C:\SDL2\lib\x86" /LIBPATH:"C:\GLEW\lib\Release\Win32" SDL2.lib SDL2main.lib shell32.lib glew32.lib opengl32.lib /SUBSYSTEM:CONSOLE
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
) else (
    echo Build succeeded!
    copy "C:\GLEW\bin\Release\Win32\glew32.dll" .
    copy "C:\SDL2\lib\x86\SDL2.dll" .
    echo DLLs copied!
    pause
)