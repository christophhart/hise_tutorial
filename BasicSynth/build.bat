@echo off

if "%1%"=="--help" (
echo HISE Build Script Windows
echo ================================================================================
echo build [--help] [--noaax]
echo.
echo This script can be used to build a project using the command line tool of HISE.
echo Options:
echo --help: shows this info
echo --noaax: deactivates AAX export. Use this for projects that can't use AAX format
echo.  
echo Requirements:
echo 1. You need to compile HISE and set the PATH environment variable so that HISE
        can be executed via HISE.exe
echo 2. You need to have the HISE SDK set correctly: `HISE set_hise_folder -p:PATH`
echo 3. You need to have the following tools installed: git, InnoSetup
echo.
echo Requirements for building AAX
echo If you want to build the AAX version, you need the Pace wraptool 4 and have
echo two additional batch files, aaxconfig.bat and aaxsign.bat in the project root
echo.
echo ================================================================================

goto :end
)

set hise_path=HISE.exe

REM set this to 0 to skip the installer
set buildInstaller=0
set buildAAX=0

set plugin_name=BasicSynth
set plugin_project_path=XmlPresetBackups/BasicSynth.xml

set installer_command="C:\Program Files (x86)\Inno Setup 5\ISCC.exe"

if "%1%"=="--noaax" set buildAAX=0

:VariableCheck
echo Checking Environment variables
REM  ====================================================================================

if %buildAAX%==1 (
  echo Building AAX plugins is enabled.
  echo Checking AAX Configuration before compiling
  call aaxconfig.bat
) 

%hise_path% --help > dummy.log

if %errorlevel% NEQ 0 (
   echo ERROR: Can't find HISE.exe
   del dummy.log
   exit /b 1
)

del dummy.log

if "%plugin_project_path%"=="" (
   echo ERROR: The path to the HISE project file is not set `plugin_project_path`
   exit /b 1
)

if "%plugin_name%"=="" (
   echo ERROR: The path to the HISE project file is not set `plugin_name`
   exit /b 1
)

REM Don't check the installer if not required...
if "%buildInstaller%"=="0" (
  goto :BuildProject
)

if not defined installer_command (
   echo ERROR: The path to the Innosetup tool is not set: `installer_command`
   exit /b 1
)


:BuildProject
echo Building Binaries...
REM  ====================================================================================


echo Setting project folder
%hise_path% set_project_folder "-p:%~dp0"

%hise_path% clean --all

if %buildAAX%==1 (
  echo Exporting %plugin_name% AAX Plugins
  %hise_path% clean 
  %hise_path% export_ci %plugin_project_path% -ipp -t:instrument -p:AAX -a:x86x64
  call Binaries/batchCompile.bat
)

echo Exporting %plugin_name% Standalone
%hise_path% clean 
%hise_path% export_ci %plugin_project_path% -ipp -t:standalone -a:x86x64
call Binaries/batchCompile.bat

echo Exporting %plugin_name% Plugins
%hise_path% clean
%hise_path% export_ci %plugin_project_path% -ipp -t:instrument -p:VST -a:x86x64
call Binaries/batchCompile.bat

:CopyFiles
echo Copying files
REM  ====================================================================================

md build

xcopy /E /Y "Binaries\Compiled\*.*" build\

del /S "build\*.lib"
del /S "build\*.exp"

if %buildAAX%==0 (
  goto :BuildInstaller
)

REM Handle AAX Studio plugin

xcopy "build\AAX\%plugin_name% x86.aaxplugin\Contents\Win32\%plugin_name% x86.aaxplugin" "build\AAX\%plugin_name% x64.aaxplugin\Contents\Win32\"
ren "build\AAX\%plugin_name% x64.aaxplugin\Contents\Win32\%plugin_name% x86.aaxplugin" "%plugin_name%.aaxplugin"
ren "build\AAX\%plugin_name% x64.aaxplugin\Contents\x64\%plugin_name% x64.aaxplugin" "%plugin_name%.aaxplugin"
ren "build\AAX\%plugin_name% x64.aaxplugin" "%plugin_name%.aaxplugin"
rmdir /Q /S "build\AAX\%plugin_name% x86.aaxplugin"

:SignAAX
echo Signing AAX plugins
REM  ====================================================================================

call aaxsign.bat

:BuildInstaller
echo Building installer
REM  ====================================================================================

if %buildInstaller%==0 (
   echo Skipping Installer
   goto :EOF
)

if %buildAAX%==0 (
  %hise_path% create-win-installer -noaax
) else (
  %hise_path% create-win-installer
)

%installer_command% WinInstaller.iss

:end