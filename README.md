# Group-Chat-Room-Master
基于muduo网络库的集群聊天服务器
运用到Json三方库、muduo网络库、Nginx负载均衡、Redis、MySQL相关知识


项目开发环境：
  Linux Ubuntu20.04 + VScode远程连接；
  
项目使用库：
  1. json库：
  2. muduo库：
  
项目功能需求：
    1. 新用户注册
    2. 用户登录
    3. 添加好友和添加群组
    4. 聊天一对一
    5. 群组聊天
    6. 离线消息保存
    7. 注销业务
    8. nginx配置tcp负载均衡 使得服务器集群、增加可连接的客户端数量
    9. redis订阅集群聊天系统支持客户端跨服务器通信
 
 数据库表设计：
  有user(用户)、Friend(好友)、AllGroup(组)、Groupuser(组员)、offlineMessage(离线消息);
  
  工程目标：
  1. 项目使用cmake生成可执行文件
  2. 文件规范规定：bin-可执行文件、lib-生成的中间的库文件、include-头文件、src-源文件、build-编译过程中产生的临时中间文件、test-测试文件、thirdparty第三方库文件；
  
