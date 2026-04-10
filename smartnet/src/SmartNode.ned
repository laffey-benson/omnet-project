//
// 智能节点模块定义 - SmartNet 网络中的通信节点
//
// 新增功能：
// 1. 优先级队列缓存
// 2. 拥塞检测与控制
// 3. 消息确认机制
//
simple SmartNode
{
    parameters:
        int nodeId = default(0);              // 节点唯一标识（0-5）
        double dropRate = default(0.03);      // 基础丢包率（0-1之间）
        volatile double sendInterval = default(1.0); // 消息发送间隔（秒）
        int numNodes = default(6);            // 网络中节点总数
        int queueSize = default(20);          // 消息队列缓存大小
        int congestionThreshold = default(15); // 拥塞阈值（超过此值触发拥塞控制）
        double congestionDropRate = default(0.5); // 拥塞时的丢包率
        int maxHops = default(10);            // 最大跳数限制
        bool enablePriority = default(true);  // 是否启用优先级调度
        bool enableCongestionControl = default(true); // 是否启用拥塞控制
        @display("i=block/routing");
        @statisticQueueLength(queueLength, record=vector);

    gates:
        // 左邻居通信端口
        input inLeft;
        output outLeft;
        // 右邻居通信端口
        input inRight;
        output outRight;
}
