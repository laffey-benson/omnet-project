// ======================================================================
// Enhanced Multi-Node Network Simulation System
// EnhancedNet - MultiNodeNet Enhanced Version
//
// NEW FEATURES (compared to MultiNodeNet):
// 1. ACK & Retransmission Mechanism - Reliable data transmission
// 2. Congestion Control - Sliding window based flow control
// 3. Routing Table Maintenance - Dynamic route updates
// 4. Link Monitoring - Heartbeat-based neighbor detection
// ======================================================================

#include <omnetpp.h>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include "messages_m.h"

using namespace omnetpp;

namespace multinodenet_enhanced {

// 路由条目结构
struct RouteEntry {
    int destNode;           // 目的节点
    int nextHop;            // 下一跳节点
    int metric;             // 跳数度量
    simtime_t lastUpdate;   // 最后更新时间
    int sequence;           // 序列号（用于检测过时信息）
};

// 待发送消息缓冲结构
struct PendingMessage {
    EnhancedNetMsg *msg;    // 原始消息
    simtime_t sendTime;     // 发送时间
    int retransmitCount;    // 重传次数
    bool ackReceived;       // 是否已收到ACK
};

class EnhancedNode : public cSimpleModule
{
  private:
    // ========== 节点基础参数 ==========
    int nodeId;
    double dropRate;
    double sendInterval;
    int numNodes;

    // ========== ACK与重传参数 ==========
    int maxRetransmit;          // 最大重传次数
    double ackTimeout;          // ACK超时时间
    std::map<int, PendingMessage> pendingMessages;  // 待确认消息缓冲
    int sequenceCounter;        // 序列号计数器

    // ========== 拥塞控制参数 ==========
    int windowSize;             // 滑动窗口大小
    int currentWindowSize;      // 当前窗口大小
    int messagesInFlight;        // 飞行中的消息数
    bool enableCongestionControl;  // 是否启用拥塞控制

    // ========== 路由表参数 ==========
    std::map<int, RouteEntry> routingTable;  // 路由表
    double routeUpdateInterval;              // 路由更新间隔
    int routeUpdateCounter;                  // 路由更新计数器

    // ========== 链路监测参数 ==========
    bool enableLinkMonitoring;    // 是否启用链路监测
    double heartbeatInterval;     // 心跳间隔
    std::set<int> aliveNeighbors; // 存活的邻居节点
    std::map<int, simtime_t> lastHeartbeat;  // 最后收到心跳的时间

    // ========== 统计变量 ==========
    int messagesSent;
    int messagesReceived;
    int messagesDelivered;        // 成功交付的消息数
    int messagesDropped;          // 丢包数
    int messagesRetransmitted;    // 重传次数
    int acksSent;                // 发送的ACK数
    int acksReceived;            // 收到的ACK数
    simtime_t totalDelay;
    int delayCount;
    int routeUpdatesSent;        // 路由更新次数
    int heartbeatsSent;          // 发送的心跳数
    int linkFailures;            // 链路故障次数

    // ========== 定时器 ==========
    cMessage *sendTimer;         // 发送定时器
    cMessage *ackTimer;          // ACK检查定时器
    cMessage *heartbeatTimer;    // 心跳定时器
    cMessage *routeUpdateTimer;  // 路由更新定时器
    cMessage *windowAdjustTimer; // 窗口调整定时器

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // ========== 基础消息功能 ==========
    void sendMessage();
    void processDataMessage(EnhancedNetMsg *msg);
    void sendToDirection(int nextNode, cMessage *msg);
    bool shouldDropPacket();

    // ========== ACK与重传功能 ==========
    void sendAck(int destNode, int sequenceNum);
    void processAck(AckMessage *ack);
    void checkPendingMessages();
    void retransmitExpiredMessages();

    // ========== 拥塞控制功能 ==========
    bool canSendMessage();
    void adjustWindowSize(bool ackReceived);
    void decreaseWindowSize();

    // ========== 路由表功能 ==========
    void initRoutingTable();
    void updateRoutingTable(RouteUpdateMsg *update);
    void sendRouteUpdate();
    void broadcastRouteUpdate();
    int getNextHop(int destNode);
    int calculateMetric(int destNode);

