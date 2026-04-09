// ======================================================================
// Part of the OMNeT++/OMNEST Discrete System Simulation System
//
// Txc 简单模块实现
// 两个节点互相交换"tic"和"toc"消息
// ======================================================================

#include <omnetpp.h>

using namespace omnetpp;

namespace tictoc {

class Txc : public cSimpleModule
{
  private:
    int counter;  // 消息计数器
    int limit;    // 最大消息数
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(Txc);

void Txc::initialize()
{
    // 初始化计数器
    counter = 0;
    
    // 从参数读取最大消息数，默认10
    limit = par("limit");
    
    // tic 节点在初始化时发送第一条消息
    if (strcmp(getName(), "tic") == 0) {
        EV << "Tic 模块初始化，发送第一条消息\n";
        cMessage *msg = new cMessage("tic");
        send(msg, "out");
        counter = 1;
    }
}

void Txc::handleMessage(cMessage *msg)
{
    counter++;
    EV << "收到消息: " << msg->getName() << " (计数: " << counter << ")\n";

    if (counter >= limit) {
        EV << "达到消息数量限制，删除消息并结束\n";
        delete msg;
        return;
    }

    // 交换消息名称: tic <-> toc
    if (strcmp(msg->getName(), "tic") == 0) {
        msg->setName("toc");
    } else {
        msg->setName("tic");
    }

    EV << "发送消息: " << msg->getName() << "\n";
    send(msg, "out");
}

void Txc::finish()
{
    EV << getName() << " 模块共收到 " << counter << " 条消息\n";
    recordScalar("收到的消息数", counter);
}

}; // namespace
