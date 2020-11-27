#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    //服务器连接 初始化聊天服务器对象
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);
    //启动服务
    void start();

private:
    //处理事件连接回调
    void onConnection(const TcpConnectionPtr &);
    //处理事件读写
    void onMessage(const TcpConnectionPtr &,
                   Buffer *,
                   Timestamp);
    //服务器
    TcpServer _server;
    //时间循环指针
    EventLoop *_loop;
};

#endif