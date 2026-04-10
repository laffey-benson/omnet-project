//
// 扩展网络消息定义
// SmartNet 仿真系统使用
//
// 新增功能：
// 1. 消息优先级（高/中/低）
// 2. 消息类型区分（数据/控制）
// 3. 跳数计数（防止无限循环）
// 4. 确认机制支持
//

message SmartMessage {
    int messageId = 0;              // 消息唯一标识
    int sourceNode = -1;             // 源节点编号
    int destNode = -1;               // 目的节点编号
    int priority = 1;                // 优先级：0=低, 1=中, 2=高
    int messageType = 0;             // 消息类型：0=数据, 1=ACK确认, 2=路由更新
    simtime_t creationTime;          // 消息创建时间（用于计算延迟）
    int hopCount = 0;                // 跳数计数（防止无限循环）
    int maxHops = 10;                // 最大跳数限制
    bool requireAck = false;        // 是否需要确认
    string payload = "";             // 消息内容负载
}
