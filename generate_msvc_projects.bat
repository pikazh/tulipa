@echo off

@rem set the prefix of the sub dir to store msvc project files
set ProjDir=msvcprojs


set FormerDir=%cd%
cd /d %~dp0

if not exist %ProjDir% (
	md %ProjDir%
)
cd %ProjDir%
echo Generating MSVC projects witch CMake...
cmake .. -G "Visual Studio 15 2017" -A x64 

cd /d %FormerDir%