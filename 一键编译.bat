@echo off
chcp 65001 >nul
title 烬域 - 一键编译工具
echo ========================================
echo    烬域：卡拉比亚战区 - 一键编译
echo ========================================
echo.

set "PROJECT_DIR=%~dp0"
set "UPROJECT=%PROJECT_DIR%EmberRealm.uproject"

if not exist "%UPROJECT%" (
    echo [错误] 找不到 EmberRealm.uproject
    echo 请确保此脚本放在项目根目录下
    pause
    exit /b 1
)

echo [1/4] 查找 Unreal Engine 5 安装路径...

set "UE_PATH="
for /f "tokens=2*" %%a in ('reg query "HKLM\SOFTWARE\EpicGames\Unreal Engine" /s 2^>nul ^| findstr "InstalledDirectory"') do (
    set "UE_PATH=%%b"
)

if "%UE_PATH%"=="" (
    if exist "C:\Program Files\Epic Games\UE_5.8" set "UE_PATH=C:\Program Files\Epic Games\UE_5.8"
    if exist "C:\Program Files\Epic Games\UE_5.7" set "UE_PATH=C:\Program Files\Epic Games\UE_5.7"
    if exist "C:\Program Files\Epic Games\UE_5.6" set "UE_PATH=C:\Program Files\Epic Games\UE_5.6"
    if exist "C:\Program Files\Epic Games\UE_5.5" set "UE_PATH=C:\Program Files\Epic Games\UE_5.5"
)

if "%UE_PATH%"=="" (
    echo [错误] 找不到 Unreal Engine 安装路径
    echo 请确认已通过 Epic Games Launcher 安装 UE5
    pause
    exit /b 1
)

echo       找到: %UE_PATH%
echo.

set "UBT=%UE_PATH%\Engine\Build\BatchFiles\Build.bat"

if not exist "%UBT%" (
    echo [错误] 找不到 Build.bat: %UBT%
    pause
    exit /b 1
)

echo [2/4] 生成 Visual Studio 项目文件...
"%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%UPROJECT%" -game -rocket -progress
if %errorlevel% neq 0 (
    echo [警告] 项目文件生成可能有问题，继续尝试编译...
)
echo.

echo [3/4] 编译项目（Development Editor Win64）...
echo       这可能需要 3-10 分钟，请耐心等待...
echo.

call "%UBT%" EmberRealmEditor Win64 Development -project="%UPROJECT%" -WaitMutex -FromMsBuild

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo    [成功] 编译完成！
    echo ========================================
    echo.
    echo 现在可以双击 EmberRealm.uproject 打开编辑器了
    echo.
) else (
    echo.
    echo ========================================
    echo    [失败] 编译出错
    echo ========================================
    echo.
    echo 请把上面的红色错误信息截图发给开发者
    echo.
)

pause
