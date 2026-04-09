//
// 简单的 TicToc 网络 - 两个节点互相交换消息
//
network TicTocNet
{
    parameters:
        @display("bgb=200,150");
    submodules:
        tic: Txc {
            @display("i=block/routing");
        }
        toc: Txc {
            @display("i=block/routing");
        }
    connections:
        tic.out --> toc.in;
        toc.out --> tic.in;
}