    // ========== 链路监测功能 ==========
    void sendHeartbeat();
    void processHeartbeat(HeartbeatMsg *heartbeat);
    void checkNeighborHealth();
    bool isNeighborAlive(int neighborId);

    // ========== 辅助功能 ==========
    int getNextNode(int dest);
    void recordStatistics();
};

Define_Module(EnhancedNode);

// ========== 初始化 ==========
void EnhancedNode::initialize()
{
    // ========== 初始化所有成员变量 ==========
    sendTimer = nullptr;
    ackTimer = nullptr;
    heartbeatTimer = nullptr;
    routeUpdateTimer = nullptr;
    windowAdjustTimer = nullptr;
    messagesSent = 0;
    messagesReceived = 0;
    messagesDelivered = 0;
    messagesDropped = 0;
    messagesRetransmitted = 0;
    acksSent = 0;
    acksReceived = 0;
    totalDelay = 0;
    delayCount = 0;
    routeUpdatesSent = 0;
    heartbeatsSent = 0;
    linkFailures = 0;
    sequenceCounter = 0;

    // 读取基础参数
    nodeId = par("nodeId");
    dropRate = par("dropRate");
    sendInterval = par("sendInterval");
    numNodes = par("numNodes");

    // 读取ACK与重传参数
    maxRetransmit = par("maxRetransmit");
    ackTimeout = par("ackTimeout");

    // 读取拥塞控制参数
    windowSize = par("windowSize");
    currentWindowSize = windowSize;
    messagesInFlight = 0;
    enableCongestionControl = par("enableCongestionControl");

    // 读取路由表参数
    routeUpdateInterval = par("routeUpdateInterval");
    routeUpdateCounter = 0;

    // 读取链路监测参数
    enableLinkMonitoring = par("enableLinkMonitoring");
    heartbeatInterval = par("heartbeatInterval");

    // 初始化路由表
    initRoutingTable();

    // 初始化邻居状态
    int leftNeighbor = (nodeId + numNodes - 1) % numNodes;
    int rightNeighbor = (nodeId + 1) % numNodes;
    aliveNeighbors.insert(leftNeighbor);
    aliveNeighbors.insert(rightNeighbor);
    lastHeartbeat[leftNeighbor] = simTime();
    lastHeartbeat[rightNeighbor] = simTime();

    // 打印初始化信息
    EV << "===========================================\n";
    EV << "增强型节点 " << nodeId << " 初始化完成\n";
    EV << "-----------------------------------------\n";
    EV << "  基础参数:\n";
    EV << "    丢包率: " << (dropRate * 100) << "%\n";
    EV << "    发送间隔: " << sendInterval << "s\n";
    EV << "    邻居节点: " << leftNeighbor << " (左), " << rightNeighbor << " (右)\n";
    EV << "  ACK与重传:\n";
    EV << "    最大重传次数: " << maxRetransmit << "\n";
    EV << "    ACK超时时间: " << ackTimeout << "s\n";
    EV << "  拥塞控制:\n";
    EV << "    窗口大小: " << windowSize << "\n";
    EV << "    启用状态: " << (enableCongestionControl ? "是" : "否") << "\n";
    EV << "  路由表:\n";
    EV << "    更新间隔: " << routeUpdateInterval << "s\n";
    EV << "  链路监测:\n";
    EV << "    心跳间隔: " << heartbeatInterval << "s\n";
    EV << "    启用状态: " << (enableLinkMonitoring ? "是" : "否") << "\n";
    EV << "===========================================\n";

    // 节点0作为源节点，初始化时发送第一条消息
    if (nodeId == 0) {
        EV << "[节点" << nodeId << "] 作为源节点，发送第一条消息\n";
        sendMessage();

        // 创建并调度发送定时器
        sendTimer = new cMessage("sendTimer");
        scheduleAt(simTime() + sendInterval, sendTimer);
    }

    // 启动ACK检查定时器
    ackTimer = new cMessage("ackTimer");
    scheduleAt(simTime() + 0.5, ackTimer);

    // 启动心跳定时器（所有节点）
    if (enableLinkMonitoring) {
        heartbeatTimer = new cMessage("heartbeatTimer");
        scheduleAt(simTime() + heartbeatInterval, heartbeatTimer);
    }

    // 启动路由更新定时器
    routeUpdateTimer = new cMessage("routeUpdateTimer");
    scheduleAt(simTime() + routeUpdateInterval, routeUpdateTimer);

    // 启动窗口调整定时器
    windowAdjustTimer = new cMessage("windowAdjustTimer");
    scheduleAt(simTime() + 5.0, windowAdjustTimer);
}

// ========== 消息处理 ==========
void EnhancedNode::handleMessage(cMessage *msg)
{
    // 判断消息类型
    if (strcmp(msg->getName(), "sendTimer") == 0) {
        // 发送定时器：检查是否可以发送新消息
        if (canSendMessage()) {
            sendMessage();
        }
        scheduleAt(simTime() + sendInterval, msg);
    }
    else if (strcmp(msg->getName(), "ackTimer") == 0) {
        // ACK检查定时器：检查超时消息
        checkPendingMessages();
        scheduleAt(simTime() + 0.5, msg);
    }
    else if (strcmp(msg->getName(), "heartbeatTimer") == 0) {
        // 心跳定时器：发送心跳并检查邻居健康
        if (enableLinkMonitoring) {
            sendHeartbeat();
            checkNeighborHealth();
            scheduleAt(simTime() + heartbeatInterval, msg);
        }
    }
    else if (strcmp(msg->getName(), "routeUpdateTimer") == 0) {
        // 路由更新定时器：广播路由更新
        broadcastRouteUpdate();
        scheduleAt(simTime() + routeUpdateInterval, msg);
    }
    else if (strcmp(msg->getName(), "windowAdjustTimer") == 0) {
        // 窗口调整定时器：动态调整窗口大小
        if (enableCongestionControl && currentWindowSize < windowSize) {
            currentWindowSize++;
            EV << "[节点" << nodeId << "] 拥塞窗口增大: " << currentWindowSize << "\n";
        }
        scheduleAt(simTime() + 5.0, msg);
    }
    else if (dynamic_cast<EnhancedNetMsg*>(msg)) {
        // 数据消息
        EnhancedNetMsg *netMsg = check_and_cast<EnhancedNetMsg*>(msg);
        processDataMessage(netMsg);
    }
    else if (dynamic_cast<AckMessage*>(msg)) {
        // ACK消息
        AckMessage *ack = check_and_cast<AckMessage*>(msg);
        processAck(ack);
    }
    else if (dynamic_cast<RouteUpdateMsg*>(msg)) {
        // 路由更新消息
        RouteUpdateMsg *update = check_and_cast<RouteUpdateMsg*>(msg);
        updateRoutingTable(update);
        delete msg;
    }
    else if (dynamic_cast<HeartbeatMsg*>(msg)) {
        // 心跳消息
        HeartbeatMsg *heartbeat = check_and_cast<HeartbeatMsg*>(msg);
        processHeartbeat(heartbeat);
        delete msg;
    }
    else {
        // 未知消息类型
        EV << "[节点" << nodeId << "] 收到未知类型消息: " << msg->getName() << "\n";
        delete msg;
    }
}

// ========== 发送消息 ==========
void EnhancedNode::sendMessage()
{
    // 检查飞行中的消息数是否超过窗口大小
    if (enableCongestionControl && messagesInFlight >= currentWindowSize) {
        EV << "[节点" << nodeId << "] 拥塞控制：飞行消息数已达窗口上限 ("
           << messagesInFlight << "/" << currentWindowSize << ")，等待ACK\n";
        return;
    }

    sequenceCounter++;
    int currentSeq = sequenceCounter;

    // 创建新消息
    EnhancedNetMsg *msg = new EnhancedNetMsg();
    msg->setMessageId(currentSeq);
    msg->setSourceNode(nodeId);
    msg->setSequenceNum(currentSeq);
    msg->setCreationTime(simTime());
    msg->setTtl(10);
    msg->setHopCount(0);
    msg->setRetransmission(false);

    // 目的节点：随机选择（排除自身）
    int destNode;
    do {
        destNode = intuniform(0, numNodes - 1);
    } while (destNode == nodeId);

    msg->setDestNode(destNode);
    msg->setPayload(("Enhanced msg #" + std::to_string(currentSeq) +
                     " from node " + std::to_string(nodeId)).c_str());

    messagesSent++;

    EV << "[节点" << nodeId << "] 发送消息 #" << currentSeq
       << " -> 目的节点 " << destNode
       << " [窗口: " << messagesInFlight + 1 << "/" << currentWindowSize << "]\n";

    // 添加到待确认缓冲
    PendingMessage pending;
    pending.msg = msg->dup();
    pending.sendTime = simTime();
    pending.retransmitCount = 0;
    pending.ackReceived = false;
    pendingMessages[currentSeq] = pending;

    // 增加飞行消息计数
    messagesInFlight++;
    recordScalar("飞行消息数", messagesInFlight);

    // 计算下一跳并发送
    int nextNode = getNextHop(destNode);
    sendToDirection(nextNode, msg);

    // 更新统计
    recordScalar("发送消息数", messagesSent);
}

// ========== 处理数据消息 ==========
void EnhancedNode::processDataMessage(EnhancedNetMsg *msg)
{
    messagesReceived++;

    int srcNode = msg->getSourceNode();
    int destNode = msg->getDestNode();
    int seqNum = msg->getSequenceNum();
    int ttl = msg->getTtl();
    int hopCount = msg->getHopCount();
    bool isRetrans = msg->getRetransmission();
    simtime_t delay = simTime() - msg->getCreationTime();

    EV << "[节点" << nodeId << "] 收到消息 #" << msg->getMessageId()
       << " (seq=" << seqNum << ", 源:" << srcNode << " -> 目的:" << destNode
       << ", 跳数:" << hopCount << "/" << ttl << ", 延迟:" << delay << "s";
    if (isRetrans) EV << " [重传]";
    EV << ")\n";

    // TTL检查
    if (ttl <= 0) {
        EV << "    [丢弃] TTL耗尽\n";
        messagesDropped++;
        delete msg;
        return;
    }

    // 丢包模拟
    if (shouldDropPacket()) {
        EV << "    [丢弃] 随机丢包\n";
        messagesDropped++;
        recordScalar("丢包数", messagesDropped);
        delete msg;
        return;
    }

    // 检查是否是目的节点
    if (destNode == nodeId) {
        // 消息到达目的节点
        EV << "    >>> 消息已到达目的节点！发送ACK\n";

        messagesDelivered++;

        // 记录延迟统计
        totalDelay += delay;
        delayCount++;

        // 发送ACK给源节点
        sendAck(srcNode, seqNum);

        // 记录统计
        recordScalar("接收消息数", messagesReceived);
        recordScalar("交付消息数", messagesDelivered);
        recordScalar("有效消息延迟", delay);

        delete msg;
    } else {
        // 需要转发到下一跳
        EV << "    路由中... 目的节点是 " << destNode;

        // 检查邻居是否存活
        int nextNode = getNextHop(destNode);
        if (!isNeighborAlive(nextNode)) {
            EV << " [邻居" << nextNode << "不可达，尝试备用路由]\n";
            // 尝试另一个方向的邻居
            int altNode = (nextNode == (nodeId + 1) % numNodes) ?
                          (nodeId + numNodes - 1) % numNodes :
                          (nodeId + 1) % numNodes;
            if (isNeighborAlive(altNode)) {
                nextNode = altNode;
                EV << "    使用备用路由: " << nextNode << "\n";
            } else {
                EV << "    [丢弃] 无可用路由\n";
                messagesDropped++;
                delete msg;
                return;
            }
        }

        EV << " -> 转发到节点 " << nextNode << "\n";

        // 更新TTL和跳数
        msg->setTtl(ttl - 1);
        msg->setHopCount(hopCount + 1);

        // 转发消息
        sendToDirection(nextNode, msg);
    }
}

// ========== 发送ACK ==========
void EnhancedNode::sendAck(int destNode, int sequenceNum)
{
    AckMessage *ack = new AckMessage();
    ack->setAckId(sequenceNum);
    ack->setSourceNode(nodeId);
    ack->setDestNode(destNode);
    ack->setSequenceNum(sequenceNum);
    ack->setSendTime(simTime());

    acksSent++;

    EV << "[节点" << nodeId << "] 发送ACK(seq=" << sequenceNum
       << ") -> 目的节点 " << destNode << "\n";

    // 通过路由发送ACK
    int nextNode = getNextHop(destNode);
    sendToDirection(nextNode, ack);

    recordScalar("ACK发送数", acksSent);
}

// ========== 处理ACK ==========
void EnhancedNode::processAck(AckMessage *ack)
{
    int srcNode = ack->getSourceNode();
    int seqNum = ack->getSequenceNum();

    EV << "[节点" << nodeId << "] 收到ACK(seq=" << seqNum
       << ") <- 来源节点 " << srcNode << "\n";

    acksReceived++;

    // 查找对应的待确认消息
    auto it = pendingMessages.find(seqNum);
    if (it != pendingMessages.end()) {
        it->second.ackReceived = true;
        messagesInFlight--;

        EV << "    ACK确认: 消息 #" << seqNum << " 已确认交付\n";

        // 拥塞控制：收到ACK，增加窗口
        if (enableCongestionControl) {
            adjustWindowSize(true);
        }

        // 删除已确认的消息
        delete it->second.msg;
        pendingMessages.erase(it);
    } else {
        EV << "    [警告] 未找到对应的待确认消息 #" << seqNum << "\n";
    }

    recordScalar("ACK接收数", acksReceived);
    delete ack;
}

// ========== 检查待确认消息 ==========
void EnhancedNode::checkPendingMessages()
{
    retransmitExpiredMessages();
}

// ========== 重传过期消息 ==========
void EnhancedNode::retransmitExpiredMessages()
{
    simtime_t now = simTime();
    std::vector<int> toRemove;

    for (auto &pair : pendingMessages) {
        int seqNum = pair.first;
        PendingMessage &pending = pair.second;

        if (pending.ackReceived) continue;

        simtime_t elapsed = now - pending.sendTime;

        if (elapsed >= ackTimeout) {
            if (pending.retransmitCount >= maxRetransmit) {
                EV << "[节点" << nodeId << "] 消息 #" << seqNum
                   << " 重传次数超限，放弃\n";
                messagesDropped++;
                messagesInFlight--;
                toRemove.push_back(seqNum);
                continue;
            }

            pending.retransmitCount++;
            messagesRetransmitted++;

            EV << "[节点" << nodeId << "] 消息 #" << seqNum
               << " 超时未收到ACK，进行第 " << pending.retransmitCount
               << " 次重传\n";

            EnhancedNetMsg *retransMsg = pending.msg->dup();
            retransMsg->setRetransmission(true);

            int destNode = retransMsg->getDestNode();
            int nextNode = getNextHop(destNode);
            sendToDirection(nextNode, retransMsg);

            pending.sendTime = now;

            if (enableCongestionControl) {
                decreaseWindowSize();
            }

            recordScalar("重传次数", messagesRetransmitted);
        }
    }

    for (int seqNum : toRemove) {
        auto it = pendingMessages.find(seqNum);
        if (it != pendingMessages.end()) {
            delete it->second.msg;
            pendingMessages.erase(it);
        }
    }
}

// ========== 拥塞控制：是否可以发送 ==========
bool EnhancedNode::canSendMessage()
{
    if (!enableCongestionControl) return true;
    return messagesInFlight < currentWindowSize;
}

// ========== 拥塞控制：窗口调整（收到ACK） ==========
void EnhancedNode::adjustWindowSize(bool ackReceived)
{
    if (!enableCongestionControl) return;

    if (ackReceived) {
        // 收到ACK，慢启动策略
        if (currentWindowSize < windowSize) {
            currentWindowSize++;
            EV << "[节点" << nodeId << "] 拥塞窗口增大（慢启动）: "
               << currentWindowSize << "\n";
        }
    }
}

// ========== 拥塞控制：窗口减小 ==========
void EnhancedNode::decreaseWindowSize()
{
    if (!enableCongestionControl) return;

    int oldSize = currentWindowSize;
    currentWindowSize = std::max(1, currentWindowSize - 1);

    EV << "[节点" << nodeId << "] 拥塞窗口减小: " << oldSize
       << " -> " << currentWindowSize << "\n";
}

// ========== 初始化路由表 ==========
void EnhancedNode::initRoutingTable()
{
    EV << "[节点" << nodeId << "] 初始化路由表\n";

    // 添加直接相连的邻居路由
    int leftNeighbor = (nodeId + numNodes - 1) % numNodes;
    int rightNeighbor = (nodeId + 1) % numNodes;

    // 左邻居路由（1跳）
    RouteEntry leftEntry;
    leftEntry.destNode = leftNeighbor;
    leftEntry.nextHop = leftNeighbor;
    leftEntry.metric = 1;
    leftEntry.lastUpdate = simTime();
    leftEntry.sequence = 0;
    routingTable[leftNeighbor] = leftEntry;

    // 右邻居路由（1跳）
    RouteEntry rightEntry;
    rightEntry.destNode = rightNeighbor;
    rightEntry.nextHop = rightNeighbor;
    rightEntry.metric = 1;
    rightEntry.lastUpdate = simTime();
    rightEntry.sequence = 0;
    routingTable[rightNeighbor] = rightEntry;

    // 初始化其他节点的路由（通过邻居间接可达）
    for (int i = 0; i < numNodes; i++) {
        if (i == nodeId) continue;
        if (routingTable.find(i) != routingTable.end()) continue;

        // 计算到目标节点的距离（顺时针）
        int clockwiseDist = (i - nodeId + numNodes) % numNodes;
        int counterClockwiseDist = numNodes - clockwiseDist;

        RouteEntry entry;
        entry.destNode = i;
        entry.metric = std::min(clockwiseDist, counterClockwiseDist) + 1;
        entry.lastUpdate = simTime();
        entry.sequence = 0;

        if (clockwiseDist <= counterClockwiseDist) {
            entry.nextHop = rightNeighbor;
        } else {
            entry.nextHop = leftNeighbor;
        }

        routingTable[i] = entry;
    }

    // 打印路由表
    EV << "    路由表初始化完成:\n";
    for (const auto &pair : routingTable) {
        EV << "      到节点 " << pair.first
           << " -> 下一跳 " << pair.second.nextHop
           << " (度量: " << pair.second.metric << ")\n";
    }
}

// ========== 获取下一跳节点 ==========
int EnhancedNode::getNextHop(int destNode)
{
    if (destNode == nodeId) return nodeId;

    // 查找路由表
    auto it = routingTable.find(destNode);
    if (it != routingTable.end()) {
        return it->second.nextHop;
    }

    // 如果没有路由表条目，使用简单的最短路径算法
    return getNextNode(destNode);
}

// ========== 计算到目的节点的度量 ==========
int EnhancedNode::calculateMetric(int destNode)
{
    if (destNode == nodeId) return 0;

    int clockwiseDist = (destNode - nodeId + numNodes) % numNodes;
    int counterClockwiseDist = numNodes - clockwiseDist;

    return std::min(clockwiseDist, counterClockwiseDist);
}

// ========== 更新路由表 ==========
void EnhancedNode::updateRoutingTable(RouteUpdateMsg *update)
{
    int sourceNode = update->getSourceNode();
    simtime_t updateTime = update->getUpdateTime();

    EV << "[节点" << nodeId << "] 收到来自节点 " << sourceNode
       << " 的路由更新\n";

    // 解析路由条目
    int entryCount = update->getDestCount();
    for (size_t i = 0; i < static_cast<size_t>(entryCount); i++) {
        std::string entryStr = update->getRouteEntries(i);

        int dest, metric, nextHop, seq;
        char colon;
        std::istringstream iss(entryStr);
        iss >> dest >> colon >> metric >> colon >> nextHop >> colon >> seq;

        if (iss.fail()) continue;

        // 检查是否是更优的路由
        auto it = routingTable.find(dest);
        if (it == routingTable.end() || metric + 1 < it->second.metric ||
            (metric + 1 == it->second.metric && seq > it->second.sequence)) {

            RouteEntry newEntry;
            newEntry.destNode = dest;
            newEntry.nextHop = sourceNode;
            newEntry.metric = metric + 1;
            newEntry.lastUpdate = updateTime;
            newEntry.sequence = seq;

            routingTable[dest] = newEntry;

            EV << "    更新路由: 到节点 " << dest
               << " -> 下一跳 " << sourceNode
               << " (新度量: " << newEntry.metric << ")\n";
        }
    }
}

// ========== 广播路由更新 ==========
void EnhancedNode::broadcastRouteUpdate()
{
    if (routingTable.empty()) return;

    routeUpdateCounter++;

    RouteUpdateMsg *update = new RouteUpdateMsg();
    update->setUpdateId(routeUpdateCounter);
    update->setSourceNode(nodeId);
    update->setUpdateTime(simTime());

    // 构建路由条目
    std::vector<std::string> entries;
    for (const auto &pair : routingTable) {
        std::ostringstream oss;
        oss << pair.first << ":" << pair.second.metric << ":"
            << pair.second.nextHop << ":" << pair.second.sequence;
        entries.push_back(oss.str());
    }

    update->setDestCount(entries.size());
    update->setRouteEntriesArraySize(entries.size());
    for (size_t i = 0; i < entries.size(); i++) {
        update->setRouteEntries(i, entries[i].c_str());
    }

    routeUpdatesSent++;

    EV << "[节点" << nodeId << "] 广播路由更新 (包含 "
       << entries.size() << " 条路由)\n";

    // 向左右邻居发送
    int leftNeighbor = (nodeId + numNodes - 1) % numNodes;
    int rightNeighbor = (nodeId + 1) % numNodes;

    RouteUpdateMsg *updateLeft = update->dup();
    RouteUpdateMsg *updateRight = update->dup();

    send(updateLeft, "outLeft");
    send(updateRight, "outRight");

    delete update;

    recordScalar("路由更新次数", routeUpdatesSent);
}

// ========== 发送心跳 ==========
void EnhancedNode::sendHeartbeat()
{
    int leftNeighbor = (nodeId + numNodes - 1) % numNodes;
    int rightNeighbor = (nodeId + 1) % numNodes;

    HeartbeatMsg *heartbeatLeft = new HeartbeatMsg();
    heartbeatLeft->setHeartbeatId(heartbeatsSent);
    heartbeatLeft->setSourceNode(nodeId);
    heartbeatLeft->setTargetNode(leftNeighbor);
    heartbeatLeft->setSendTime(simTime());
    heartbeatLeft->setIsAlive(true);

    HeartbeatMsg *heartbeatRight = new HeartbeatMsg();
    heartbeatRight->setHeartbeatId(heartbeatsSent);
    heartbeatRight->setSourceNode(nodeId);
    heartbeatRight->setTargetNode(rightNeighbor);
    heartbeatRight->setSendTime(simTime());
    heartbeatRight->setIsAlive(true);

    heartbeatsSent++;

    EV << "[节点" << nodeId << "] 发送心跳 -> 节点 "
       << leftNeighbor << ", " << rightNeighbor << "\n";

    send(heartbeatLeft, "outLeft");
    send(heartbeatRight, "outRight");

    recordScalar("心跳发送数", heartbeatsSent);
}

// ========== 处理心跳 ==========
void EnhancedNode::processHeartbeat(HeartbeatMsg *heartbeat)
{
    int sourceNode = heartbeat->getSourceNode();
    simtime_t recvTime = simTime();
    bool isAlive = heartbeat->isAlive();

    EV << "[节点" << nodeId << "] 收到来自节点 " << sourceNode
       << " 的心跳\n";

    // 更新最后心跳时间
    lastHeartbeat[sourceNode] = recvTime;

    if (isAlive) {
        aliveNeighbors.insert(sourceNode);
        EV << "    节点 " << sourceNode << " 存活\n";
    }
}

// ========== 检查邻居健康 ==========
void EnhancedNode::checkNeighborHealth()
{
    simtime_t now = simTime();
    double timeoutThreshold = heartbeatInterval * 3;  // 3倍心跳间隔未响应则认为失效

    int leftNeighbor = (nodeId + numNodes - 1) % numNodes;
    int rightNeighbor = (nodeId + 1) % numNodes;

    // 检查左邻居
    if (lastHeartbeat.find(leftNeighbor) != lastHeartbeat.end()) {
        simtime_t elapsed = now - lastHeartbeat[leftNeighbor];
        if (elapsed > timeoutThreshold) {
            if (aliveNeighbors.count(leftNeighbor) > 0) {
                EV << "[节点" << nodeId << "] 检测到链路故障: 左邻居节点 "
                   << leftNeighbor << " 无响应 (" << elapsed << "s)\n";
                aliveNeighbors.erase(leftNeighbor);
                linkFailures++;
                recordScalar("链路故障次数", linkFailures);
            }
        }
    }

    // 检查右邻居
    if (lastHeartbeat.find(rightNeighbor) != lastHeartbeat.end()) {
        simtime_t elapsed = now - lastHeartbeat[rightNeighbor];
        if (elapsed > timeoutThreshold) {
            if (aliveNeighbors.count(rightNeighbor) > 0) {
                EV << "[节点" << nodeId << "] 检测到链路故障: 右邻居节点 "
                   << rightNeighbor << " 无响应 (" << elapsed << "s)\n";
                aliveNeighbors.erase(rightNeighbor);
                linkFailures++;
                recordScalar("链路故障次数", linkFailures);
            }
        }
    }
}

// ========== 检查邻居是否存活 ==========
bool EnhancedNode::isNeighborAlive(int neighborId)
{
    if (!enableLinkMonitoring) return true;
    return aliveNeighbors.count(neighborId) > 0;
}

// ========== 向指定方向发送消息 ==========
void EnhancedNode::sendToDirection(int nextNode, cMessage *msg)
{
    // 计算下一跳相对于当前节点的方向
    int diff = (nextNode - nodeId + numNodes) % numNodes;

    if (diff == 1 || diff == -(numNodes - 1)) {
        // 下一跳在右边（顺时针）
        send(msg, "outRight");
    } else {
        // 下一跳在左边（逆时针）
        send(msg, "outLeft");
    }
}

// ========== 计算下一跳节点（简单路由） ==========
int EnhancedNode::getNextNode(int dest)
{
    int diff = (dest - nodeId + numNodes) % numNodes;

    if (diff > 0 && diff <= numNodes / 2) {
        return (nodeId + 1) % numNodes;
    } else {
        return (nodeId + numNodes - 1) % numNodes;
    }
}

// ========== 判断是否丢包 ==========
bool EnhancedNode::shouldDropPacket()
{
    double rand = uniform(0, 1);
    return rand < dropRate;
}

// ========== 记录统计 ==========
void EnhancedNode::recordStatistics()
{
    recordScalar("发送消息数", messagesSent);
    recordScalar("接收消息数", messagesReceived);
    recordScalar("交付消息数", messagesDelivered);
    recordScalar("丢包数", messagesDropped);
    recordScalar("重传次数", messagesRetransmitted);
    recordScalar("ACK发送数", acksSent);
    recordScalar("ACK接收数", acksReceived);
    recordScalar("路由更新次数", routeUpdatesSent);
    recordScalar("心跳发送数", heartbeatsSent);
    recordScalar("链路故障次数", linkFailures);

    if (delayCount > 0) {
        simtime_t avgDelay = totalDelay / delayCount;
        recordScalar("平均延迟", avgDelay);
    }
}

// ========== 结束 ==========
void EnhancedNode::finish()
{
    EV << "===========================================\n";
    EV << "增强型节点 " << nodeId << " 统计报告\n";
    EV << "===========================================\n";
    EV << "基础统计:\n";
    EV << "  发送消息数: " << messagesSent << "\n";
    EV << "  接收消息数: " << messagesReceived << "\n";
    EV << "  交付消息数: " << messagesDelivered << "\n";
    EV << "  丢包数: " << messagesDropped << "\n";
    EV << "  重传次数: " << messagesRetransmitted << "\n";

    if (delayCount > 0) {
        simtime_t avgDelay = totalDelay / delayCount;
        EV << "  平均延迟: " << avgDelay << "s\n";
    }

    EV << "ACK与重传:\n";
    EV << "  ACK发送数: " << acksSent << "\n";
    EV << "  ACK接收数: " << acksReceived << "\n";
    EV << "  待确认消息: " << pendingMessages.size() << "\n";

    EV << "拥塞控制:\n";
    EV << "  窗口大小: " << currentWindowSize << "\n";
    EV << "  飞行消息: " << messagesInFlight << "\n";

    EV << "路由表:\n";
    EV << "  路由条目数: " << routingTable.size() << "\n";
    EV << "  路由更新次数: " << routeUpdatesSent << "\n";

    EV << "链路监测:\n";
    EV << "  心跳发送数: " << heartbeatsSent << "\n";
    EV << "  链路故障次数: " << linkFailures << "\n";
    EV << "  存活邻居: ";
    for (int n : aliveNeighbors) EV << n << " ";
    EV << "\n";

    EV << "===========================================\n";

    recordStatistics();
}

}; // namespace
