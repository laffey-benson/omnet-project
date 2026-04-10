# EnhancedNet 增强型多节点网络仿真系统说明

## 目录

- [系统概述](#系统概述)
- [系统架构](#系统架构)
- [网络拓扑](#网络拓扑)
- [消息类型](#消息类型)
- [核心功能](#核心功能)
- [模块设计](#模块设计)
- [参数配置](#参数配置)
- [编译运行](#编译运行)
- [输出结果](#输出结果)
- [扩展方向](#扩展方向)

---

## 系统概述

| 属性 | 说明 |
|------|------|
| 项目名称 | EnhancedNet |
| 项目类型 | OMNeT++ 6.2.0 网络仿真系统 |
| 编程语言 | C++、NED、NES（消息定义） |
| 源码位置 | `/home/sfm/文档/omnet-project/multinodenet-enhanced/` |
| 默认仿真时长 | 200 秒 |
| 节点数量 | 6 个 |

EnhancedNet 是一个**增强型多节点环形网络通信仿真系统**，在原 MultiNodeNet 的基础上新增了可靠性传输、拥塞控制、动态路由和链路监测四大核心功能，适用于网络协议研究和性能分析。

---

## 系统架构

```
┌─────────────────────────────────────────────────────────────────┐
│                        EnhancedNet 系统架构                      │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│    ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐          │
│    │  节点0  │──│  节点1  │──│  节点2  │──│  节点3  │          │
│    └────┬────┘  └────┬────┘  └────┬────┘  └────┬────┘          │
│         │            │            │            │                │
│         └────────────┴────────────┴────────────┘                │
│                      环形拓扑（6节点）                            │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│                      节点内部功能模块                             │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐              │
│  │ ACK/重传    │  │ 拥塞控制    │  │ 路由表维护  │              │
│  │ 模块        │  │ 模块        │  │ 模块        │              │
│  └──────┬──────┘  └──────┬──────┘  └──────┬──────┘              │
│         │                │                │                     │
│         └────────────────┼────────────────┘                     │
│                          │                                      │
│                   ┌──────┴──────┐                               │
│                   │  消息处理    │                               │
│                   │  核心        │                               │
│                   └─────────────┘                               │
│                          │                                      │
│  ┌─────────────┐  ┌──────┴──────┐  ┌─────────────┐              │
│  │ 链路监测    │  │  定时器      │  │  统计记录    │              │
│  │ 模块        │  │  管理        │  │  模块        │              │
│  └─────────────┘  └─────────────┘  └─────────────┘              │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## 网络拓扑

### 物理拓扑

```
                       left        right
                     ←──────       ──────→
                 0 ◄─────────────────────► 1
                 ▲                           ▲
                 │      ◄──────────►         │
                 │                           │
                 │      ◄──────────►         │
                 ▼                           ▼
                 5 ◄─────────────────────► 2
                 ▲                           ▲
                 │      ◄──────────►         │
                 ▼                           ▼
                 4 ◄─────────────────────► 3
                     ◄──────       ──────→
                       right       left
```

### 拓扑特点

- **6 节点环形结构**：节点编号 0-5，顺时针和逆时针均可通信
- **双通道连接**：每个节点与左右邻居各有一条双向链路
- **统一链路延迟**：所有链路延迟均为 100ms
- **最短路径路由**：消息沿环形传递，选择跳数最少的路径

### 端口定义

| 端口 | 方向 | 连接对象 |
|------|------|---------|
| `inLeft` | 输入 | 左邻居节点 |
| `outLeft` | 输出 | 左邻居节点 |
| `inRight` | 输入 | 右邻居节点 |
| `outRight` | 输出 | 右邻居节点 |

---

## 消息类型

### 1. EnhancedNetMsg（增强型数据消息）

```cpp
message EnhancedNetMsg {
    int messageId = 0;           // 消息唯一标识
    int sourceNode = -1;         // 源节点编号
    int destNode = -1;           // 目的节点编号
    int sequenceNum = 0;         // 序列号（用于重传检测）
    simtime_t creationTime;      // 消息创建时间
    int ttl = 10;                // 生存时间（防止路由循环）
    int hopCount = 0;            // 已跳数计数
    bool retransmission = false; // 是否为重传消息
    string payload = "";         // 消息内容负载
}
```

### 2. AckMessage（确认消息）

```cpp
message AckMessage {
    int ackId = 0;              // ACK唯一标识
    int sourceNode = -1;         // ACK发送者（目的节点）
    int destNode = -1;           // ACK接收者（源节点）
    int sequenceNum = 0;         // 被确认的消息序列号
    simtime_t sendTime;          // ACK发送时间
}
```

### 3. RouteUpdateMsg（路由更新消息）

```cpp
message RouteUpdateMsg {
    int updateId = 0;            // 更新消息ID
    int sourceNode = -1;         // 发送更新的节点
    simtime_t updateTime;        // 更新时间
    int destCount = 0;           // 路由条目数量
    string routeEntries[];       // 路由条目列表
}
```

### 4. HeartbeatMsg（心跳消息）

```cpp
message HeartbeatMsg {
    int heartbeatId = 0;        // 心跳ID
    int sourceNode = -1;         // 发送心跳的节点
    int targetNode = -1;         // 目标邻居节点
    simtime_t sendTime;          // 发送时间
    bool isAlive = true;        // 节点是否存活
}
```

---

## 核心功能

### 功能对比

| 功能特性 | MultiNodeNet | EnhancedNet |
|---------|--------------|-------------|
| 节点数量 | 4 个 | 6 个 |
| 网络拓扑 | 4节点环形 | 6节点环形 |
| 丢包模拟 | 随机丢包 | 随机丢包 |
| 延迟统计 | 端到端延迟 | 端到端延迟 |
| **ACK确认** | 无 | 有 |
| **重传机制** | 无 | 有 |
| **拥塞控制** | 无 | 有 |
| **路由表维护** | 简单固定 | 动态更新 |
| **链路监测** | 无 | 心跳机制 |

---

### 1. ACK 确认与重传机制

#### 工作流程

```
源节点(0)                          目的节点(3)
    │                                    │
    │── EnhancedNetMsg(seq=100) ────────>│
    │     (TTL=10, Hop=0)                │
    │                                    │
    │              ... 路由转发中 ...     │
    │                                    │
    │<────── AckMessage(seq=100) ────────│
    │                                    │
```

#### 核心机制

| 机制 | 说明 |
|------|------|
| 消息缓冲 | 源节点维护 `pendingMessages` 映射，记录每个已发送消息 |
| 超时检测 | 每 0.5 秒检查一次未确认消息是否超时 |
| 自动重传 | 超时消息自动重传，最多 `maxRetransmit` 次 |
| 序列号 | 每条消息分配唯一序列号，用于 ACK 匹配 |

#### 参数配置

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `maxRetransmit` | 3 | 最大重传次数 |
| `ackTimeout` | 2.0s | ACK 超时时间 |

---

### 2. 拥塞控制

#### 滑动窗口机制

```
发送窗口示意：

┌─────────────────────────────────────────────────────────┐
│  已确认    │   已确认    │   等待中   │   待发送   │   未使用 │
├───────────┼─────────────┼───────────┼───────────┼─────────┤
│  Msg#1    │   Msg#2     │   Msg#3   │   Msg#4   │  ...    │
│           │             │           │           │         │
└───────────┴─────────────┴───────────┴───────────┴─────────┘
          ↑窗口左边界                    ↑窗口右边界
```

#### 窗口调整策略

| 事件 | 窗口变化 | 说明 |
|------|---------|------|
| 收到 ACK | +1 | 慢启动阶段 |
| 超时 | -1 | 拥塞避免 |
| 窗口达到上限 | 停止增长 | 受 `windowSize` 限制 |

#### 参数配置

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `windowSize` | 3 | 最大窗口大小 |
| `enableCongestionControl` | true | 是否启用拥塞控制 |

---

### 3. 动态路由表

#### 路由表结构

```cpp
struct RouteEntry {
    int destNode;           // 目的节点
    int nextHop;            // 下一跳节点
    int metric;             // 跳数度量
    simtime_t lastUpdate;   // 最后更新时间
    int sequence;            // 序列号（防旧信息）
};
```

#### 最短路径计算

```
示例：节点0到节点3的路由选择

    节点0          节点3
       │            │
       ├──顺时针───>│  (3跳)
       │            │
       │<──逆时针───┤  (3跳)
       │            │

两种路径跳数相同，选择顺时针方向
```

#### 路由更新机制

| 特性 | 说明 |
|------|------|
| 更新周期 | 每 10 秒广播一次 |
| 距离矢量 | 每个节点维护到所有其他节点的路由 |
| 序列号 | 防止接收过时的路由信息 |
| 备用路由 | 主路由不可用时切换到备用方向 |

---

### 4. 链路监测

#### 心跳机制

```
节点0                          节点1
   │                               │
   │── HeartbeatMsg(alive=true) ─>│
   │     targetNode = 1           │
   │                               │
   │── HeartbeatMsg(alive=true) ─>│
   │     targetNode = 5           │
   │                               │
```

#### 故障检测

| 状态 | 检测条件 |
|------|---------|
| 正常 | 心跳间隔内收到响应 |
| 疑似故障 | 超过 2 倍心跳间隔未响应 |
| 确认故障 | 超过 3 倍心跳间隔未响应 |

#### 参数配置

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `heartbeatInterval` | 5.0s | 心跳发送间隔 |
| `enableLinkMonitoring` | true | 是否启用链路监测 |

---

## 模块设计

### 类结构

```cpp
class EnhancedNode : public cSimpleModule
{
  private:
    // 基础参数
    int nodeId;              // 节点标识
    double dropRate;         // 丢包率
    int numNodes;            // 网络节点总数

    // ACK与重传
    std::map<int, PendingMessage> pendingMessages;
    int sequenceCounter;

    // 拥塞控制
    int windowSize, currentWindowSize;
    int messagesInFlight;

    // 路由表
    std::map<int, RouteEntry> routingTable;

    // 链路监测
    std::set<int> aliveNeighbors;
    std::map<int, simtime_t> lastHeartbeat;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 消息处理
    void sendMessage();
    void processDataMessage(EnhancedNetMsg *msg);
    void sendAck(int destNode, int sequenceNum);
    void retransmitExpiredMessages();

    // 拥塞控制
    bool canSendMessage();
    void adjustWindowSize(bool ackReceived);

    // 路由功能
    void initRoutingTable();
    void broadcastRouteUpdate();
    int getNextHop(int destNode);

    // 链路监测
    void sendHeartbeat();
    void checkNeighborHealth();
};
```

### 定时器系统

| 定时器 | 触发间隔 | 功能 |
|--------|---------|------|
| `sendTimer` | `sendInterval` (1s) | 源节点发送新消息 |
| `ackTimer` | 0.5s | 检查 ACK 超时 |
| `heartbeatTimer` | `heartbeatInterval` (5s) | 发送心跳 + 检查邻居 |
| `routeUpdateTimer` | `routeUpdateInterval` (10s) | 广播路由更新 |
| `windowAdjustTimer` | 5s | 调整拥塞窗口 |

### 消息处理流程

```
                    ┌─────────────┐
                    │ 收到消息    │
                    └──────┬──────┘
                           │
              ┌────────────┼────────────┐
              │            │            │
              ▼            ▼            ▼
        ┌─────────┐  ┌─────────┐  ┌─────────┐
        │定时器消息│  │数据消息 │  │其他消息 │
        └────┬────┘  └────┬────┘  └────┬────┘
             │             │             │
             ▼             ▼             ▼
        ┌─────────┐  ┌─────────┐  ┌─────────┐
        │处理定时 │  │检查TTL  │  │ACK/路由/│
        │事件     │  │路由转发 │  │心跳处理 │
        └─────────┘  └────┬────┘  └─────────┘
                          │
              ┌───────────┴───────────┐
              │                       │
              ▼                       ▼
        ┌─────────┐             ┌─────────┐
        │目的节点 │             │中间节点 │
        │发送ACK  │             │转发消息 │
        └─────────┘             └─────────┘
```

---

## 参数配置

### 节点参数

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `nodeId` | int | 0 | 节点唯一标识 (0-5) |
| `dropRate` | double | 0.05 | 丢包率 (0-1) |
| `sendInterval` | double | 1.0 | 消息发送间隔 (秒) |
| `numNodes` | int | 6 | 网络中节点总数 |
| `maxRetransmit` | int | 3 | 最大重传次数 |
| `ackTimeout` | double | 2.0 | ACK 超时时间 (秒) |
| `windowSize` | int | 3 | 拥塞控制窗口大小 |
| `enableCongestionControl` | bool | true | 是否启用拥塞控制 |
| `routeUpdateInterval` | double | 10.0 | 路由更新间隔 (秒) |
| `enableLinkMonitoring` | bool | true | 是否启用链路监测 |
| `heartbeatInterval` | double | 5.0 | 心跳间隔 (秒) |

### 预定义配置场景

| 配置名称 | 说明 | 典型应用 |
|----------|------|---------|
| `Default` | 默认配置，启用所有功能 | 常规测试 |
| `CongestionHigh` | 大窗口 (5)，高吞吐量 | 性能测试 |
| `CongestionLow` | 小窗口 (1)，低延迟 | 延迟敏感测试 |
| `HighLoss` | 高丢包率 (20%) | 可靠性测试 |
| `LowLoss` | 低丢包率 (1%) | 基准测试 |
| `FastHeartbeat` | 快速心跳 (1s) | 快速故障检测 |
| `FastRouting` | 快速路由更新 (5s) | 路由收敛测试 |
| `LongRun` | 长时间仿真 (600s) | 稳定性测试 |

---

## 编译运行

### 目录结构

```
multinodenet-enhanced/
├── src/
│   ├── package.ned           # 包声明
│   ├── EnhancedNet.ned      # 网络拓扑定义
│   ├── Node.ned             # 节点模块定义
│   ├── NodeApp.cc           # 节点实现代码
│   ├── messages.msg         # 消息定义源文件
│   ├── messages_m.cc        # 自动生成代码
│   └── messages_m.h         # 自动生成头文件
├── simulations/
│   ├── omnetpp.ini          # 仿真配置
│   └── results/             # 结果输出目录
├── out/
│   └── clang-release/
│       └── EnhancedNet       # 编译后的可执行文件
├── Makefile                  # 编译配置
├── run.sh                    # Qtenv 图形启动脚本
├── run_cmdenv.sh            # Cmdenv 命令行启动脚本
└── EnhancedNet系统说明.md    # 系统说明文档
```

### 编译项目

```bash
cd /home/sfm/文档/omnet-project/multinodenet-enhanced
make -j4
```

### 运行方式

#### 图形界面模式 (Qtenv)

```bash
chmod +x run.sh
./run.sh
```

#### 命令行模式 (Cmdenv)

```bash
chmod +x run_cmdenv.sh
./run_cmdenv.sh
```

#### 运行特定配置

```bash
# 设置环境变量
source /home/sfm/omnetpp-6.2.0/setenv

# 运行高丢包场景
./out/clang-release/EnhancedNet -u Cmdenv -f simulations/omnetpp.ini -c HighLoss

# 运行长时测试
./out/clang-release/EnhancedNet -u Cmdenv -f simulations/omnetpp.ini -c LongRun
```

---

## 输出结果

### 仿真日志

```
===========================================
增强型节点 0 初始化完成
-----------------------------------------
  基础参数:
    丢包率: 5%
    发送间隔: 1s
    邻居节点: 5 (左), 1 (右)
  ACK与重传:
    最大重传次数: 3
    ACK超时时间: 2s
  拥塞控制:
    窗口大小: 3
    启用状态: 是
  路由表:
    更新间隔: 10s
  链路监测:
    心跳间隔: 5s
    启用状态: 是
===========================================
[节点0] 作为源节点，发送第一条消息
[节点0] 发送消息 #1 -> 目的节点 3 [窗口: 1/3]
[节点0] 收到ACK(seq=1) <- 来源节点 3
    ACK确认: 消息 #1 已确认交付
[节点0] 拥塞窗口增大（慢启动）: 2
...
```

### 统计报告

```
===========================================
增强型节点 0 统计报告
===========================================
基础统计:
  发送消息数: XX
  接收消息数: XX
  交付消息数: XX
  丢包数: XX
  重传次数: XX

ACK与重传:
  ACK发送数: XX
  ACK接收数: XX
  待确认消息: XX

拥塞控制:
  窗口大小: XX
  飞行消息: XX

路由表:
  路由条目数: XX
  路由更新次数: XX

链路监测:
  心跳发送数: XX
  链路故障次数: XX
===========================================
```

### 结果文件

| 文件类型 | 扩展名 | 说明 |
|---------|--------|------|
| 标量结果 | `.sca` | 各模块统计标量 |
| 向量结果 | `.vec` | 时序变化数据 |
| 分析文件 | `.anf` | 仿真结果分析 |

---

## 扩展方向

| 扩展方向 | 说明 |
|---------|------|
| **多路径路由** | 支持同时使用多条路径传输数据 |
| **流量分配** | 根据网络状态动态分配流量 |
| **能量模型** | 添加节点能耗统计 |
| **移动性支持** | 支持节点位置变化 |
| **QoS 机制** | 支持不同优先级的消息类型 |
| **多播支持** | 支持一对多的数据传输 |
| **加密传输** | 添加消息加密和认证机制 |

---

*文档生成时间：2026-04-10*
*OMNeT++ 版本：6.2.0*
