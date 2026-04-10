// ======================================================================
// SmartNet 智能网络仿真系统
// SmartNet - 增强版多节点网络仿真
//
// 新增功能（相比 MultiNodeNet）：
// 1. 网格+环形混合拓扑（6节点）
// 2. 优先级消息队列
// 3. 动态拥塞控制
// 4. 跳数限制机制
// 5. 多路径路由选择
// ======================================================================

#include <omnetpp.h>
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include "messages_m.h"

using namespace omnetpp;

namespace smartnet {

// 优先级消息结构
struct PriorityMessage {
    SmartMessage* msg;
    int priority;  // 0=低, 1=中, 2=高
    simtime_t arrivalTime;

    bool operator<(const PriorityMessage& other) const {
        // 优先级队列：高的优先（所以取反）
        if (priority != other.priority)
            return priority > other.priority;  // 大优先
        return arrivalTime > other.arrivalTime;  // 早的先出
    }
};

// 链路状态结构
struct LinkState {
    int neighbor;
    bool active;
    simtime_t lastUpdate;
    double reliability;
};

class SmartNode : public cSimpleModule
{
  private:
    // 节点参数
    int nodeId;
    double dropRate;
    double sendInterval;
    int numNodes;
    int queueSize;
    int congestionThreshold;
    double congestionDropRate;
    int maxHops;
    bool enablePriority;
    bool enableCongestionControl;

    // 统计变量
    int messagesSent;
    int messagesReceived;
    int messagesDelivered;
    int messagesDropped;
    int messagesForwarded;
    simtime_t totalDelay;
    int delayCount;
    int congestionEvents;

    // 消息ID生成器
    int messageIdCounter;

    // 定时器
    cMessage *sendTimer;

    // 优先级队列（优先级调度）
    std::priority_queue<PriorityMessage> priorityQueue;

    // 链路状态表（用于动态路由）
    std::map<int, LinkState> linkTable;

    // 队列长度统计
    cOutVector queueLengthVector;

    // 拥塞状态
    bool isCongested;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 核心功能函数
    void sendMessage();
    void processReceivedMessage(SmartMessage *msg);
    bool shouldDropPacket(SmartMessage *msg);
    int selectNextHop(int dest);
    void sendToNeighbor(int neighbor, SmartMessage *msg);
    void enqueueMessage(SmartMessage *msg);
    SmartMessage* dequeueMessage();
    void updateLinkState(int neighbor, bool active);
    void checkCongestion();
    void sendAck(int originalMsgId, int dest);
    void forwardMessage(SmartMessage *msg);

