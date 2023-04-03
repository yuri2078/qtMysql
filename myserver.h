#ifndef MYSERVER_H

#define MYSERVER_H

#include <QTcpServer>
#include <qtcpserver.h>
#include <QTcpSocket>
#include <QDataStream>
#include <QMap>
#include "myclient.h"

class MyServer :public QTcpServer {
  Q_OBJECT

public:
  friend class MyClient;
  QString last_error;
  MyServer(QObject *parent = nullptr);
  ~MyServer();

  quint16 getUsers();
  quint16 write(MyClient *client, const QByteArray &data);
  const QString getMsg(QTcpSocket *socket);
  bool start(QHostAddress host = QHostAddress::Any, quint16 port = 2078);
  bool is_servering;

signals:
  
  void newUser(QTcpSocket *);

private:
  QMap<QTcpSocket *, QString> messages;
  QMap<MyClient *, QTcpSocket *> user;

  void insertUser(MyClient *, QTcpSocket *);

  void newClient();
  void receiveData();
  
  
};



#endif