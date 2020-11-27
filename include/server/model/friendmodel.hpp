#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "user.hpp"
#include <vector>

using namespace std;

//提供用户好友信息管理
class FriendModel
{
public:
    //添加好友
    void insert(int userid,int friendid);
    //返回用户好友列表 id - name
    vector<User> query(int userid);

};

#endif
