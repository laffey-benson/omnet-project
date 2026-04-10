//
// EnhancedNetMsg - 增强型网络消息
// 相比原版新增：TTL、跳数计数、序列号、重传标志
//
message EnhancedNetMsg {
    int messageId = 0;              // 消息唯一标识
    int sourceNode = -1;             // 源节点编号
    int destNode = -1;               // 目的节点编号
    int sequenceNum = 0;             // 序列号（用于重传检测）
    simtime_t creationTime;          // 消息创建时间
    int ttl = 10;                    // 生存时间（防止路由循环）
    int hopCount = 0;                // 已跳数计数
    bool retransmission = false;     // 是否为重传消息
    string payload = "";             // 消息内容负载
}

//
// AckMessage - 确认消息
// 用于可靠性传输：目的节点收到数据后回复ACK
//
message AckMessage {
    int ackId = 0;                  // ACK唯一标识
    int sourceNode = -1;             // ACK发送者（目的节点）
    int destNode = -1;               // ACK接收者（源节点）
    int sequenceNum = 0;             // 被确认的消息序列号
    simtime_t sendTime;              // ACK发送时间
}

//
// RouteUpdateMsg - 路由更新消息
// 用于路由表维护：节点间交换路由信息
//
message RouteUpdateMsg {
    int updateId = 0;                // 更新消息ID
    int sourceNode = -1;             // 发送更新的节点
    simtime_t updateTime;            // 更新时间
    int destCount = 0;               // 路由条目数量

    // 路由信息数组（动态大小，由destCount决定）
    // 格式：destNode:metric:nextHop:sequnce
    string routeEntries[];           // 路由条目列表
}

//
// HeartbeatMsg - 心跳消息
// 用于链路监测：检测邻居节点和链路状态
//
message HeartbeatMsg {
    int heartbeatId = 0;             // 心跳ID
    int sourceNode = -1;             // 发送心跳的节点
    int targetNode = -1;             // 目标邻居节点
    simtime_t sendTime;              // 发送时间
    bool isAlive = true;             // 节点是否存活
}
