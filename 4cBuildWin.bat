xcopy .\assets build\assets /s /e /y /q

pushd build

SET buildConfig=%1
IF "%1"=="" (
	SET buildConfig="Release"
)


cmake --build . --config %buildConfig%
if %ERRORLEVEL% EQU 0 bin\ShaderFinal.exe

popd