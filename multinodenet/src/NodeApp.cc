// ======================================================================
// 多节点环形网络仿真系统
// MultiNodeNet - 功能增强版 TicToc
//
// 新增功能：
// 1. 多节点环形拓扑（4节点）
// 2. 自定义结构化消息
// 3. 随机丢包模拟
// 4. 端到端延迟统计
// ======================================================================

#include <omnetpp.h>
#include <iostream>
#include "messages_m.h"

using namespace omnetpp;

namespace multinodenet {

class NodeApp : public cSimpleModule
{
  private:
    // 节点参数
    int nodeId;              // 节点唯一标识
    double dropRate;          // 丢包率
    double sendInterval;      // 发送间隔
    int numNodes;             // 节点总数

    // 统计变量
    int messagesSent;         // 发送消息计数
    int messagesReceived;     // 接收消息计数
    int messagesDropped;      // 丢包计数
    simtime_t totalDelay;     // 累计延迟（用于计算平均延迟）
    int delayCount;           // 有效延迟计数

    // 消息ID生成器
    int messageIdCounter;

    // 定时器指针
    cMessage *sendTimer;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 自定义函数
    void sendMessage();                       // 发送消息
    void processReceivedMessage(NetMessage *msg);  // 处理接收的消息
    bool shouldDropPacket();                  // 判断是否丢包
    int getNextNode(int dest);                // 计算下一跳节点
    void sendToDirection(int nextNode, NetMessage *msg);  // 向指定方向发送消息

