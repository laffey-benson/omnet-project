//
// Txc 简单模块定义
//
simple Txc
{
    parameters:
        int limit = default(10);
        @display("i=block/routing");
    gates:
        input in;
        output out;
}
