//
// SmartNet 网络拓扑定义
// 6节点环形网络
//
// 网络拓扑：
//        node[1] -------- node[2]
//           |                |
//           |                |
//        node[0] -------- node[3]
//           |                |
//           |                |
//        node[5] -------- node[4]
//
network SmartNet
{
    parameters:
        int numNodes = default(6);

    submodules:
        node[6]: SmartNode {
            nodeId = index;
            numNodes = 6;
            @display("i=block/routing;p=60,60");
        }

    connections:
        // 环形连接 - 顺时针方向
        // 上层连接（节点0-1-2-3）
        node[0].outRight --> node[1].inLeft;
        node[1].outRight --> node[2].inLeft;
        node[2].outRight --> node[3].inLeft;

        // 下层连接（节点0-5-4-3）
        node[0].outLeft --> node[5].inRight;
        node[5].outLeft --> node[4].inRight;
        node[4].outLeft --> node[3].inRight;

        // 侧向连接（节点1-5, 节点2-4）
        node[1].outLeft --> node[5].inRight { @display("ls=,55,55"); }
        node[2].outLeft --> node[4].inRight { @display("ls=,55,55"); }
        node[5].outRight --> node[1].inLeft { @display("ls=,55,55"); }
        node[4].outRight --> node[2].inLeft { @display("ls=,55,55"); }

        // 跨越连接（形成网状结构）
        node[0].outRight --> node[2].inLeft { @display("ls=dashed,55,55"); }
        node[1].outRight --> node[3].inLeft { @display("ls=dashed,55,55"); }
        node[5].outLeft --> node[3].inRight { @display("ls=dashed,55,55"); }
}
