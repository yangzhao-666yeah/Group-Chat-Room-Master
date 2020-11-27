#include "ChatServer.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>

using namespace std;

//处理ctrl+c 换成重置user的状态信息
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main()
{

    signal(SIGINT,resetHandler);
    EventLoop loop;
    InetAddress addr("127.0.0.1",6000);

    ChatServer server(&loop,addr,"Chat");

    server.start();
    loop.loop();
    return 0;
}