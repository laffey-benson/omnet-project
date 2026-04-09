//
// 4节点环形网络拓扑定义
//
// 拓扑结构：
//         node[1]
//            ↑
//            │
//     ←────── ───────→
// node[0]           node[2]
//     ←────── ───────→
//            ↓
//         node[3]
//
// 消息沿环形传递，根据目的节点选择最短路径
//
network MultiNodeNet
{
    parameters:
        int numNodes = default(4);              // 节点数量
        @display("bgb=400,300");                 // 画布大小

    submodules:
        // 4个通信节点
        node[4]: NodeApp {
            @display("i=block/routing");
        }

    connections:
        // 环形连接：node[0] <-> node[1]
        node[0].outRight --> { delay = 100ms; } --> node[1].inLeft;
        node[1].outLeft --> { delay = 100ms; } --> node[0].inRight;

        // 环形连接：node[1] <-> node[2]
        node[1].outRight --> { delay = 100ms; } --> node[2].inLeft;
        node[2].outLeft --> { delay = 100ms; } --> node[1].inRight;

        // 环形连接：node[2] <-> node[3]
        node[2].outRight --> { delay = 100ms; } --> node[3].inLeft;
        node[3].outLeft --> { delay = 100ms; } --> node[2].inRight;

        // 环形连接：node[3] <-> node[0]
        node[3].outRight --> { delay = 100ms; } --> node[0].inLeft;
        node[0].outLeft --> { delay = 100ms; } --> node[3].inRight;
}
