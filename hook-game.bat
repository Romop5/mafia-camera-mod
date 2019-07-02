copy "build\bin\CameraMod.dll" "C:\Program Files (x86)\Cenega Czech\Mafia\CameraMod.dll"

::Run camera injector inside Mafia directory
cd "C:\Program Files (x86)\Cenega Czech\Mafia"
camera_injector.exe

echo "Done"