  public:
    SmartNode();
    virtual ~SmartNode();
};

Define_Module(SmartNode);

// 构造函数
SmartNode::SmartNode()
{
    sendTimer = nullptr;
    messagesSent = 0;
    messagesReceived = 0;
    messagesDelivered = 0;
    messagesDropped = 0;
    messagesForwarded = 0;
    totalDelay = 0;
    delayCount = 0;
    congestionEvents = 0;
    messageIdCounter = 0;
    isCongested = false;
}

// 析构函数
SmartNode::~SmartNode()
{
    if (sendTimer != nullptr) {
        if (sendTimer->isScheduled()) {
            cancelEvent(sendTimer);
        }
        delete sendTimer;
    }

    // 清空优先级队列
    while (!priorityQueue.empty()) {
        delete priorityQueue.top().msg;
        priorityQueue.pop();
    }
}

// 初始化函数
void SmartNode::initialize()
{
    // 读取参数
    nodeId = par("nodeId");
    dropRate = par("dropRate");
    sendInterval = par("sendInterval");
    numNodes = par("numNodes");
    queueSize = par("queueSize");
    congestionThreshold = par("congestionThreshold");
    congestionDropRate = par("congestionDropRate");
    maxHops = par("maxHops");
    enablePriority = par("enablePriority").boolValue();
    enableCongestionControl = par("enableCongestionControl").boolValue();

    // 初始化计数器
    messagesSent = 0;
    messagesReceived = 0;
    messagesDelivered = 0;
    messagesDropped = 0;
    messagesForwarded = 0;
    totalDelay = 0;
    delayCount = 0;
    congestionEvents = 0;
    messageIdCounter = 0;
    isCongested = false;

    // 初始化链路状态表
    // 根据环形拓扑，初始化邻居连接
    int leftNeighbor = (nodeId + numNodes - 1) % numNodes;
    int rightNeighbor = (nodeId + 1) % numNodes;
    int upperNeighbor = -1, lowerNeighbor = -1;

    // 特殊拓扑处理
    if (numNodes >= 6) {
        if (nodeId == 0) { upperNeighbor = 2; lowerNeighbor = 4; }
        else if (nodeId == 1) { upperNeighbor = 3; lowerNeighbor = 5; }
        else if (nodeId == 2) { upperNeighbor = 3; lowerNeighbor = 4; }
        else if (nodeId == 5) { upperNeighbor = 1; lowerNeighbor = 3; }
    }

    linkTable[leftNeighbor] = {leftNeighbor, true, simTime(), 1.0 - dropRate};
    linkTable[rightNeighbor] = {rightNeighbor, true, simTime(), 1.0 - dropRate};
    if (upperNeighbor >= 0)
        linkTable[upperNeighbor] = {upperNeighbor, true, simTime(), 0.95};
    if (lowerNeighbor >= 0)
        linkTable[lowerNeighbor] = {lowerNeighbor, true, simTime(), 0.95};

    // 打印初始化信息
    EV << "===========================================\n";
    EV << "节点 " << nodeId << " 初始化完成\n";
    EV << "  丢包率: " << (dropRate * 100) << "%\n";
    EV << "  发送间隔: " << sendInterval << "s\n";
    EV << "  队列大小: " << queueSize << "\n";
    EV << "  拥塞阈值: " << congestionThreshold << "\n";
    EV << "  启用优先级: " << (enablePriority ? "是" : "否") << "\n";
    EV << "  启用拥塞控制: " << (enableCongestionControl ? "是" : "否") << "\n";
    EV << "  邻居节点: ";
    for (auto& pair : linkTable) {
        EV << pair.first << " ";
    }
    EV << "\n===========================================\n";

    // 节点0作为源节点，发送第一条消息
    if (nodeId == 0) {
        EV << "[节点" << nodeId << "] 作为源节点，发送第一批消息\n";
        // 发送多条不同优先级的消息
        sendMessage();
        sendMessage();
        sendMessage();

        // 创建并调度定时器
        sendTimer = new cMessage("sendTimer");
        scheduleAt(simTime() + sendInterval, sendTimer);
    }
}

// 消息处理函数
void SmartNode::handleMessage(cMessage *msg)
{
    // 判断是否是定时器消息
    if (msg == sendTimer) {
        // 定时器到期，发送新消息并重新调度
        sendMessage();
        scheduleAt(simTime() + sendInterval, sendTimer);
    } else {
        // 网络消息
        SmartMessage *smartMsg = check_and_cast<SmartMessage *>(msg);
        processReceivedMessage(smartMsg);
    }

    // 每次处理消息后检查队列长度
    checkCongestion();
    queueLengthVector.record((double)priorityQueue.size());
}

// 发送消息
void SmartNode::sendMessage()
{
    messageIdCounter++;

    // 创建新消息
    SmartMessage *msg = new SmartMessage();
    msg->setMessageId(messageIdCounter);
    msg->setSourceNode(nodeId);
    msg->setCreationTime(simTime());
    msg->setHopCount(0);
    msg->setMaxHops(maxHops);

    // 随机选择目的节点（排除自身）
    int destNode;
    do {
        destNode = intuniform(0, numNodes - 1);
    } while (destNode == nodeId);
    msg->setDestNode(destNode);

    // 随机选择优先级（模拟不同类型流量）
    int priority = 0;
    double rand = uniform(0, 1);
    if (rand < 0.2) {
        priority = 2;  // 20% 高优先级
    } else if (rand < 0.6) {
        priority = 1;  // 40% 中优先级
    } else {
        priority = 0;  // 40% 低优先级
    }
    msg->setPriority(priority);

    // 消息类型
    msg->setMessageType(0);  // 数据消息
    msg->setRequireAck(uniform(0, 1) < 0.3);  // 30%需要确认

    // 设置负载
    std::string priorityStr;
    switch (priority) {
        case 2: priorityStr = "HIGH"; break;
        case 1: priorityStr = "MEDIUM"; break;
        default: priorityStr = "LOW"; break;
    }
    msg->setPayload(("Data from node " + std::to_string(nodeId) +
                    " [P:" + priorityStr + "]").c_str());

    messagesSent++;

    EV << "[节点" << nodeId << "] 发送消息 #" << messageIdCounter
       << " -> 目的节点 " << destNode
       << " [优先级:" << priorityStr << "]"
       << "\n";

    // 直接发送或入队
    if (enablePriority && priorityQueue.size() >= (size_t)queueSize) {
        // 队列满，根据优先级决定是否丢弃
        if (priority < 2) {
            // 低/中优先级消息被丢弃
            messagesDropped++;
            EV << "    [队列已满，丢弃低优先级消息]\n";
            delete msg;
            return;
        }
    }

    enqueueMessage(msg);

    // 如果队列中有消息，尝试转发
    if (!priorityQueue.empty()) {
        SmartMessage* toSend = dequeueMessage();
        if (toSend) {
            forwardMessage(toSend);
        }
    }
}

// 入队消息
void SmartNode::enqueueMessage(SmartMessage *msg)
{
    PriorityMessage pm;
    pm.msg = msg;
    pm.priority = msg->getPriority();
    pm.arrivalTime = simTime();
    priorityQueue.push(pm);

    EV << "    [入队] 队列大小: " << priorityQueue.size() << "/" << queueSize << "\n";
}

// 出队消息
SmartMessage* SmartNode::dequeueMessage()
{
    if (priorityQueue.empty()) {
        return nullptr;
    }

    PriorityMessage pm = priorityQueue.top();
    priorityQueue.pop();
    return pm.msg;
}

// 处理接收到的消息
void SmartNode::processReceivedMessage(SmartMessage *msg)
{
    messagesReceived++;

    int srcNode = msg->getSourceNode();
    int destNode = msg->getDestNode();
    int hopCount = msg->getHopCount();
    simtime_t delay = simTime() - msg->getCreationTime();

    EV << "[节点" << nodeId << "] 收到消息 #" << msg->getMessageId()
       << " (源:" << srcNode << " -> 目的:" << destNode
       << ", 跳数:" << hopCount
       << ", 延迟:" << delay << "s)\n";

    // 检查是否是目的节点
    if (destNode == nodeId) {
        // 消息到达目的节点
        EV << "    >>> 消息已到达目的节点！\n";

        messagesDelivered++;

        // 记录延迟统计
        totalDelay += delay;
        delayCount++;

        // 如果需要确认，发送ACK
        if (msg->getRequireAck()) {
            sendAck(msg->getMessageId(), srcNode);
        }

        recordScalar("成功投递消息数", messagesDelivered);
        recordScalar("消息延迟", delay);
        recordScalar("总跳数", hopCount);

        delete msg;
    } else {
        // 需要转发
        // 检查跳数限制
        if (msg->getHopCount() >= msg->getMaxHops()) {
            EV << "    [跳数超限，丢弃消息]\n";
            messagesDropped++;
            delete msg;
            return;
        }

        // 检查是否丢包
        if (shouldDropPacket(msg)) {
            messagesDropped++;
            delete msg;
            return;
        }

        // 转发消息
        msg->setHopCount(msg->getHopCount() + 1);
        forwardMessage(msg);
    }
}

// 转发消息（选择最佳下一跳）
void SmartNode::forwardMessage(SmartMessage *msg)
{
    int dest = msg->getDestNode();

    // 选择下一跳
    int nextHop = selectNextHop(dest);

    if (nextHop >= 0) {
        EV << "    路由中... 选择下一跳: 节点 " << nextHop << "\n";
        messagesForwarded++;
        sendToNeighbor(nextHop, msg);
    } else {
        EV << "    [无可用路由，丢弃消息]\n";
        messagesDropped++;
        delete msg;
    }
}

// 选择最佳下一跳（动态路由）
int SmartNode::selectNextHop(int dest)
{
    // 动态路由策略：选择最优路径
    // 考虑因素：
    // 1. 链路可靠性
    // 2. 距离（跳数）

    int bestHop = -1;
    double bestScore = -1;

    // 遍历所有邻居
    for (auto& pair : linkTable) {
        int neighbor = pair.first;
        LinkState& state = pair.second;

        if (!state.active) continue;

        // 计算距离：环形网络中的距离
        int distForward = (neighbor - nodeId + numNodes) % numNodes;
        int distBackward = (nodeId - neighbor + numNodes) % numNodes;
        int minDist = std::min(distForward, distBackward);

        // 简单的距离向量：选择使目的节点更近的邻居
        int newDistForward = (dest - neighbor + numNodes) % numNodes;
        int newDistBackward = (neighbor - dest + numNodes) % numNodes;
        int newMinDist = std::min(newDistForward, newDistBackward);

        // 如果经过此邻居能到达目的节点
        if (newMinDist < minDist || (newMinDist == 0 && neighbor == dest)) {
            // 计算分数：考虑可靠性和距离
            double score = state.reliability * 100 - newMinDist * 10;

            if (score > bestScore) {
                bestScore = score;
                bestHop = neighbor;
            }
        }
    }

    // 如果没有找到更好的路径，选择任意可用邻居
    if (bestHop < 0) {
        for (auto& pair : linkTable) {
            if (pair.second.active) {
                bestHop = pair.first;
                break;
            }
        }
    }

    return bestHop;
}

// 发送到指定邻居
void SmartNode::sendToNeighbor(int neighbor, SmartMessage *msg)
{
    // 根据邻居方向选择端口
    // 这里简化处理，使用简单的左右方向

    int diff = (neighbor - nodeId + numNodes) % numNodes;

    // 判断方向（基于简单的规则）
    if (diff > 0 && diff <= numNodes / 2) {
        // 向右
        send(msg, "outRight");
    } else {
        // 向左
        send(msg, "outLeft");
    }
}

// 判断是否丢包（智能丢包策略）
bool SmartNode::shouldDropPacket(SmartMessage *msg)
{
    double currentDropRate = dropRate;

    // 如果启用拥塞控制，根据拥塞状态调整丢包率
    if (enableCongestionControl && isCongested) {
        currentDropRate = congestionDropRate;

        // 低优先级消息更容易被丢弃
        if (msg->getPriority() == 0) {
            currentDropRate = std::min(1.0, currentDropRate * 1.5);
        }
    }

    // 优先级保护：高优先级消息几乎不会被随机丢弃
    if (msg->getPriority() == 2) {
        currentDropRate *= 0.1;  // 高优先级丢包率降低90%
    }

    double rand = uniform(0, 1);
    return rand < currentDropRate;
}

// 检查拥塞状态
void SmartNode::checkCongestion()
{
    int currentQueueSize = priorityQueue.size();

    bool wasCongested = isCongested;

    if (currentQueueSize >= congestionThreshold) {
        if (!isCongested) {
            isCongested = true;
            congestionEvents++;
            EV << "[节点" << nodeId << "] >>> 触发拥塞控制！队列长度:" << currentQueueSize << "\n";
        }
    } else if (currentQueueSize < congestionThreshold / 2) {
        // 队列长度降到阈值一半以下，解除拥塞
        if (isCongested) {
            isCongested = false;
            EV << "[节点" << nodeId << "] >>> 拥塞解除！队列长度:" << currentQueueSize << "\n";
        }
    }

    if (isCongested != wasCongested) {
        recordScalar("拥塞事件", congestionEvents);
    }
}

// 发送确认消息
void SmartNode::sendAck(int originalMsgId, int dest)
{
    SmartMessage *ack = new SmartMessage();
    ack->setMessageId(-originalMsgId);  // 用负数表示ACK
    ack->setSourceNode(nodeId);
    ack->setDestNode(dest);
    ack->setMessageType(1);  // ACK类型
    ack->setPriority(2);  // 高优先级
    ack->setHopCount(0);
    ack->setPayload(("ACK for msg " + std::to_string(originalMsgId)).c_str());

    EV << "[节点" << nodeId << "] 发送ACK #" << originalMsgId << " -> 节点 " << dest << "\n";

    int nextHop = selectNextHop(dest);
    if (nextHop >= 0) {
        sendToNeighbor(nextHop, ack);
    } else {
        delete ack;
    }
}

// 结束函数
void SmartNode::finish()
{
    EV << "===========================================\n";
    EV << "节点 " << nodeId << " 统计报告\n";
    EV << "===========================================\n";
    EV << "发送消息数: " << messagesSent << "\n";
    EV << "接收消息数: " << messagesReceived << "\n";
    EV << "成功投递数: " << messagesDelivered << "\n";
    EV << "转发消息数: " << messagesForwarded << "\n";
    EV << "丢包数: " << messagesDropped << "\n";
    EV << "拥塞事件: " << congestionEvents << "\n";

    if (delayCount > 0) {
        simtime_t avgDelay = totalDelay / delayCount;
        EV << "平均延迟: " << avgDelay << "s\n";
        EV << "投递成功率: " << (messagesDelivered * 100.0 / messagesSent) << "%\n";
    }

    EV << "===========================================\n";

    // 记录标量统计数据
    recordScalar("发送消息数", messagesSent);
    recordScalar("接收消息数", messagesReceived);
    recordScalar("成功投递数", messagesDelivered);
    recordScalar("转发消息数", messagesForwarded);
    recordScalar("丢包数", messagesDropped);
    recordScalar("拥塞事件数", congestionEvents);

    if (delayCount > 0) {
        recordScalar("平均延迟", totalDelay / delayCount);
        recordScalar("投递成功率", messagesDelivered * 100.0 / messagesSent);
    }
}

}; // namespace