  public:
    NodeApp();
    virtual ~NodeApp();
};

Define_Module(NodeApp);

// 构造函数
NodeApp::NodeApp()
{
    sendTimer = nullptr;
    messagesSent = 0;
    messagesReceived = 0;
    messagesDropped = 0;
    totalDelay = 0;
    delayCount = 0;
    messageIdCounter = 0;
}

// 析构函数
NodeApp::~NodeApp()
{
    // 取消调度并删除定时器
    if (sendTimer != nullptr) {
        if (sendTimer->isScheduled()) {
            cancelEvent(sendTimer);
        }
        delete sendTimer;
        sendTimer = nullptr;
    }
}

// 初始化函数
void NodeApp::initialize()
{
    // 读取参数
    nodeId = par("nodeId");
    dropRate = par("dropRate");
    sendInterval = par("sendInterval");
    numNodes = par("numNodes");

    // 初始化计数器
    messagesSent = 0;
    messagesReceived = 0;
    messagesDropped = 0;
    totalDelay = 0;
    delayCount = 0;
    messageIdCounter = 0;

    // 打印初始化信息
    EV << "===========================================\n";
    EV << "节点 " << nodeId << " 初始化完成\n";
    EV << "  丢包率: " << (dropRate * 100) << "%\n";
    EV << "  发送间隔: " << sendInterval << "s\n";
    EV << "  邻居节点: " << (nodeId + numNodes - 1) % numNodes
       << " (左), " << (nodeId + 1) % numNodes << " (右)\n";
    EV << "===========================================\n";

    // 节点0作为源节点，在初始化时发送第一条消息
    if (nodeId == 0) {
        EV << "[节点0] 作为源节点，发送第一条消息\n";
        sendMessage();

        // 创建并调度定时器
        sendTimer = new cMessage("sendTimer");
        scheduleAt(simTime() + sendInterval, sendTimer);
    }
}

// 消息处理函数
void NodeApp::handleMessage(cMessage *msg)
{
    // 判断是否是定时器消息
    if (msg == sendTimer) {
        // 定时器到期，发送新消息并重新调度
        sendMessage();
        scheduleAt(simTime() + sendInterval, sendTimer);
    } else {
        // 网络消息
        NetMessage *netMsg = check_and_cast<NetMessage *>(msg);
        processReceivedMessage(netMsg);
    }
}

// 发送消息
void NodeApp::sendMessage()
{
    messageIdCounter++;

    // 创建新消息
    NetMessage *msg = new NetMessage();
    msg->setMessageId(messageIdCounter);
    msg->setSourceNode(nodeId);
    msg->setCreationTime(simTime());

    // 目的节点：随机选择（排除自身）
    int destNode;
    do {
        destNode = intuniform(0, numNodes - 1);
    } while (destNode == nodeId);

    msg->setDestNode(destNode);
    msg->setPayload(("Hello from node " + std::to_string(nodeId)).c_str());

    messagesSent++;

    EV << "[节点" << nodeId << "] 发送消息 #" << messageIdCounter
       << " -> 目的节点 " << destNode << "\n";

    // 计算下一跳并发送
    int nextNode = getNextNode(destNode);
    sendToDirection(nextNode, msg);

    // 更新统计
    recordScalar("发送消息数", messagesSent);
}

// 处理接收到的消息
void NodeApp::processReceivedMessage(NetMessage *msg)
{
    messagesReceived++;

    int srcNode = msg->getSourceNode();
    int destNode = msg->getDestNode();
    simtime_t delay = simTime() - msg->getCreationTime();

    EV << "[节点" << nodeId << "] 收到消息 #" << msg->getMessageId()
       << " (源:" << srcNode << " -> 目的:" << destNode
       << ", 延迟:" << delay << "s)\n";

    // 检查是否是目的节点
    if (destNode == nodeId) {
        // 消息到达目的节点
        EV << "    >>> 消息已到达目的节点！\n";

        // 记录延迟统计
        totalDelay += delay;
        delayCount++;

        recordScalar("接收消息数", messagesReceived);
        recordScalar("有效消息延迟", delay);

        delete msg;
    } else {
        // 需要转发到下一跳
        EV << "    路由中... 目的节点是 " << destNode;

        // 判断是否丢包
        if (shouldDropPacket()) {
            messagesDropped++;
            EV << " [丢包！]\n";
            recordScalar("丢包数", messagesDropped);
            delete msg;
            return;
        }

        // 计算下一跳
        int nextNode = getNextNode(destNode);
        EV << " -> 转发到节点 " << nextNode << "\n";

        // 转发消息（不修改 creationTime）
        sendToDirection(nextNode, msg);
    }
}

// 判断是否丢包
bool NodeApp::shouldDropPacket()
{
    // 使用均匀分布随机数判断是否丢包
    double rand = uniform(0, 1);
    return rand < dropRate;
}

// 计算下一跳节点
int NodeApp::getNextNode(int dest)
{
    // 简单路由：沿环形网络顺时针方向（节点编号递增方向）转发
    // 如果目的节点在当前节点的顺时针方向，则向右转发
    // 否则向左转发

    int diff = (dest - nodeId + numNodes) % numNodes;

    if (diff > 0 && diff <= numNodes / 2) {
        // 目的在右半圆，向右转发
        return (nodeId + 1) % numNodes;
    } else {
        // 目的在左半圆（包括自身），向左转发
        return (nodeId + numNodes - 1) % numNodes;
    }
}

// 向指定方向发送消息
void NodeApp::sendToDirection(int nextNode, NetMessage *msg)
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

// 结束函数
void NodeApp::finish()
{
    EV << "===========================================\n";
    EV << "节点 " << nodeId << " 统计报告\n";
    EV << "===========================================\n";
    EV << "发送消息数: " << messagesSent << "\n";
    EV << "接收消息数: " << messagesReceived << "\n";
    EV << "丢包数: " << messagesDropped << "\n";

    if (delayCount > 0) {
        simtime_t avgDelay = totalDelay / delayCount;
        EV << "平均延迟: " << avgDelay << "s\n";
    }

    EV << "===========================================\n";

    // 记录标量统计数据
    recordScalar("发送消息数", messagesSent);
    recordScalar("接收消息数", messagesReceived);
    recordScalar("丢包数", messagesDropped);
    if (delayCount > 0) {
        recordScalar("平均延迟", totalDelay / delayCount);
    }
}

}; // namespace
