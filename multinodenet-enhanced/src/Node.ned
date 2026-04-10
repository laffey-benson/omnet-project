//
// EnhancedNode - 增强型网络节点
// 支持：确认重传、拥塞控制、路由表维护、链路监测
//
// 拓扑结构示意：
//              left        right
//            ←──────       ──────→
//        0 ◄──────────────────► 1
//        ▲                       ▲
//        │      ◄──────►        │
//        │                       │
//        ▼                       ▼
//        3 ◄──────────────────► 2
//            ◄──────       ──────→
//              right       left
//
simple EnhancedNode
{
    parameters:
        int nodeId = default(0);              // 节点唯一标识
        double dropRate = default(0.05);      // 丢包率（0-1之间）
        volatile double sendInterval = default(1.0s); // 消息发送间隔
        int numNodes = default(4);           // 网络中节点总数
        int windowSize = default(3);          // 拥塞控制窗口大小
        int maxRetransmit = default(3);       // 最大重传次数
        double ackTimeout = default(2.0);     // ACK超时时间（秒）
        double heartbeatInterval = default(5.0); // 心跳间隔（秒）
        double routeUpdateInterval = default(10.0); // 路由更新间隔（秒）
        bool enableCongestionControl = default(true); // 是否启用拥塞控制
        bool enableLinkMonitoring = default(true);    // 是否启用链路监测
        @display("i=block/routing");          // 显示图标样式

    gates:
        // 左邻居通信端口
        input inLeft;
        output outLeft;
        // 右邻居通信端口
        input inRight;
        output outRight;
        // ACK专用端口（复用同一通道，通过消息类型区分）
        // 为了简化设计，ACK和普通数据共用同一端口
}
