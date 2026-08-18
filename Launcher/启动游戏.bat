@echo off
chcp 65001 >nul
title 烬域：卡拉比亚战区 - 启动器

echo ========================================
echo   烬域：卡拉比亚战区 启动器
echo   EmberRealm: Calabia War Zone
echo ========================================
echo.

REM 检查Python是否安装
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未检测到 Python，请先安装 Python 3.8+
    echo 下载地址: https://www.python.org/downloads/
    echo.
    pause
    exit /b 1
)

REM 切换到启动器目录
cd /d "%~dp0"

REM 启动启动器
echo 正在启动...
python launcher.py

if %errorlevel% neq 0 (
    echo.
    echo [错误] 启动器运行失败
    pause
)
