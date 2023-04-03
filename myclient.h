#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QTcpSocket>

class MyClient : public QTcpSocket {
  Q_OBJECT

public:
  MyClient(QObject *parent = nullptr);
  QString last_error;
  QString last_msg;
  ~MyClient();

  bool is_start;

  bool start(const QString &hostname, quint16 port, int msecs = 30000);
  quint16 write(const QByteArray &data);

  void setUser(QTcpSocket *);

private:
  void onSocketError(QAbstractSocket::SocketError socketError);
};

#endif