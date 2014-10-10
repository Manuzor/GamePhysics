@echo off

python3 "%GP_Tool_CopyDependencies% " "%~1 " "%GP_Config% " "%GP_Architecture% " "%CD%\Dependencies.py" -f
