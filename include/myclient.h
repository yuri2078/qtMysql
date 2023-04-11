#ifndef MYCLIENT_H
#define MYCLIENT_H

/*
自定义socket 类
实现功能 ：
  start : 连接到指定端口，并关联读取信息函数
  end : 断开连接，释放资源
  write : 向服务端发送数据
  setUser : 关联this 和 对面 socket
*/

#include <QTcpSocket>

class MyClient : public QTcpSocket {
  Q_OBJECT

public:
  MyClient(QObject *parent = nullptr);
  ~MyClient();

  QString last_error; // 最近的一条错误
  QString last_msg; // 最近接收到的一条消息
  
  bool is_start; // 是否已经成功连接到服务端

  bool start(const QString &hostname, quint16 port, int msecs = 30000);
  void end();
  quint16 write(const QByteArray &data);
  
  /* slots */
  
  void setUser(QTcpSocket *);

signals:
  void readyRead();



private:
  /* signals */

  void onSocketError(QAbstractSocket::SocketError socketError);
};

#endif