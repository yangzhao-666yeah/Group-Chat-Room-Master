#include "ChatServer.hpp"
#include "chatservice.hpp"

#include <functional>
#include <iostream>
#include <string>
#include "json.hpp"
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

//服务器连接 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    //函数连接回调绑定
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    //函数读写绑定
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    //创建工作线程
    _server.setThreadNum(4);
}
//启动服务
void ChatServer::start()
{
    _server.start();
}

//处理事件连接回调
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        //连接不成功 客户端断开连接
        //调用一个方法 反馈给客户端 同时服务端要将此用户改为离线
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}
//处理事件读写
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    // 测试，添加json打印代码
    cout << buf << endl;
    // 数据的反序列化
    json js = json::parse(buf);
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"] 获取=》业务handler=》conn  js  time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}