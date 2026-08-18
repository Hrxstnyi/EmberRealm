# 烬域启动器 (EmberRealm Launcher)

跨平台游戏启动器，基于 Python + Tkinter，零第三方依赖。

## 功能

- 三大阵营选择（戍卫军 / 合约团 / 自治同盟），实时显示阵营特性
- 指挥官代号设置
- 分辨率、画质、全屏、音量调节
- 游戏路径配置（支持已打包的exe直接启动）
- 开发模式：自动定位 `.uproject` 项目文件，提示用UE5编辑器打开
- 配置持久化（保存到 `~/.emberrealm/launcher_config.json`）

## 运行

```bash
# 直接运行（需要Python 3.8+）
python launcher.py
```

## 打包为独立可执行文件（可选）

```bash
pip install pyinstaller
pyinstaller --onefile --windowed --name "EmberRealmLauncher" launcher.py
```

打包后可执行文件在 `dist/` 目录下。

## 配置文件

启动器配置保存在：
- Windows: `%USERPROFILE%\.emberrealm\launcher_config.json`
- macOS/Linux: `~/.emberrealm/launcher_config.json`
