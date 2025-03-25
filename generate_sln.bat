mkdir bin\
copy premake\settings\* bin\
call "premake/premake5" --file=source/game/premake5.lua vs2022
pause
