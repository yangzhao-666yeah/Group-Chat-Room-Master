#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include "json.hpp"
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

#include "usermodel.hpp"
#include "offlinemessage.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"

//表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

//业务
class ChatService
{
public:
    //获取单例对象的接口函数
    static ChatService *instance();
    //处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //聊天业务 1对1
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    //服务器异常 业务重置方法
    void reset();
    //添加好友id
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //添加群组
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //加入群组
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //群聊天
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);

private:
    //单例设计
    ChatService();

    //存储id和其对应的业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;

    //存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    //定义互斥锁 保证userconnmap的线程安全
    mutex _connMutex;

    //数据操作类
    UserModel _userModel;                 //用户
    OfflineMessageModel _offlineMsgModel; //离线消息
    FriendModel _friendModel;             //好友类
    GroupModel _groupModel;               //群组操作

    // redis操作对象
    Redis _redis;
};

#endif