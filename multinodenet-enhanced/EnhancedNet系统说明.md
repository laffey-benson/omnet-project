# EnhancedNet 增强型多节点网络仿真系统说明

## 目录

- [系统概述](#系统概述)
- [与 MultiNodeNet 的功能对比](#与-multinodenet-的功能对比)
- [新增功能详解](#新增功能详解)
- [文件结构](#文件结构)
- [模块功能详解](#模块功能详解)
- [配置参数说明](#配置参数说明)
- [运行方式](#运行方式)
- [输出结果](#输出结果)

---

## 系统概述

**项目名称：** EnhancedNet
**项目类型：** OMNeT++ 增强版网络仿真系统
**功能描述：** 6节点环形网络通信仿真，支持ACK重传、拥塞控制、动态路由表和链路监测
**仿真时长：** 默认 200 秒
**源码位置：** `/home/sfm/文档/omnet-project/multinodenet-enhanced/`

---

## 与 MultiNodeNet 的功能对比

| 功能 | MultiNodeNet | EnhancedNet |
|------|--------------|-------------|
| 节点数量 | 4个 | **6个** |
| 网络拓扑 | 4节点环形 | **6节点环形** |
| 消息类型 | 基础结构化消息 | **增强型消息（含TTL、跳数、重传标志）** |
| 丢包模拟 | 随机丢包 | **随机丢包** |
| 延迟统计 | 端到端延迟 | **端到端延迟** |
| **ACK确认** | 无 | **新增：可靠性传输保障** |
| **重传机制** | 无 | **新增：超时自动重传** |
| **拥塞控制** | 无 | **新增：滑动窗口流控** |
| **路由表维护** | 简单固定路由 | **新增：动态路由更新** |
| **链路监测** | 无 | **新增：心跳机制** |

### 新增的4个核心功能

1. **ACK确认与重传机制（Reliability）**
   - 目的节点收到消息后发送ACK
   - 源节点等待ACK，超时自动重传
   - 防止消息丢失导致传输失败

2. **拥塞控制（Congestion Control）**
   - 基于滑动窗口的流量控制
   - 动态调整发送速率
   - 避免网络过载

3. **动态路由表（Routing Table）**
   - 节点维护路由表，记录可达路径
   - 周期性地广播路由更新
   - 支持备用路由切换

4. **链路监测（Link Monitoring）**
   - 心跳消息检测邻居存活状态
   - 自动检测链路故障
   - 故障时启用备用路由

---

## 新增功能详解

### 1. ACK确认与重传机制

#### 工作流程

```
源节点(0)                        目的节点(3)
    |                                  |
    |--- EnhancedNetMsg(seq=100) ------>|
    |                                  |
    |<---- AckMessage(seq=100) --------|
    |                                  |
```

#### 核心参数

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `maxRetransmit` | 3 | 最大重传次数 |
| `ackTimeout` | 4.0s | ACK超时时间 |

#### 实现机制

- **消息缓冲**：源节点维护待确认消息列表
- **超时检测**：定期检查未收到ACK的消息
- **重传计数**：每条消息最多重传N次
- **窗口控制**：限制飞行中的消息数量

### 2. 拥塞控制

#### 工作原理

采用类似TCP的滑动窗口机制：

```
发送窗口：[ Msg1 ] [ Msg2 ] [ Msg3 ] [ Msg4 ] [ Msg5 ]
           ↓      ↓      ↓
          已确认   已确认   等待中...
                  ↑窗口左边界
                              ↑窗口右边界
```

#### 核心参数

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `windowSize` | 5 | 最大窗口大小 |
| `enableCongestionControl` | true | 是否启用 |

#### 窗口调整策略

- **收到ACK**：窗口+1（慢启动）
- **超时**：窗口-1（拥塞避免）
- **上限**：不超过windowSize

### 3. 动态路由表

#### 路由表结构

```cpp
struct RouteEntry {
    int destNode;        // 目的节点
    int nextHop;         // 下一跳
    int metric;          // 度量值（跳数）
    simtime_t lastUpdate; // 更新时间
    int sequence;        // 序列号
};
```

#### 路由更新机制

- **周期性广播**：每15秒广播一次路由更新
- **距离矢量**：每个节点维护到所有其他节点的路由
- **序列号**：防止接收过时的路由信息

### 4. 链路监测

#### 心跳机制

```
节点0                           节点1
    |                               |
    |--- HeartbeatMsg(alive=true) ->|
    |                               |
    |--- HeartbeatMsg(alive=true) ->|
    |                               |
```

#### 故障检测

- **心跳间隔**：每5秒发送一次心跳
- **超时阈值**：3倍心跳间隔未响应认为失效
- **状态维护**：维护存活邻居集合

---

## 文件结构

```
multinodenet-enhanced/
├── src/
│   ├── package.ned           # 包声明文件
│   ├── Node.ned             # 节点模块定义（增强版）
│   ├── NodeApp.cc           # 节点实现（C++ 代码）
│   ├── messages.msg         # 自定义消息定义（新增多种消息）
│   ├── EnhancedNet.ned      # 网络拓扑定义（6节点）
│   ├── messages_m.cc         # 消息自动生成代码
│   └── messages_m.h          # 消息自动生成头文件
├── simulations/
│   ├── omnetpp.ini           # 仿真配置文件（多场景）
│   └── results/              # 结果输出目录
├── out/
│   └── clang-release/
│       └── EnhancedNet       # 编译后的可执行文件
├── Makefile                  # 编译配置
├── run.sh                    # Qtenv图形启动脚本
└── run_cmdenv.sh            # Cmdenv命令行启动脚本
```

---

## 消息类型详解

### EnhancedNetMsg（增强型数据消息）

```cpp
message EnhancedNetMsg {
    int messageId = 0;        // 消息唯一标识
    int sourceNode = -1;      // 源节点编号
    int destNode = -1;        // 目的节点编号
    int sequenceNum = 0;     // 序列号（用于重传检测）
    simtime_t creationTime;  // 消息创建时间
    int ttl = 10;             // 生存时间（防止路由循环）
    int hopCount = 0;         // 已跳数计数
    bool retransmission = false;  // 是否为重传消息
    string payload = "";      // 消息内容负载
}
```

### AckMessage（确认消息）

```cpp
message AckMessage {
    int ackId = 0;            // ACK唯一标识
    int sourceNode = -1;      // ACK发送者（目的节点）
    int destNode = -1;       // ACK接收者（源节点）
    int sequenceNum = 0;     // 被确认的消息序列号
    simtime_t sendTime;      // ACK发送时间
}
```

### RouteUpdateMsg（路由更新消息）

```cpp
message RouteUpdateMsg {
    int updateId = 0;         // 更新消息ID
    int sourceNode = -1;      // 发送更新的节点
    simtime_t updateTime;    // 更新时间
    int destCount = 0;        // 路由条目数量
    string routeEntries[];    // 路由条目列表
}
```

### HeartbeatMsg（心跳消息）

```cpp
message HeartbeatMsg {
    int heartbeatId = 0;     // 心跳ID
    int sourceNode = -1;     // 发送心跳的节点
    int targetNode = -1;     // 目标邻居节点
    simtime_t sendTime;      // 发送时间
    bool isAlive = true;     // 节点是否存活
}
```

---

## 节点参数说明

### EnhancedNode 模块（Node.ned）

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `nodeId` | int | 0 | 节点唯一标识（0-5） |
| `dropRate` | double | 0.05 | 丢包率（0-1） |
| `sendInterval` | double | 2.0 | 消息发送间隔（秒） |
| `numNodes` | int | 6 | 网络中节点总数 |
| `maxRetransmit` | int | 3 | 最大重传次数 |
| `ackTimeout` | double | 4.0 | ACK超时时间（秒） |
| `windowSize` | int | 5 | 拥塞控制窗口大小 |
| `enableCongestionControl` | bool | true | 是否启用拥塞控制 |
| `routeUpdateInterval` | double | 15.0 | 路由更新间隔（秒） |
| `enableLinkMonitoring` | bool | true | 是否启用链路监测 |
| `heartbeatInterval` | double | 5.0 | 心跳间隔（秒） |

---

## 配置参数说明

配置文件位置：`simulations/omnetpp.ini`

### 预定义配置场景

| 配置名称 | 说明 | 适用场景 |
|----------|------|----------|
| `Default` | 默认配置，启用所有增强功能 | 常规测试 |
| `CongestionHigh` | 大窗口拥塞控制 | 高吞吐量测试 |
| `CongestionLow` | 小窗口拥塞控制 | 低延迟测试 |
| `HighLoss` | 高丢包率场景 | 可靠性测试 |
| `LowLoss` | 低丢包率场景 | 性能基准测试 |
| `FastHeartbeat` | 快速心跳检测 | 快速故障检测测试 |
| `FastRouting` | 快速路由收敛 | 路由协议测试 |
| `LongRun` | 长时间仿真 | 稳定性测试 |

---

## 运行方式

### 编译项目

```bash
cd /home/sfm/文档/omnet-project/multinodenet-enhanced
make -j4
```

### 图形界面模式（Qtenv）

```bash
chmod +x run.sh
./run.sh
```

或直接运行：

```bash
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/EnhancedNet -u Qtenv -f simulations/omnetpp.ini -n src:simulations
```

### 命令行模式（Cmdenv）

```bash
chmod +x run_cmdenv.sh
./run_cmdenv.sh
```

或直接运行：

```bash
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/EnhancedNet -u Cmdenv -f simulations/omnetpp.ini -n src:simulations
```

### 运行特定配置场景

```bash
# 运行高丢包场景
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/EnhancedNet -u Cmdenv -f simulations/omnetpp.ini -n src:simulations -c HighLoss

# 运行长时测试
source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/EnhancedNet -u Cmdenv -f simulations/omnetpp.ini -n src:simulations -c LongRun
```

---

## 输出结果

### 运行日志示例

```
===========================================
增强型节点 0 初始化完成
-----------------------------------------
  基础参数:
    丢包率: 3%
    发送间隔: 2s
    邻居节点: 5 (左), 1 (右)
  ACK与重传:
    最大重传次数: 3
    ACK超时时间: 4s
  拥塞控制:
    窗口大小: 5
    启用状态: 是
  路由表:
    更新间隔: 15s
  链路监测:
    心跳间隔: 5s
    启用状态: 是
===========================================
[节点0] 作为源节点，发送第一条消息
[节点0] 发送消息 #1 -> 目的节点 3 [窗口: 1/5]
[节点0] 收到ACK(seq=1) <- 来源节点 3
    ACK确认: 消息 #1 已确认交付
[节点0] 拥塞窗口增大（慢启动）: 2
...
===========================================
增强型节点 0 统计报告
===========================================
基础统计:
  发送消息数: 100
  接收消息数: 150
  交付消息数: 95
  丢包数: 5
  重传次数: 3
ACK与重传:
  ACK发送数: 95
  ACK接收数: 95
  待确认消息: 0
拥塞控制:
  窗口大小: 5
  飞行消息: 0
路由表:
  路由条目数: 5
  路由更新次数: 13
链路监测:
  心跳发送数: 40
  链路故障次数: 0
===========================================
```

### 结果文件

仿真结果保存在 `simulations/results/` 目录：

```
simulations/results/
├── Default-#0.sca    # 标量结果
├── Default-#0.vec    # 向量结果
├── Default-#0.anf    # 分析文件
├── HighLoss-#0.sca   # 高丢包场景标量结果
└── ...
```

---

## 技术栈

- **仿真框架：** OMNeT++ 6.2.0
- **编程语言：** C++（模块实现）、NED（拓扑描述）、MSG（消息定义）
- **编译器：** Clang
- **用户界面：** Cmdenv（命令行）、Qtenv（图形）

---

## 进一步扩展方向

当前 EnhancedNet 已实现以下功能，可进一步扩展：

1. **多路径路由：** 支持同时使用多条路径传输数据
2. **流量分配：** 根据网络状态动态分配流量
3. **能量模型：** 添加节点能耗统计
4. **移动性支持：** 支持节点位置变化
5. **服务质量（QoS）：** 支持不同优先级的消息类型
6. **多播支持：** 支持一对多的数据传输
7. **加密传输：** 添加消息加密和认证机制

---

*文档生成时间：2026-04-10*