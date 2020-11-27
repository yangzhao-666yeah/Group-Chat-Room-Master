#ifndef REDIS_H
#define REDIS_H

#include <thread>
#include <functional>

#include <hiredis/hiredis.h>

using namespace std;

//创建redis类
class Redis
{
public:
    Redis();
    ~Redis();

    //连接redis服务器
    bool connect();

    //像redis订阅的频道channel发布消息
    bool publish(int channel,string message);

    //向指定的redis通道订阅消息
    bool subscribe(int channel);

    //取消订阅的消息
    bool unsubscribe(int channel);

    //响应通道内发生的消息
    void observer_channel_message();

    //通道内有消息 上报消息
    void init_notify_handler(function<void(int,string)>fn);
    
private:
    /*
        集群服务器之间相互聊天 需要借助redis消息队列
        1.服务器要在消息队列中订阅感兴趣的时间
        2.redis收到感兴趣消息后要向服务器返回消息
        3.redis需要接收到特定数字的消息后 在redis中发布消息 publish
        */
    //发布消息
    redisContext *_publish_context;

    //订阅消息
    redisContext *_subcribe_context;

    //收到感兴趣的消息 向服务器返回消息  回调
    function<void(int, string)> _notify_message_handler;
};

#endif