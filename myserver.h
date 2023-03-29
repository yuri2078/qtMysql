#ifndef MYSERVER_H

#define MYSERVER_H

#include <QTcpServer>
#include <qtcpserver.h>
#include <QTcpSocket>
#include <QDataStream>
#include <QMap>

class MyServer :public QTcpServer {
  Q_OBJECT

public:
  QString last_error;
  MyServer(QObject *parent = nullptr);
  ~MyServer();

  quint16 getUsers();
  const QString getMsg(QTcpSocket *socket);
  bool start(QHostAddress host = QHostAddress::Any, quint16 port = 2078);

private:
  
  QMap<QTcpSocket *, QString> messages;
  bool is_servering;
  void newClient();
  void receiveData();
};



#endif