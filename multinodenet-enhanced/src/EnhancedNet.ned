@namespace(multinodenet_enhanced);
// 拓扑结构：
//              left        right
//            ←──────       ──────→
//        0 ◄─────────────────────► 1
//        ▲                           ▲
//        │      ◄──────────►         │
//        │                           │
//        │      ◄──────────►         │
//        ▼                           ▼
//        5 ◄─────────────────────► 2
//        ▲                           ▲
//        │      ◄──────────►         │
//        ▼                           ▼
//        4 ◄─────────────────────► 3
//            ◄──────       ──────→
//              right       left
//
// 6节点环形网络，消息沿环形传递，根据目的节点选择最短路径
// 支持动态路由表维护和链路故障检测
//
network EnhancedNet
{
    parameters:
        int numNodes = default(6);              // 节点数量
        @display("bgb=600,400");                // 画布大小

    submodules:
        // 6个通信节点
        node[6]: EnhancedNode {
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

        // 环形连接：node[3] <-> node[4]
        node[3].outRight --> { delay = 100ms; } --> node[4].inLeft;
        node[4].outLeft --> { delay = 100ms; } --> node[3].inRight;

        // 环形连接：node[4] <-> node[5]
        node[4].outRight --> { delay = 100ms; } --> node[5].inLeft;
        node[5].outLeft --> { delay = 100ms; } --> node[4].inRight;

        // 环形连接：node[5] <-> node[0]
        node[5].outRight --> { delay = 100ms; } --> node[0].inLeft;
        node[0].outLeft --> { delay = 100ms; } --> node[5].inRight;
}
