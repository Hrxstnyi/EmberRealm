#!/bin/bash
# 烬域：卡拉比亚战区 - 项目构建脚本
# 用法: ./Scripts/build.sh [Development|Shipping] [Win64]

set -e

PROJECT_NAME="EmberRealm"
BUILD_TYPE=${1:-Development}
PLATFORM=${2:-Win64}
UE_ENGINE_DIR=${UE_ENGINE_DIR:-""}

echo "========================================"
echo "  烬域：卡拉比亚战区 - 构建脚本"
echo "  项目: ${PROJECT_NAME}"
echo "  配置: ${BUILD_TYPE} | ${PLATFORM}"
echo "========================================"

# 定位UE引擎
if [ -z "$UE_ENGINE_DIR" ]; then
    # 常见安装路径
    for path in "/c/Program Files/Epic Games/UE_5.3" "/c/Program Files/Epic Games/UE_5.4" "/Applications/Epic Games/UE_5.3"; do
        if [ -d "$path" ]; then
            UE_ENGINE_DIR="$path"
            break
        fi
    done
fi

if [ -z "$UE_ENGINE_DIR" ]; then
    echo "[错误] 未找到Unreal Engine安装目录"
    echo "请设置环境变量 UE_ENGINE_DIR 指向引擎根目录"
    exit 1
fi

echo "[信息] 使用引擎: ${UE_ENGINE_DIR}"

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
UPROJECT="${PROJECT_DIR}/${PROJECT_NAME}.uproject"

if [ ! -f "$UPROJECT" ]; then
    echo "[错误] 未找到项目文件: ${UPROJECT}"
    exit 1
fi

# 生成项目文件
echo "[步骤1] 生成Visual Studio项目文件..."
"${UE_ENGINE_DIR}/Engine/Build/BatchFiles/Build.bat" \
    -projectfiles -project="$UPROJECT" -game -rocket -progress

# 编译编辑器
echo "[步骤2] 编译编辑器目标..."
"${UE_ENGINE_DIR}/Engine/Build/BatchFiles/Build.bat" \
    "${PROJECT_NAME}Editor" ${PLATFORM} ${BUILD_TYPE} \
    -project="$UPROJECT" -WaitMutex -FromMsBuild

echo "[完成] 构建成功！可在UE5编辑器中打开 ${UPROJECT}"
