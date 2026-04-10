# SmartNet 智能网络仿真系统说明

## 目录

- [系统概述](#系统概述)
- [与 MultiNodeNet 的功能对比](#与-multinodenet-的功能对比)
- [文件结构](#文件结构)
- [模块功能详解](#模块功能详解)
- [配置参数说明](#配置参数说明)
- [运行方式](#运行方式)
- [输出结果](#输出结果)

---

## 系统概述

**项目名称：** SmartNet
**项目类型：** OMNeT++ 智能网络仿真系统
**功能描述：** 6节点网格+环形混合拓扑网络，支持优先级队列、拥塞控制、跳数限制和多路径路由
**仿真时长：** 默认 100 秒
**源码位置：** `/home/sfm/文档/omnet-project/smartnet/`

### 新增功能（相比 MultiNodeNet）

| 功能 | MultiNodeNet | SmartNet |
|------|--------------|----------|
| 节点数量 | 4个 | **6个** |
| 网络拓扑 | 简单环形 | **网格+环形混合拓扑** |
| 消息优先级 | 无 | **三级优先级（高/中/低）** |
| 拥塞控制 | 简单丢包率 | **动态队列监测+智能丢包** |
| 跳数限制 | 无 | **防止消息无限循环** |
| 消息确认 | 无 | **ACK确认机制** |
| 队列缓存 | 无 | **优先级消息队列** |
| 链路状态 | 无 | **动态链路状态表** |

---

## 与 MultiNodeNet 的功能对比

### 1. 拓扑结构

**MultiNodeNet：** 4节点简单环形

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

**SmartNet：** 6节点网格+环形混合拓扑

```
        node[1] -------- node[2]
           |                |
           |                |
        node[0] -------- node[3]
           |                |
           |                |
        node[5] -------- node[4]
```

关键改进：
- 增加更多节点形成网格拓扑
- 支持侧向连接（节点1-5, 节点2-4）
- 支持跨越连接（节点0-2, 节点1-3, 节点5-3）
- 提供更多路由选择路径

### 2. 消息格式

**MultiNodeNet：** 基础网络消息

```cpp
NetMessage {
    int messageId;           // 消息唯一标识
    int sourceNode;          // 源节点编号
    int destNode;            // 目的节点编号
    simtime_t creationTime; // 创建时间
    string payload;          // 消息内容
}
```

**SmartNet：** 扩展智能消息

```cpp
SmartMessage {
    int messageId = 0;        // 消息唯一标识
    int sourceNode = -1;      // 源节点编号
    int destNode = -1;        // 目的节点编号
    int priority = 1;         // 优先级：0=低, 1=中, 2=高
    int messageType = 0;      // 消息类型：0=数据, 1=ACK, 2=路由更新
    simtime_t creationTime;   // 创建时间
    int hopCount = 0;         // 跳数计数
    int maxHops = 10;         // 最大跳数限制
    bool requireAck = false;  // 是否需要确认
    string payload = "";     // 消息内容
}
```

### 3. 优先级队列机制

**MultiNodeNet：** 无优先级

**SmartNet：** 基于优先级的消息队列调度

```
优先级队列处理流程：

1. 消息到达 → 根据优先级入队
   - 高优先级(2): 入队优先
   - 中优先级(1): 入队次之
   - 低优先级(0): 入队最后

2. 队列调度原则：
   - 高优先级消息总是先处理
   - 同优先级按到达时间（FIFO）处理
   - 队列满时，低优先级消息先被丢弃

3. 丢包策略（拥塞时）：
   - 高优先级：保持低丢包率（正常×0.1）
   - 中优先级：正常丢包率
   - 低优先级：提高丢包率（正常×1.5）
```

### 4. 拥塞控制

**MultiNodeNet：** 固定丢包率（5%）

**SmartNet：** 动态拥塞控制

```cpp
拥塞检测与响应：

1. 拥塞条件：
   - 队列长度 >= 拥塞阈值(默认15)

2. 拥塞响应：
   - 提高丢包率到50%
   - 低优先级消息丢包率更高
   - 高优先级消息受保护

3. 拥塞解除：
   - 队列长度 < 拥塞阈值/2 时解除
```

### 5. 跳数限制机制

**MultiNodeNet：** 无跳数限制

**SmartNet：** 防止消息无限循环

```cpp
跳数限制逻辑：

1. 每条消息有最大跳数限制（默认10跳）
2. 每经过一个节点，hopCount + 1
3. 当 hopCount >= maxHops 时，消息被丢弃
4. 目的节点不计跳数
```

### 6. 动态路由

**MultiNodeNet：** 简单环形方向选择

**SmartNet：** 基于链路状态的动态路由

```cpp
动态路由算法：

1. 链路状态表维护：
   - 每个节点维护邻居链路状态
   - 记录链路可靠性和活跃状态

2. 路由选择策略：
   - 计算经过各邻居到目的节点的距离
   - 选择使目的节点更近的邻居
   - 考虑链路可靠性作为权重

3. 备用路由：
   - 当最优路径不可用时，选择次优路径
   - 确保网络鲁棒性
```

---

## 文件结构

```
smartnet/
├── src/
│   ├── package.ned           # 包声明文件
│   ├── SmartNode.ned         # 节点模块定义
│   ├── SmartNode.cc          # 节点实现（C++ 代码）
│   ├── SmartNet.ned          # 网络拓扑定义
│   ├── messages.msg          # 自定义消息定义
│   ├── messages_m.cc         # 消息自动生成代码
│   └── messages_m.h          # 消息自动生成头文件
├── simulations/
│   ├── omnetpp.ini           # 仿真配置文件
│   └── results/              # 结果输出目录
├── out/
│   └── clang-release/
│       └── SmartNet          # 编译后的可执行文件
└── Makefile                  # 编译配置
```

---

## 模块功能详解

### 1. SmartMessage 消息（`messages.msg`）

结构化智能网络消息，包含以下字段：

| 字段 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `messageId` | int | 0 | 消息唯一标识 |
| `sourceNode` | int | -1 | 源节点编号 |
| `destNode` | int | -1 | 目的节点编号 |
| `priority` | int | 1 | 优先级（0低/1中/2高） |
| `messageType` | int | 0 | 消息类型（0数据/1ACK/2路由） |
| `creationTime` | simtime_t | - | 创建时间 |
| `hopCount` | int | 0 | 当前跳数 |
| `maxHops` | int | 10 | 最大跳数限制 |
| `requireAck` | bool | false | 是否需要确认 |
| `payload` | string | "" | 消息内容 |

### 2. SmartNode 模块（`SmartNode.ned` + `SmartNode.cc`）

智能网络中的通信实体。

#### 参数定义

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `nodeId` | int | 0 | 节点唯一标识（0-5） |
| `dropRate` | double | 0.03 | 基础丢包率 |
| `sendInterval` | double | 1.0 | 消息发送间隔 |
| `numNodes` | int | 6 | 网络节点总数 |
| `queueSize` | int | 20 | 消息队列大小 |
| `congestionThreshold` | int | 15 | 拥塞阈值 |
| `congestionDropRate` | double | 0.5 | 拥塞时丢包率 |
| `maxHops` | int | 10 | 最大跳数 |
| `enablePriority` | bool | true | 启用优先级调度 |
| `enableCongestionControl` | bool | true | 启用拥塞控制 |

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
- 初始化统计计数器和链路状态表
- 建立与所有邻居的连接
- 节点0作为源节点，发送第一批消息

##### 2.2 优先级队��� (`enqueueMessage()` / `dequeueMessage()`)

- 根据消息优先级入队
- 高优先级消息优先出队
- 同优先级按到达时间FIFO处理

##### 2.3 消息处理 (`processReceivedMessage()`)

- 判断消息类型（数据/ACK/路由）
- 检查是否是目的节点
- 处理跳数限制
- 决定是否转发或丢弃

##### 2.4 动态路由 (`selectNextHop()`)

- 遍历所有可用邻居
- 计算经过各邻居到目的节点的距离
- 选择最优下一跳（考虑可靠性和距离）

##### 2.5 拥塞控制 (`checkCongestion()`)

- 监测队列长度
- 超过阈值触发拥塞控制
- 低于阈值一半解除拥塞

##### 2.6 智能丢包 (`shouldDropPacket()`)

- 根据拥塞状态调整丢包率
- 高优先级消息受保护
- 低优先级消息更易被丢弃

##### 2.7 确认机制 (`sendAck()`)

- 目的节点收到需要确认的消息时发送ACK
- ACK使用高优先级
- 沿原路径返回

##### 2.8 结束统计 (`finish()`)

输出并记录以下标量：
- 发送消息数
- 接收消息数
- 成功投递数
- 转发消息数
- 丢包数
- 拥塞事件数
- 平均延迟
- 投递成功率

---

## 配置参数说明

配置文件位置：`simulations/omnetpp.ini`

```ini
[General]
network = SmartNet
description = "SmartNet 智能网络仿真系统"
sim-time-limit = 100s

# 节点参数配置
**.node[*].nodeId = index
**.node[*].numNodes = 6
**.node[*].dropRate = 0.03
**.node[*].sendInterval = 1.0
**.node[*].queueSize = 20
**.node[*].congestionThreshold = 15
**.node[*].congestionDropRate = 0.5
**.node[*].maxHops = 10
**.node[*].enablePriority = true
**.node[*].enableCongestionControl = true

# 连接延迟配置
**.conn[*].delay = 50ms
```

### 场景配置

| 配置名 | 说明 | 参数调整 |
|--------|------|----------|
| Default | 默认配置，所有功能启用 | 标准参数 |
| NoPriority | 禁用优先级调度 | enablePriority=false |
| NoCongestion | 禁用拥塞控制 | enableCongestionControl=false |
| HighLoad | 高负载场景 | sendInterval=0.5s |
| Reliable | 高可靠性配置 | dropRate=0.01 |
| Stress | 压力测试 | sendInterval=0.2s |

---

## 运行方式

### 编译

```bash
cd /home/sfm/文档/omnet-project/smartnet
make -j4
```

### 命令行模式（Cmdenv）

```bash
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/SmartNet -u Cmdenv -f simulations/omnetpp.ini -n src:simulations
```

### 图形模式（Qtenv）

```bash
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/SmartNet -u Qtenv -f simulations/omnetpp.ini -n src:simulations
```

> **注意：** 在图形模式下，可以直观看到消息在网格网络中的传播过程，支持优先级调度可视化。

---

## 输出结果

### 运行日志示例

```
===========================================
节点 0 初始化完成
  丢包率: 3%
  发送间隔: 1s
  队列大小: 20
  拥塞阈值: 15
  启用优先级: 是
  启用拥塞控制: 是
  邻居节点: 5 4 2 3
===========================================
[节点0] 作为源节点，发送第一批消息
[节点0] 发送消息 #1 -> 目的节点 3 [优先级:HIGH]
    [入队] 队列大小: 1/20
    路由中... 选择下一跳: 节点 3
[节点0] 发送消息 #2 -> 目的节点 2 [优先级:MEDIUM]
    [入队] 队列大小: 1/20
    路由中... 选择下一跳: 节点 2
[节点0] 发送消息 #3 -> 目的节点 5 [优先级:LOW]
    [入队] 队列大小: 1/20
    路由中... 选择下一跳: 节点 4
...
[节点3] 收到消息 #1 (源:0 -> 目的:3, 跳数:1, 延迟:0.05s)
    >>> 消息已到达目的节点！
===========================================
节点 0 统计报告
===========================================
发送消息数: 100
接收消息数: 250
成功投递数: 85
转发消息数: 165
丢包数: 8
拥塞事件: 3

平均延迟: 0.15s
投递成功率: 85%
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
run SmartNet-Default-0-20260410-...
config network SmartNet

scalar SmartNet.node[0] 发送消息数 100
scalar SmartNet.node[0] 成功投递数 85
scalar SmartNet.node[0] 平均延迟 0.15s
scalar SmartNet.node[0] 投递成功率 85
scalar SmartNet.node[0] 拥塞事件数 3
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

当前 SmartNet 已实现以下功能，可进一步扩展：

1. **能量模型：** 添加节点能耗统计，支持绿色网络仿真
2. **移动性：** 支持节点位置变化（移动自组织网络）
3. **多播路由：** 支持组播消息传递
4. **入侵检测：** 添加异常流量检测和告警
5. **QoS保障：** 实现端到端的服务质量保证
6. **网络编码：** 支持网络编码技术提高吞吐量

---

*文档生成时间：2026-04-10*
