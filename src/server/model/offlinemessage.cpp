#include "offlinemessage.hpp"
#include "db.h"

//存储用户离线接收到的消息
void OfflineMessageModel::insert(int userid, string msg)
{
    //获取sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage values(%d,'%s')", userid, msg.c_str());

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}

//用户登录后 将消息从离线消息表中删除
void OfflineMessageModel::remove(int userid)
{
    //获取sql语句
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userid=%d", userid);

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}

//查询该用户是否有离线消息
vector<string> OfflineMessageModel::query(int userid)
{
    //获取sql语句
    char sql[1024] = {0};
    sprintf(sql, "select message from offlinemessage where userid = %d", userid);

    vector<string> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        //获取mysql中语句
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            //获取查询到的信息  有多个信息 多行
            MYSQL_ROW row = mysql_fetch_row(res) ;
            while (row != nullptr)
            {
                 //把userid用户的所有离线消息放入vec中
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
            return vec;
        
        }
    }
    return vec;
}