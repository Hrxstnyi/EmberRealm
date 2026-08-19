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
    pause
    exit /b 1
)

echo [1/4] UE5.8 路径: C:\Epic Games\虚幻引擎5\UE_5.8
set "UE_PATH=C:\Epic Games\虚幻引擎5\UE_5.8"
echo.

set "UBT=%UE_PATH%\Engine\Build\BatchFiles\Build.bat"

if not exist "%UBT%" (
    echo [错误] 找不到 Build.bat
    pause
    exit /b 1
)

echo [2/4] 生成 Visual Studio 项目文件...
"%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%UPROJECT%" -game -rocket -progress
echo.

echo [3/4] 编译项目（Development Editor Win64，禁用UBA）...
echo       这可能需要 5-15 分钟，请耐心等待...
echo.

call "%UBT%" EmberRealmEditor Win64 Development -project="%UPROJECT%" -WaitMutex -FromMsBuild -NoUBA

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
