//
// 节点模块定义 - 环形网络中的通信节点
// 支持左右邻居通信，具有丢包模拟和延迟统计功能
//
simple NodeApp
{
    parameters:
        int nodeId = default(0);              // 节点唯一标识（0-3）
        double dropRate = default(0.05);      // 丢包率（0-1之间）
        volatile double sendInterval = default(1.0); // 消息发送间隔（秒）
        int numNodes = default(4);            // 网络中节点总数
        @display("i=block/routing");          // 显示图标样式

    gates:
        // 左邻居通信端口
        input inLeft;
        output outLeft;
        // 右邻居通信端口
        input inRight;
        output outRight;
}
