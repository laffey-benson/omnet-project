# MultiNodeNet 多节点环形网络仿真系统说明

## 目录

- [系统概述](#系统概述)
- [与 TicToc 的功能对比](#与-tictoc-的功能对比)
- [文件结构](#文件结构)
- [模块功能详解](#模块功能详解)
- [配置参数说明](#配置参数说明)
- [运行方式](#运行方式)
- [输出结果](#输出结果)

---

## 系统概述

**项目名称：** MultiNodeNet
**项目类型：** OMNeT++ 功能增强版示例
**功能描述：** 4节点环形网络通信仿真，支持路由、丢包模拟和延迟统计
**仿真时长：** 默认 100 秒
**源码位置：** `/home/sfm/文档/omnet-project-1/multinodenet/`

### 新增功能（相比 TicToc）

| 功能 | TicToc | MultiNodeNet |
|------|--------|--------------|
| 节点数量 | 2个（tic, toc） | **4个（node[0-3]）** |
| 网络拓扑 | 点对点连接 | **环形拓扑** |
| 消息类型 | 简单字符串 | **自定义结构化消息** |
| 网络特性 | 无 | **随机丢包模拟** |
| 延迟统计 | 无 | **端到端延迟统计** |

---

## 与 TicToc 的功能对比

### 1. 拓扑结构

**TicToc：**
```
tic <-----------> toc
```
两点线性连接，自动切换消息名称。

**MultiNodeNet：**
```
        node[1]
           ↑
           │
    ←────── ───────→
node[0]           node[2]
    ←────── ───────→
           ↓
        node[3]
```
4节点环形拓扑，根据目的节点智能路由。

### 2. 消息格式

**TicToc：** 字符串 "tic" 或 "toc"
**MultiNodeNet：** 结构化消息

```cpp
NetMessage {
    int messageId;           // 消息唯一标识
    int sourceNode;          // 源节点编号
    int destNode;            // 目的节点编号
    simtime_t creationTime; // 创建时间（用于计算延迟）
    string payload;         // 消息内容
}
```

### 3. 丢包模拟

**TicToc：** 无丢包功能
**MultiNodeNet：** 可配置丢包率（默认 5%）

```cpp
bool NodeApp::shouldDropPacket()
{
    double rand = uniform(0, 1);
    return rand < dropRate;  // 5% 概率丢包
}
```

### 4. 延迟统计

**TicToc：** 仅统计消息数量
**MultiNodeNet：** 端到端延迟统计

```cpp
simtime_t delay = simTime() - msg->getCreationTime();
totalDelay += delay;
delayCount++;
```

---

## 文件结构

```
multinodenet/
├── src/
│   ├── package.ned           # 包声明文件
│   ├── Node.ned              # 节点模块定义
│   ├── NodeApp.cc            # 节点实现（C++ 代码）
│   ├── messages.msg          # 自定义消息定义
│   ├── MultiNodeNet.ned      # 网络拓扑定义
│   ├── messages_m.cc         # 消息自动生成代码
│   └── messages_m.h          # 消息自动生成头文件
├── simulations/
│   └── omnetpp.ini           # 仿真配置文件
├── out/
│   └── clang-release/
│       └── MultiNodeNet      # 编译后的可执行文件
└── Makefile                  # 编译配置
```

---

## 模块功能详解

### 1. NetMessage 消息（`messages.msg`）

结构化网络消息，包含以下字段：

| 字段 | 类型 | 说明 |
|------|------|------|
| `messageId` | int | 消息唯一标识 |
| `sourceNode` | int | 源节点编号（0-3） |
| `destNode` | int | 目的节点编号（0-3） |
| `creationTime` | simtime_t | 创建时间（用于延迟计算） |
| `payload` | string | 消息内容 |

### 2. NodeApp 模块（`Node.ned` + `NodeApp.cc`）

节点是环形网络中的通信实体。

#### 参数定义

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `nodeId` | int | 0 | 节点唯一标识（0-3） |
| `dropRate` | double | 0.05 | 丢包率（0-1） |
| `sendInterval` | double | 1.0 | 消息发送间隔（秒） |
| `numNodes` | int | 4 | 网络中节点总数 |

#### 端口定义

| 端口名 | 类型 | 方向 | 说明 |
|--------|------|------|------|
| `inLeft` | input | 输入 | 左邻居输入 |
| `outLeft` | output | 输出 | 左邻居输出 |
| `inRight` | input | 输入 | 右邻居输入 |
| `outRight` | output | 输出 | 右邻居输出 |

#### 核心功能

##### 2.1 初始化功能 (`initialize()`)

- 读取节点参数
- 初始化统计计数器
- **节点0特殊行为：**
  - 发送第一条消息
  - 启动定时器周期性发送

##### 2.2 消息处理功能 (`handleMessage(cMessage *msg)`)

- 判断消息类型（定时器/网络消息）
- 定时器到期：发送消息并重新调度
- 网络消息：调用 `processReceivedMessage()`

##### 2.3 消息路由功能 (`getNextNode(int dest)`)

根据目的节点选择最优方向：

```
目的在右半圆(diff > 0 && diff <= 2) → 向右转发
否则 → 向左转发
```

##### 2.4 丢包模拟功能 (`shouldDropPacket()`)

使用均匀分布随机数判断是否丢包。

##### 2.5 延迟统计功能 (`processReceivedMessage()`)

- 目的节点：计算 `simTime() - creationTime`
- 记录延迟到统计系统

##### 2.6 结束统计功能 (`finish()`)

输出并记录以下标量：
- 发送消息数
- 接收消息数
- 丢包数
- 平均延迟

---

## 配置参数说明

配置文件位置：`simulations/omnetpp.ini`

```ini
[General]
network = MultiNodeNet
description = "多节点环形网络仿真"
sim-time-limit = 100s

# 节点参数配置
**.node[*].nodeId = index
**.node[*].numNodes = 4
**.node[*].dropRate = 0.05
**.node[*].sendInterval = 1.0

# 连接延迟配置（所有链路统一100ms延迟）
**.conn[*].delay = 100ms

# Cmdenv输出配置
**.cmdenv-log-level = INFO

# 结果输出配置
**.scalar-recording = true
**.vector-recording = true

# 随机数种子（确保可重复实验）
seed-set = 42
```

### 参数说明

| 参数 | 值 | 说明 |
|------|-----|------|
| `network` | MultiNodeNet | 指定要仿真的网络 |
| `sim-time-limit` | 100s | 仿真时间限制 |
| `**.node[*].dropRate` | 0.05 | 5% 丢包率 |
| `**.node[*].sendInterval` | 1.0 | 每秒发送一条消息 |
| `**.conn[*].delay` | 100ms | 链路固定延迟 |
| `seed-set` | 42 | 随机数种子 |

---

## 运行方式

### 编译

```bash
cd /home/sfm/文档/omnet-project-1/multinodenet
make -j4
```

### 命令行模式（Cmdenv）

```bash
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/MultiNodeNet -u Cmdenv -f simulations/omnetpp.ini -n src:simulations
```

### 图形模式（Qtenv）

```bash
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/MultiNodeNet -u Qtenv -f simulations/omnetpp.ini -n src:simulations
```

> **注意：** 在图形模式下，可以直观看到消息在环形网络中传播的过程。

---

## 输出结果

### 运行日志示例

```
===========================================
节点 0 初始化完成
  丢包率: 5%
  发送间隔: 1s
  邻居节点: 3 (左), 1 (右)
===========================================
[节点0] 作为源节点，发送第一条消息
[节点0] 发送消息 #1 -> 目的节点 2
[节点1] 收到消息 #1 (源:0 -> 目的:2, 延迟:0.1s)
    路由中... 目的节点是 2 -> 转发到节点 2
[节点2] 收到消息 #1 (源:0 -> 目的:2, 延迟:0.2s)
    >>> 消息已到达目的节点！
...
===========================================
节点 0 统计报告
===========================================
发送消息数: 100
接收消息数: 25
丢包数: 2
平均延迟: 0.25s
===========================================
```

### 结果文件

仿真结果保存在 `simulations/results/` 目录：

```
simulations/results/
├── General-#0.sca    # 标量结果
├── General-#0.vec    # 向量结果
└── General-#0.anf    # 分析文件
```

### 标量结果示例

```
run General-0-20260409-18:08:27-26475
config network MultiNodeNet
config sim-time-limit 100s

scalar MultiNodeNet.node[0] 发送消息数 100
scalar MultiNodeNet.node[0] 接收消息数 25
scalar MultiNodeNet.node[0] 丢包数 3
scalar MultiNodeNet.node[0] 平均延迟 0.25s

scalar MultiNodeNet.node[1] 发送消息数 0
scalar MultiNodeNet.node[1] 接收消息数 30
...
```

---

## 技术栈

- **仿真框架：** OMNeT++ 6.2.0
- **编程语言：** C++（模块实现）、NED（拓扑描述）、MSG（消息定义）
- **编译器：** Clang
- **用户界面：** Cmdenv（命令行）、Qtenv（图形）

---

## 扩展方向

当前 MultiNodeNet 已实现以下功能，可进一步扩展：

1. **更多节点：** 将 4 节点扩展到 N 节点
2. **动态路由：** 实现更复杂的路由协议（如距离矢量）
3. **多路径路由：** 支持多条路径转发
4. **拥塞控制：** 根据队列长度决定丢包
5. **能量模型：** 添加节点能耗统计
6. **移动性：** 支持节点位置变化

---

*文档生成时间：2026-04-09*
