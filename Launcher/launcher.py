#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
烬域：卡拉比亚战区 - 游戏启动器
EmberRealm: Calabia War Zone - Game Launcher

功能：
- 启动游戏
- 管理存档
- 游戏设置
- 版本信息
"""

import os
import sys
import json
import subprocess
import tkinter as tk
from tkinter import ttk, messagebox, filedialog
from pathlib import Path
from datetime import datetime

# ============================================================
# 配置
# ============================================================

GAME_NAME = "烬域：卡拉比亚战区"
GAME_NAME_EN = "EmberRealm: Calabia War Zone"
VERSION = "0.1.0 (Phase 0 - Pre-Alpha)"
ENGINE = "Unreal Engine 5.5"

# 游戏可执行文件名（打包后）
GAME_EXE_NAMES = [
    "EmberRealm.exe",
    "EmberRealm-Win64-Shipping.exe",
    "EmberRealm-Win64-DebugGame.exe",
]

# 默认设置
DEFAULT_SETTINGS = {
    "resolution": "1920x1080",
    "fullscreen": True,
    "language": "zh-CN",
    "graphics_quality": "High",
    "volume_master": 80,
    "volume_music": 60,
    "volume_sfx": 90,
    "auto_save": True,
    "auto_save_interval": 5,
}

# 路径
BASE_DIR = Path(__file__).resolve().parent.parent
SAVE_DIR = BASE_DIR / "Saved" / "SaveGames"
SETTINGS_FILE = BASE_DIR / "Saved" / "Config" / "launcher_settings.json"
LOG_FILE = BASE_DIR / "Saved" / "Logs" / "launcher.log"


# ============================================================
# 工具函数
# ============================================================

def log(message: str):
    """记录日志"""
    LOG_FILE.parent.mkdir(parents=True, exist_ok=True)
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(f"[{timestamp}] {message}\n")
    print(f"[Launcher] {message}")


def load_settings() -> dict:
    """加载设置"""
    if SETTINGS_FILE.exists():
        try:
            with open(SETTINGS_FILE, "r", encoding="utf-8") as f:
                return {**DEFAULT_SETTINGS, **json.load(f)}
        except Exception as e:
            log(f"加载设置失败: {e}")
    return DEFAULT_SETTINGS.copy()


def save_settings(settings: dict):
    """保存设置"""
    SETTINGS_FILE.parent.mkdir(parents=True, exist_ok=True)
    with open(SETTINGS_FILE, "w", encoding="utf-8") as f:
        json.dump(settings, f, indent=2, ensure_ascii=False)
    log("设置已保存")


def find_game_exe() -> Path | None:
    """查找游戏可执行文件"""
    search_dirs = [
        BASE_DIR / "Binaries" / "Win64",
        BASE_DIR / "Binaries" / "Win32",
        BASE_DIR,
    ]
    for search_dir in search_dirs:
        if not search_dir.exists():
            continue
        for exe_name in GAME_EXE_NAMES:
            exe_path = search_dir / exe_name
            if exe_path.exists():
                return exe_path
    return None


def list_saves() -> list[dict]:
    """列出存档"""
    saves = []
    if not SAVE_DIR.exists():
        return saves
    for save_file in SAVE_DIR.glob("*.sav"):
        try:
            stat = save_file.stat()
            saves.append({
                "name": save_file.stem,
                "path": str(save_file),
                "size": stat.st_size,
                "modified": datetime.fromtimestamp(stat.st_mtime).strftime("%Y-%m-%d %H:%M:%S"),
            })
        except Exception:
            pass
    return sorted(saves, key=lambda x: x["modified"], reverse=True)


# ============================================================
# 启动器主界面
# ============================================================

class EmberRealmLauncher:
    def __init__(self, root: tk.Tk):
        self.root = root
        self.root.title(f"{GAME_NAME} - 启动器")
        self.root.geometry("720x520")
        self.root.minsize(640, 480)
        self.root.configure(bg="#1a1a2e")

        self.settings = load_settings()
        self.game_exe = find_game_exe()

        self._setup_style()
        self._build_ui()

        log("启动器初始化完成")

    def _setup_style(self):
        """设置样式"""
        style = ttk.Style()
        style.theme_use("clam")

        style.configure("TFrame", background="#1a1a2e")
        style.configure("TLabel", background="#1a1a2e", foreground="#e0e0e0", font=("Microsoft YaHei", 10))
        style.configure("Title.TLabel", background="#1a1a2e", foreground="#ff6b35", font=("Microsoft YaHei", 24, "bold"))
        style.configure("Subtitle.TLabel", background="#1a1a2e", foreground="#8888aa", font=("Microsoft YaHei", 11))
        style.configure("Version.TLabel", background="#1a1a2e", foreground="#666688", font=("Consolas", 9))
        style.configure("TButton", font=("Microsoft YaHei", 11), padding=10)
        style.configure("Play.TButton", font=("Microsoft YaHei", 16, "bold"), padding=15)
        style.configure("TNotebook", background="#1a1a2e", borderwidth=0)
        style.configure("TNotebook.Tab", background="#16213e", foreground="#aaaacc", padding=[15, 8], font=("Microsoft YaHei", 10))
        style.map("TNotebook.Tab", background=[("selected", "#0f3460")], foreground=[("selected", "#ffffff")])
        style.configure("Treeview", background="#16213e", foreground="#e0e0e0", fieldbackground="#16213e", borderwidth=0)
        style.configure("Treeview.Heading", background="#0f3460", foreground="#ffffff", font=("Microsoft YaHei", 10, "bold"))

    def _build_ui(self):
        """构建界面"""
        header = ttk.Frame(self.root, style="TFrame")
        header.pack(fill="x", padx=30, pady=(25, 10))

        ttk.Label(header, text=GAME_NAME, style="Title.TLabel").pack(anchor="w")
        ttk.Label(header, text=GAME_NAME_EN, style="Subtitle.TLabel").pack(anchor="w", pady=(2, 0))
        ttk.Label(header, text=f"v{VERSION} | {ENGINE}", style="Version.TLabel").pack(anchor="w", pady=(5, 0))

        separator = tk.Frame(self.root, height=1, bg="#0f3460")
        separator.pack(fill="x", padx=30, pady=10)

        notebook = ttk.Notebook(self.root)
        notebook.pack(fill="both", expand=True, padx=30, pady=(0, 15))

        self._build_home_tab(notebook)
        self._build_saves_tab(notebook)
        self._build_settings_tab(notebook)
        self._build_about_tab(notebook)

        status_bar = ttk.Frame(self.root, style="TFrame")
        status_bar.pack(fill="x", padx=30, pady=(0, 15))

        self.status_label = ttk.Label(status_bar, text="就绪", style="Version.TLabel")
        self.status_label.pack(side="left")

        exe_status = "游戏已就绪" if self.game_exe else "游戏未打包（开发模式）"
        ttk.Label(status_bar, text=exe_status, style="Version.TLabel").pack(side="right")

    def _build_home_tab(self, notebook):
        """主页标签"""
        frame = ttk.Frame(notebook, style="TFrame")
        notebook.add(frame, text="  主页  ")

        play_frame = ttk.Frame(frame, style="TFrame")
        play_frame.pack(expand=True)

        play_btn = ttk.Button(
            play_frame,
            text="▶  开始游戏",
            style="Play.TButton",
            command=self.launch_game
        )
        play_btn.pack(pady=20)

        info_frame = ttk.Frame(frame, style="TFrame")
        info_frame.pack(fill="x", padx=50, pady=20)

        if self.game_exe:
            ttk.Label(info_frame, text=f"游戏路径: {self.game_exe}", style="Version.TLabel").pack(anchor="w", pady=2)
        else:
            ttk.Label(info_frame, text="提示：游戏尚未打包。请在UE5编辑器中运行，或使用打包功能生成可执行文件。",
                      style="Version.TLabel", wraplength=550, justify="left").pack(anchor="w", pady=2)

        save_count = len(list_saves())
        ttk.Label(info_frame, text=f"存档数量: {save_count}", style="Version.TLabel").pack(anchor="w", pady=2)

        quick_frame = ttk.Frame(frame, style="TFrame")
        quick_frame.pack(fill="x", padx=50, pady=10)

        ttk.Button(quick_frame, text="打开存档目录", command=self.open_save_dir).pack(side="left", padx=(0, 10))
        ttk.Button(quick_frame, text="打开游戏目录", command=self.open_game_dir).pack(side="left")

    def _build_saves_tab(self, notebook):
        """存档管理标签"""
        frame = ttk.Frame(notebook, style="TFrame")
        notebook.add(frame, text="  存档管理  ")

        columns = ("name", "modified", "size")
        self.save_tree = ttk.Treeview(frame, columns=columns, show="headings", height=10)
        self.save_tree.heading("name", text="存档名称")
        self.save_tree.heading("modified", text="修改时间")
        self.save_tree.heading("size", text="大小")
        self.save_tree.column("name", width=250)
        self.save_tree.column("modified", width=180)
        self.save_tree.column("size", width=100, anchor="e")
        self.save_tree.pack(fill="both", expand=True, padx=20, pady=20)

        btn_frame = ttk.Frame(frame, style="TFrame")
        btn_frame.pack(fill="x", padx=20, pady=(0, 20))

        ttk.Button(btn_frame, text="刷新", command=self.refresh_saves).pack(side="left", padx=(0, 10))
        ttk.Button(btn_frame, text="删除选中存档", command=self.delete_save).pack(side="left", padx=(0, 10))
        ttk.Button(btn_frame, text="打开存档目录", command=self.open_save_dir).pack(side="left")

        self.refresh_saves()

    def _build_settings_tab(self, notebook):
        """设置标签"""
        frame = ttk.Frame(notebook, style="TFrame")
        notebook.add(frame, text="  设置  ")

        settings_frame = ttk.Frame(frame, style="TFrame")
        settings_frame.pack(fill="both", expand=True, padx=40, pady=30)

        ttk.Label(settings_frame, text="分辨率:").grid(row=0, column=0, sticky="w", pady=8)
        self.resolution_var = tk.StringVar(value=self.settings["resolution"])
        ttk.Combobox(settings_frame, textvariable=self.resolution_var,
                     values=["1280x720", "1920x1080", "2560x1440", "3840x2160"],
                     state="readonly", width=20).grid(row=0, column=1, sticky="w", pady=8, padx=20)

        ttk.Label(settings_frame, text="全屏模式:").grid(row=1, column=0, sticky="w", pady=8)
        self.fullscreen_var = tk.BooleanVar(value=self.settings["fullscreen"])
        ttk.Checkbutton(settings_frame, variable=self.fullscreen_var).grid(row=1, column=1, sticky="w", pady=8, padx=20)

        ttk.Label(settings_frame, text="画质:").grid(row=2, column=0, sticky="w", pady=8)
        self.quality_var = tk.StringVar(value=self.settings["graphics_quality"])
        ttk.Combobox(settings_frame, textvariable=self.quality_var,
                     values=["Low", "Medium", "High", "Epic", "Cinematic"],
                     state="readonly", width=20).grid(row=2, column=1, sticky="w", pady=8, padx=20)

        ttk.Label(settings_frame, text="语言:").grid(row=3, column=0, sticky="w", pady=8)
        self.language_var = tk.StringVar(value=self.settings["language"])
        ttk.Combobox(settings_frame, textvariable=self.language_var,
                     values=["zh-CN", "en-US", "ja-JP"],
                     state="readonly", width=20).grid(row=3, column=1, sticky="w", pady=8, padx=20)

        ttk.Label(settings_frame, text="主音量:").grid(row=4, column=0, sticky="w", pady=8)
        self.master_var = tk.IntVar(value=self.settings["volume_master"])
        ttk.Scale(settings_frame, from_=0, to=100, variable=self.master_var,
                  orient="horizontal", length=200).grid(row=4, column=1, sticky="w", pady=8, padx=20)

        ttk.Label(settings_frame, text="音乐音量:").grid(row=5, column=0, sticky="w", pady=8)
        self.music_var = tk.IntVar(value=self.settings["volume_music"])
        ttk.Scale(settings_frame, from_=0, to=100, variable=self.music_var,
                  orient="horizontal", length=200).grid(row=5, column=1, sticky="w", pady=8, padx=20)

        ttk.Label(settings_frame, text="音效音量:").grid(row=6, column=0, sticky="w", pady=8)
        self.sfx_var = tk.IntVar(value=self.settings["volume_sfx"])
        ttk.Scale(settings_frame, from_=0, to=100, variable=self.sfx_var,
                  orient="horizontal", length=200).grid(row=6, column=1, sticky="w", pady=8, padx=20)

        ttk.Label(settings_frame, text="自动存档:").grid(row=7, column=0, sticky="w", pady=8)
        self.autosave_var = tk.BooleanVar(value=self.settings["auto_save"])
        ttk.Checkbutton(settings_frame, variable=self.autosave_var).grid(row=7, column=1, sticky="w", pady=8, padx=20)

        ttk.Button(settings_frame, text="保存设置", command=self.save_current_settings).grid(
            row=8, column=0, columnspan=2, pady=20)

    def _build_about_tab(self, notebook):
        """关于标签"""
        frame = ttk.Frame(notebook, style="TFrame")
        notebook.add(frame, text="  关于  ")

        about_frame = ttk.Frame(frame, style="TFrame")
        about_frame.pack(expand=True, padx=40, pady=30)

        ttk.Label(about_frame, text=GAME_NAME, style="Title.TLabel").pack(pady=(0, 5))
        ttk.Label(about_frame, text=GAME_NAME_EN, style="Subtitle.TLabel").pack(pady=(0, 20))

        info_text = f"""
