#ifndef OFFLINEMESSAGE_H
#define OFFLINEMESSAGE_H

#include <string>
#include <vector>
using namespace std;

//离线消息表的操作类（接口）
//用于实现离线消息的存储

class OfflineMessageModel
{
public:
    //存储用户离线接收到的消息
    void insert(int userid, string msg);

    //用户登录后 将消息从离线消息表中删除
    void remove(int userid);

    //查询该用户是否有离线消息
    vector<string> query(int userid);
};

#endif
