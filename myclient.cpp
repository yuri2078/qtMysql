#include "myclient.h"
#include <qabstractsocket.h>

MyClient::MyClient(QObject *parent) : QTcpSocket(parent) {
  
}

MyClient::~MyClient() {
  
}

bool MyClient::start(const QString &hostname, quint16 port, int msecs) {
  connectToHost(hostname, port); // 连接host
  if (waitForConnected(msecs)) {
    qDebug() << "连接 " << hostname << " " << port << " 成功!";
    return true;
  } else {
    qDebug() << "连接 " << hostname << " " << port << " 失败捏!";
    return false;
  }

  write("sdf");

  return true;
}