版本: {VERSION}
引擎: {ENGINE}
类型: 回合制4X策略 + FPS精英小队双视角

核心特色:
• 三阶时间流速系统（季度/周/四小时）
• 战略层与FPS层双向联动
• AI军官内鬼系统
• 塔科夫式硬核真实FPS
• 多元胜利路线

阵营:
• 戍卫军 - 正规军、阵地防守、后勤强势
• 合约团 - 雇佣武装、高速机动、突袭战术
• 自治同盟 - 地方武装、地形游击、隐蔽作战

在烬域之中，每一个决策都可能改变战争的走向。
        """
        ttk.Label(about_frame, text=info_text, style="Version.TLabel",
                  justify="left", wraplength=550).pack(pady=10)

    def launch_game(self):
        """启动游戏"""
        if not self.game_exe:
            messagebox.showinfo(
                "开发模式",
                "游戏尚未打包为可执行文件。\n\n"
                "请在 Unreal Engine 5 编辑器中打开项目：\n"
                f"{BASE_DIR / 'EmberRealm.uproject'}\n\n"
                "打包后可通过此启动器直接启动。"
            )
            self._set_status("已提示：开发模式")
            return

        try:
            self._set_status("正在启动游戏...")
            log(f"启动游戏: {self.game_exe}")

            args = [str(self.game_exe)]
            if self.settings["fullscreen"]:
                args.append("-fullscreen")
            else:
                args.append("-windowed")
            args.append(f"-ResX={self.settings['resolution'].split('x')[0]}")
            args.append(f"-ResY={self.settings['resolution'].split('x')[1]}")

            subprocess.Popen(args, cwd=str(BASE_DIR))
            self._set_status("游戏已启动")
            log("游戏启动成功")
            self.root.iconify()

        except Exception as e:
            log(f"启动游戏失败: {e}")
            messagebox.showerror("启动失败", f"无法启动游戏：\n{e}")
            self._set_status("启动失败")

    def refresh_saves(self):
        """刷新存档列表"""
        for item in self.save_tree.get_children():
            self.save_tree.delete(item)

        for save in list_saves():
            size_kb = save["size"] / 1024
            self.save_tree.insert("", "end", values=(
                save["name"],
                save["modified"],
                f"{size_kb:.1f} KB"
            ))

        self._set_status(f"已加载 {len(list_saves())} 个存档")

    def delete_save(self):
        """删除选中存档"""
        selected = self.save_tree.selection()
        if not selected:
            messagebox.showwarning("提示", "请先选择要删除的存档")
            return

        item = self.save_tree.item(selected[0])
        save_name = item["values"][0]

        if messagebox.askyesno("确认删除", f"确定要删除存档「{save_name}」吗？\n此操作不可撤销。"):
            save_path = SAVE_DIR / f"{save_name}.sav"
            if save_path.exists():
                save_path.unlink()
                log(f"删除存档: {save_name}")
                self.refresh_saves()
                self._set_status(f"已删除存档: {save_name}")

    def open_save_dir(self):
        """打开存档目录"""
        SAVE_DIR.mkdir(parents=True, exist_ok=True)
        self._open_path(SAVE_DIR)

    def open_game_dir(self):
        """打开游戏目录"""
        self._open_path(BASE_DIR)

    def _open_path(self, path: Path):
        """在文件管理器中打开路径"""
        try:
            if sys.platform == "win32":
                os.startfile(str(path))
            elif sys.platform == "darwin":
                subprocess.Popen(["open", str(path)])
            else:
                subprocess.Popen(["xdg-open", str(path)])
        except Exception as e:
            log(f"打开目录失败: {e}")

    def save_current_settings(self):
        """保存当前设置"""
        self.settings.update({
            "resolution": self.resolution_var.get(),
            "fullscreen": self.fullscreen_var.get(),
            "graphics_quality": self.quality_var.get(),
            "language": self.language_var.get(),
            "volume_master": self.master_var.get(),
            "volume_music": self.music_var.get(),
            "volume_sfx": self.sfx_var.get(),
            "auto_save": self.autosave_var.get(),
        })
        save_settings(self.settings)
        self._set_status("设置已保存")
        messagebox.showinfo("成功", "设置已保存")

    def _set_status(self, message: str):
        """设置状态栏"""
        self.status_label.config(text=message)
        log(message)


# ============================================================
# 入口
# ============================================================

def main():
    log("=" * 50)
    log(f"{GAME_NAME} 启动器启动")
    log(f"版本: {VERSION}")
    log(f"基础目录: {BASE_DIR}")

    root = tk.Tk()

    icon_path = BASE_DIR / "Launcher" / "assets" / "icon.ico"
    if icon_path.exists():
        try:
            root.iconbitmap(str(icon_path))
        except Exception:
            pass

    app = EmberRealmLauncher(root)
    root.mainloop()

    log("启动器关闭")


if __name__ == "__main__":
    main()
