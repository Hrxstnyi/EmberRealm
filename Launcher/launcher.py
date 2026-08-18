#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
烬域：卡拉比亚战区 - 游戏启动器
EmberRealm Game Launcher
跨平台启动器，支持阵营选择、画质设置、游戏启动
"""

import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import json
import os
import sys
import subprocess
import platform
from pathlib import Path

# ============================================================
# 配置管理
# ============================================================
CONFIG_FILE = Path.home() / ".emberrealm" / "launcher_config.json"

DEFAULT_CONFIG = {
    "faction": "garrison",
    "resolution": "1920x1080",
    "fullscreen": True,
    "quality": "high",
    "volume": 80,
    "game_path": "",
    "player_name": "指挥官"
}

FACTIONS = {
    "garrison": {
        "name": "戍卫军",
        "color": "#3B6FD4",
        "desc": "正规军 · 阵地防守 · 后勤强势 · 稳扎稳打",
        "bonus": "防御+20%，税收+15%"
    },
    "contract": {
        "name": "合约团",
        "color": "#D4A03B",
        "desc": "雇佣武装 · 高速机动 · 突袭战术 · 外交博弈",
        "bonus": "移动力+1，突袭成功率+25%"
    },
    "autonomy": {
        "name": "自治同盟",
        "color": "#3BC46B",
        "desc": "地方武装 · 地形游击 · 隐蔽作战 · 持续消耗",
        "bonus": "山地移动无惩罚，隐蔽+30%"
    }
}

RESOLUTIONS = ["1280x720", "1920x1080", "2560x1440", "3840x2160"]
QUALITIES = ["low", "medium", "high", "epic"]


def load_config():
    try:
        if CONFIG_FILE.exists():
            with open(CONFIG_FILE, "r", encoding="utf-8") as f:
                cfg = json.load(f)
                merged = DEFAULT_CONFIG.copy()
                merged.update(cfg)
                return merged
    except Exception:
        pass
    return DEFAULT_CONFIG.copy()


def save_config(config):
    try:
        CONFIG_FILE.parent.mkdir(parents=True, exist_ok=True)
        with open(CONFIG_FILE, "w", encoding="utf-8") as f:
            json.dump(config, f, ensure_ascii=False, indent=2)
    except Exception as e:
        print(f"[Launcher] 保存配置失败: {e}")


# ============================================================
# 启动器主界面
# ============================================================
class EmberLauncher:
    def __init__(self, root):
        self.root = root
        self.config = load_config()
        self.selected_faction = tk.StringVar(value=self.config["faction"])
        self.resolution_var = tk.StringVar(value=self.config["resolution"])
        self.fullscreen_var = tk.BooleanVar(value=self.config["fullscreen"])
        self.quality_var = tk.StringVar(value=self.config["quality"])
        self.volume_var = tk.IntVar(value=self.config["volume"])
        self.name_var = tk.StringVar(value=self.config["player_name"])

        self.setup_window()
        self.build_ui()

    def setup_window(self):
        self.root.title("烬域：卡拉比亚战区 - 启动器")
        self.root.geometry("960x640")
        self.root.minsize(860, 580)
        self.root.configure(bg="#0D0F14")

        # 居中显示
        self.root.update_idletasks()
        w = self.root.winfo_width()
        h = self.root.winfo_height()
        x = (self.root.winfo_screenwidth() - w) // 2
        y = (self.root.winfo_screenheight() - h) // 2
        self.root.geometry(f"+{x}+{y}")

    def build_ui(self):
        # 顶部标题栏
        header = tk.Frame(self.root, bg="#14171F", height=80)
        header.pack(fill="x", side="top")
        header.pack_propagate(False)

        tk.Label(
            header, text="烬 域",
            font=("Microsoft YaHei", 28, "bold"),
            fg="#E8B84B", bg="#14171F"
        ).pack(side="left", padx=30, pady=15)

        tk.Label(
            header, text="卡拉比亚战区\nEMBERREALM",
            font=("Microsoft YaHei", 11),
            fg="#8892A4", bg="#14171F", justify="left"
        ).pack(side="left", pady=15)

        tk.Label(
            header, text="v0.1.0  Prototype",
            font=("Consolas", 9),
            fg="#555E6E", bg="#14171F"
        ).pack(side="right", padx=20)

        # 主内容区
        content = tk.Frame(self.root, bg="#0D0F14")
        content.pack(fill="both", expand=True, padx=20, pady=15)

        # 左侧：阵营选择
        left = tk.Frame(content, bg="#0D0F14", width=420)
        left.pack(side="left", fill="both", expand=True)
        left.pack_propagate(False)

        tk.Label(
            left, text="选择阵营",
            font=("Microsoft YaHei", 14, "bold"),
            fg="#E0E4EC", bg="#0D0F14", anchor="w"
        ).pack(fill="x", pady=(0, 10))

        self.faction_buttons = {}
        for key, info in FACTIONS.items():
            self._create_faction_card(left, key, info)

        # 阵营详情
        self.faction_detail = tk.Frame(left, bg="#161A23", bd=1, relief="solid")
        self.faction_detail.pack(fill="x", pady=10)
        self.detail_name = tk.Label(
            self.faction_detail, text="",
            font=("Microsoft YaHei", 13, "bold"),
            fg="#E8B84B", bg="#161A23", anchor="w"
        )
        self.detail_name.pack(fill="x", padx=12, pady=(10, 2))
        self.detail_desc = tk.Label(
            self.faction_detail, text="",
            font=("Microsoft YaHei", 9),
            fg="#A0A8B8", bg="#161A23", anchor="w", justify="left", wraplength=380
        )
        self.detail_desc.pack(fill="x", padx=12)
        self.detail_bonus = tk.Label(
            self.faction_detail, text="",
            font=("Microsoft YaHei", 9, "bold"),
            fg="#5BC98A", bg="#161A23", anchor="w"
        )
        self.detail_bonus.pack(fill="x", padx=12, pady=(2, 10))
        self._update_faction_detail()

        # 玩家名
        name_frame = tk.Frame(left, bg="#0D0F14")
        name_frame.pack(fill="x", pady=(5, 0))
        tk.Label(name_frame, text="指挥官代号：", font=("Microsoft YaHei", 10),
                 fg="#A0A8B8", bg="#0D0F14").pack(side="left")
        name_entry = tk.Entry(
            name_frame, textvariable=self.name_var,
            font=("Microsoft YaHei", 10), bg="#1A1E28", fg="#E0E4EC",
            insertbackground="#E0E4EC", relief="flat", width=20
        )
        name_entry.pack(side="left", padx=5, ipady=4)

        # 右侧：设置
        right = tk.Frame(content, bg="#0D0F14", width=420)
        right.pack(side="right", fill="both", expand=True, padx=(20, 0))
        right.pack_propagate(False)

        tk.Label(
            right, text="游戏设置",
            font=("Microsoft YaHei", 14, "bold"),
            fg="#E0E4EC", bg="#0D0F14", anchor="w"
        ).pack(fill="x", pady=(0, 10))

        settings_box = tk.Frame(right, bg="#161A23", bd=1, relief="solid")
        settings_box.pack(fill="both", expand=True)

        # 分辨率
        self._add_setting_row(settings_box, "分辨率",
            ttk.Combobox(settings_box, textvariable=self.resolution_var,
                         values=RESOLUTIONS, state="readonly", width=18))

        # 画质
        quality_combo = ttk.Combobox(settings_box, textvariable=self.quality_var,
                                     values=QUALITIES, state="readonly", width=18)
        self._add_setting_row(settings_box, "画质预设", quality_combo)

        # 全屏
        fs_frame = tk.Frame(settings_box, bg="#161A23")
        self._add_setting_row(settings_box, "全屏模式",
            tk.Checkbutton(fs_frame, variable=self.fullscreen_var, bg="#161A23",
                           activebackground="#161A23", fg="#E0E4EC", selectcolor="#1A1E28",
                           text="启用", font=("Microsoft YaHei", 10)))
        fs_frame = None  # 已在_add_setting_row中创建

        # 音量
        vol_frame = tk.Frame(settings_box, bg="#161A23")
        vol_scale = tk.Scale(
            vol_frame, from_=0, to=100, orient="horizontal",
            variable=self.volume_var, bg="#161A23", fg="#A0A8B8",
            troughcolor="#1A1E28", highlightthickness=0, length=180
        )
        vol_scale.pack(side="left")
        self._add_setting_row(settings_box, "主音量", vol_frame)

        # 游戏路径
        path_frame = tk.Frame(settings_box, bg="#161A23")
        self.path_label = tk.Label(
            path_frame, text=self.config["game_path"] or "未设置（使用编辑器模式）",
            font=("Consolas", 8), fg="#6B7384", bg="#161A23", anchor="w",
            wraplength=250, justify="left"
        )
        self.path_label.pack(side="left", fill="x", expand=True)
        tk.Button(
            path_frame, text="浏览", command=self._browse_game_path,
            font=("Microsoft YaHei", 9), bg="#252A36", fg="#E0E4EC",
            relief="flat", padx=10, cursor="hand2", activebackground="#303644"
        ).pack(side="right", padx=(5, 0))
        self._add_setting_row(settings_box, "游戏路径", path_frame)

        # 底部按钮区
        footer = tk.Frame(self.root, bg="#0D0F14", height=70)
        footer.pack(fill="x", side="bottom", padx=20, pady=(0, 15))
        footer.pack_propagate(False)

        tk.Button(
            footer, text="退出", command=self.root.quit,
            font=("Microsoft YaHei", 11), bg="#1A1E28", fg="#A0A8B8",
            relief="flat", padx=30, pady=8, cursor="hand2", activebackground="#252A36"
        ).pack(side="right", padx=(10, 0))

        tk.Button(
            footer, text="保存设置", command=self._save_settings,
            font=("Microsoft YaHei", 11), bg="#1A1E28", fg="#E0E4EC",
            relief="flat", padx=20, pady=8, cursor="hand2", activebackground="#252A36"
        ).pack(side="right")

        self.start_btn = tk.Button(
            footer, text="▶  开始游戏", command=self._start_game,
            font=("Microsoft YaHei", 13, "bold"), bg="#C8922E", fg="#0D0F14",
            relief="flat", padx=40, pady=10, cursor="hand2", activebackground="#E8B84B"
        )
        self.start_btn.pack(side="right", padx=(0, 10))

        # 底部状态
        self.status_var = tk.StringVar(value="就绪")
        tk.Label(
            footer, textvariable=self.status_var,
            font=("Consolas", 9), fg="#555E6E", bg="#0D0F14"
        ).pack(side="left", padx=10)

    def _create_faction_card(self, parent, key, info):
        frame = tk.Frame(parent, bg="#161A23", bd=1, relief="solid", height=72)
        frame.pack(fill="x", pady=4)
        frame.pack_propagate(False)

        # 颜色条
        color_bar = tk.Frame(frame, bg=info["color"], width=4)
        color_bar.pack(side="left", fill="y")

        # 单选按钮
        rb = tk.Radiobutton(
            frame, variable=self.selected_faction, value=key,
            command=self._update_faction_detail,
            bg="#161A23", activebackground="#161A23",
            selectcolor="#1A1E28", fg="#E0E4EC"
        )
        rb.pack(side="left", padx=8)
        self.faction_buttons[key] = rb

        # 文字
        text_frame = tk.Frame(frame, bg="#161A23")
        text_frame.pack(side="left", fill="both", expand=True, pady=8)
        tk.Label(
            text_frame, text=info["name"],
            font=("Microsoft YaHei", 12, "bold"),
            fg=info["color"], bg="#161A23", anchor="w"
        ).pack(fill="x")
        tk.Label(
            text_frame, text=info["desc"],
            font=("Microsoft YaHei", 8),
            fg="#7A8294", bg="#161A23", anchor="w"
        ).pack(fill="x")

    def _add_setting_row(self, parent, label, widget):
        row = tk.Frame(parent, bg="#161A23")
        row.pack(fill="x", padx=15, pady=8)
        tk.Label(
            row, text=label, font=("Microsoft YaHei", 10),
            fg="#A0A8B8", bg="#161A23", width=10, anchor="w"
        ).pack(side="left")
        widget.pack(side="left", padx=(10, 0))

    def _update_faction_detail(self):
        key = self.selected_faction.get()
        info = FACTIONS[key]
        self.detail_name.config(text=info["name"], fg=info["color"])
        self.detail_desc.config(text=info["desc"])
        self.detail_bonus.config(text=f"阵营特性：{info['bonus']}")

    def _browse_game_path(self):
        path = filedialog.askopenfilename(
            title="选择游戏可执行文件",
            filetypes=[("可执行文件", "*.exe"), ("所有文件", "*.*")]
        )
        if path:
            self.config["game_path"] = path
            self.path_label.config(text=path)

    def _save_settings(self):
        self.config["faction"] = self.selected_faction.get()
        self.config["resolution"] = self.resolution_var.get()
        self.config["fullscreen"] = self.fullscreen_var.get()
        self.config["quality"] = self.quality_var.get()
        self.config["volume"] = self.volume_var.get()
        self.config["player_name"] = self.name_var.get()
        save_config(self.config)
        self.status_var.set("设置已保存")
        self.root.after(2000, lambda: self.status_var.set("就绪"))

    def _start_game(self):
        self._save_settings()
        faction = FACTIONS[self.selected_faction.get()]["name"]
        self.status_var.set(f"正在启动游戏 - 阵营：{faction}...")

        game_path = self.config.get("game_path", "")

        # 尝试查找UE5可执行文件
        if game_path and os.path.exists(game_path):
            try:
                args = [game_path]
                if self.fullscreen_var.get():
                    args.append("-fullscreen")
                else:
                    args.append("-windowed")
                args.extend(["-ResX", self.resolution_var.get().split("x")[0]])
                args.extend(["-ResY", self.resolution_var.get().split("x")[1]])
                subprocess.Popen(args)
                self.status_var.set("游戏已启动")
                self.root.after(1000, self.root.quit)
                return
            except Exception as e:
                messagebox.showerror("启动失败", f"无法启动游戏：{e}")
                self.status_var.set("启动失败")
                return

        # 开发模式：提示用UE5编辑器打开
        project_dir = Path(__file__).parent.parent
        uproject = project_dir / "EmberRealm.uproject"

        msg = (
            f"未找到游戏可执行文件。\n\n"
            f"当前为开发原型阶段，请通过以下方式启动：\n\n"
            f"1. 双击项目文件：{uproject}\n"
            f"2. 在UE5编辑器中点击 Play 按钮\n\n"
            f"已选择阵营：{faction}\n"
            f"指挥官：{self.name_var.get()}\n\n"
            f"是否打开项目所在目录？"
        )
        if messagebox.askyesno("开发模式启动", msg):
            self._open_project_dir(project_dir)

        self.status_var.set("就绪")

    def _open_project_dir(self, path):
        try:
            if platform.system() == "Windows":
                os.startfile(str(path))
            elif platform.system() == "Darwin":
                subprocess.Popen(["open", str(path)])
            else:
                subprocess.Popen(["xdg-open", str(path)])
        except Exception:
            pass


def main():
    root = tk.Tk()
    # 设置ttk主题
    style = ttk.Style()
    try:
        style.theme_use("clam")
    except Exception:
        pass

    app = EmberLauncher(root)
    root.mainloop()


if __name__ == "__main__":
    main()
