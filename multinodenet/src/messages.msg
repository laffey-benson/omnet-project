//
// 自定义网络消息定义
// 包含消息ID、源节点、目的节点、创建时间和负载内容
//

message NetMessage {
    int messageId = 0;           // 消息唯一标识
    int sourceNode = -1;          // 源节点编号
    int destNode = -1;            // 目的节点编号
    simtime_t creationTime;       // 消息创建时间（用于计算端到端延迟）
    string payload = "";          // 消息内容负载
}
