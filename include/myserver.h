#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <qtcpserver.h>
#include <QTcpSocket>
#include <QDataStream>
#include <QMap>
#include "./myclient.h"


/*
自定义tcpServer 类
实现功能 ：
  start : 监听一个端口，并且返回是否监听成功，默认监听任意ip 的 2078 端口
  end : 断开连接，释放资源
  write : 向已经存储的客户端发送消息，如果不存在就报错
  getMsg : 从myclient 中获取最新接受的信息
*/

class MyServer :public QTcpServer {
  Q_OBJECT

public:
  friend class MyClient;
  MyServer(QObject *parent = nullptr);
  ~MyServer();

  quint16 write(MyClient *client, const QByteArray &data);
  const QString getMsg(MyClient *socket);
  bool start(QHostAddress host = QHostAddress::Any, quint16 port = 2078);
  void end();

  bool is_servering;
  QString last_error;
  
signals:

  void newUser(QTcpSocket *); // 接受到新的连接的信号
  void readyRead(); // 能够接受数据信号
  
private:
  QMap<QTcpSocket *, QString> messages; // 保存所有tcpsocket 信息
  QMap<MyClient *, QTcpSocket *> user; // 保存myclient 对应的socket 信息

  void insertUser(MyClient *, QTcpSocket *); // 将myclient 插入到用户列表中
  
  void newClient(); // 新的连接处理函数
  void receiveData(); // 获取数据处理函数
};
#endif