#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QTcpSocket>

class MyClient : public QTcpSocket {
  Q_OBJECT

public:
  MyClient(QObject *parent = nullptr);
  ~MyClient();

  bool start(const QString &hostname, quint16 port, int msecs = 30000);
};


#endif