@echo off
rd /s /q "%~dp0bin"
rd /s /q "%~dp0local"
rd /s /q "%~dp0temp"
rd /s /q "%~dp0dependencies"
rd /s /q "%~dp0.vs
del "%~dp0game.sln"