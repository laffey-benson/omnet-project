# OMNeT++ 项目开发备忘录

## ⚠️ 重要规则

> **任何 OMNeT++ 项目首次运行时，必须使用 Qtenv 图形界面！**

## 原因

1. **可视化调试** - 图形界面可以直观看到消息在网络中的传播过程
2. **即时发现问题** - 能够立即发现拓扑连接、消息路由等逻辑错误
3. **参数调整** - 可以实时修改参数并观察效果
4. **学习理解** - 帮助理解仿真系统的运行机制

## 运行命令

### ✅ 推荐：图形界面（Qtenv）

```bash
cd /path/to/project
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/<ProjectName> -u Qtenv -f simulations/omnetpp.ini -n src:simulations
```

### ❌ 不推荐：命令行模式（Cmdenv）

```bash
./out/clang-release/<ProjectName> -u Cmdenv -f simulations/omnetpp.ini -n src:simulations
```

命令行模式仅适用于：
- 批量运行自动化测试
- 超长时间仿真
- 无图形界面的服务器环境

## Qtenv 使用指南

### 控制按钮

| 按钮 | 快捷键 | 功能 |
|------|--------|------|
| ▶ | F5 | 连续运行 |
| ▶▶ | F6 | 单步执行 |
| ▶▶▶ | F8 | 快进（可调速度） |
| ■ | F6 | 暂停 |
| ⟲ | F9 | 重置仿真 |

### 常用操作

1. **查看节点信息** - 右键点击节点 → Inspect → This Module
2. **查看统计数据** - 菜单 → Simulation → Statistics
3. **调整显示速度** - 菜单 → Simulation → Set Simulation Speed
4. **查看消息内容** - 点击消息气泡查看详情

## 项目结构模板

```
project/
├── src/
│   ├── package.ned           # 包声明
│   ├── *.ned                 # 模块和网络定义
│   ├── *.cc                  # C++ 实现
│   └── *.msg                 # 消息定义
├── simulations/
│   ├── omnetpp.ini           # 仿真配置
│   └── results/              # 结果输出目录
├── out/
│   └── clang-release/        # 编译输出
├── Makefile
└── README.md
```

## 调试建议

1. **首次运行用 Qtenv** - 确保拓扑正确、消息传递正常
2. **设置合理仿真时间** - 先用短时间（如 10s）快速验证
3. **观察消息流向** - 确认路由逻辑正确
4. **检查统计数据** - 确认统计记录完整
5. **命令行模式** - 仅在验证无误后用于批量测试

## 快速启动脚本

在项目根目录创建 `run.sh`：

```bash
#!/bin/bash
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/<ProjectName> -u Qtenv -f simulations/omnetpp.ini -n src:simulations
```

使用时：

```bash
chmod +x run.sh
./run.sh
```

---

*创建时间：2026-04-09*
