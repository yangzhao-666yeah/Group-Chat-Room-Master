#include "redis.hpp"

#include <iostream>
using namespace std;

Redis::Redis():_publish_context(nullptr),_subcribe_context(nullptr)
{
}

Redis::~Redis()
{
    if(_publish_context!=nullptr)
    {
        redisFree(_publish_context);
    }
    if(_publish_context!=nullptr)
    {
        redisFree(_subcribe_context);
    }
}

//连接redis服务器
bool Redis::connect()
{
    //连接redis的端口号和本地的服务器  负责的是发布消息模块
    _publish_context=redisConnect("127.0.0.1",6379);
    if(_publish_context==nullptr)
    {
        cerr<<"connect redis failed!"<<endl;
        return  false;
    }

    //连接redis的端口号和本地的服务器  负责的是接收消息模块
    _subcribe_context=redisConnect("127.0.0.1",6379);
    if(_subcribe_context==nullptr)
    {
        cerr<<"connect redis failed!"<<endl;
        return  false;
    }   

    //创建单独的线程 负责监听通道内的消息 有消息之后给业务层进行上报
    
    //subcrivbe接收特定通道后 会一直堵塞 直到publish在通道内发布消息
    thread t([&]()
    {
        observer_channel_message();
    });
    //回收线程
    t.detach();

    cout<<"connect redis connect!"<<endl;
    return true;
}

//像redis订阅的频道channel发布消息
bool Redis::publish(int channel, string message)
{
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBLISH %d %s", channel, message.c_str());
    if (nullptr == reply)
    {
        cerr << "publish command failed!" << endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

//向指定的redis通道订阅消息
bool Redis::subscribe(int channel)
{
    // SUBSCRIBE命令本身会造成线程阻塞等待通道里面发生消息，这里只做订阅通道，不接收通道消息
    // 通道消息的接收专门在observer_channel_message函数中的独立线程中进行
    // 只负责发送命令，不阻塞接收redis server响应消息，否则和notifyMsg线程抢占响应资源
    if (REDIS_ERR == redisAppendCommand(this->_subcribe_context, "SUBSCRIBE %d", channel))
    {
        cerr << "subscribe command failed!" << endl;
        return false;
    }
    // redisBufferWrite可以循环发送缓冲区，直到缓冲区数据发送完毕（done被置为1）
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(this->_subcribe_context, &done))
        {
            cerr << "subscribe command failed!" << endl;
            return false;
        }
    }
    // redisGetReply

    return true;
}

//取消订阅的消息
bool Redis::unsubscribe(int channel)
{
    if (REDIS_ERR == redisAppendCommand(this->_subcribe_context, "UNSUBSCRIBE %d", channel))
    {
        cerr << "unsubscribe command failed!" << endl;
        return false;
    }
    // redisBufferWrite可以循环发送缓冲区，直到缓冲区数据发送完毕（done被置为1）
    int done = 0;
    while (!done)
    {
        if (REDIS_ERR == redisBufferWrite(this->_subcribe_context, &done))
        {
            cerr << "unsubscribe command failed!" << endl;
            return false;
        }
    }
    return true;
}

//响应通道内发生的消息
void Redis::observer_channel_message()
{
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(this->_subcribe_context, (void **)&reply))
    {
        // 订阅收到的消息是一个带三元素的数组
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr)
        {
            // 给业务层上报通道上发生的消息
            _notify_message_handler(atoi(reply->element[1]->str) , reply->element[2]->str);
        }

        freeReplyObject(reply);
    }

    cerr << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<" << endl;
}

//通道内有消息 上报消息
void Redis::init_notify_handler(function<void(int, string)> fn)
{
    this->_notify_message_handler = fn;
}