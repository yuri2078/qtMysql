#include "myserver.h"

MyServer::MyServer(QObject *parent, quint16 port) : QTcpServer(parent) {
  connect(this, &MyServer::newConnection, this, &MyServer::newClient);
  if (listen(QHostAddress::Any, port))
    qDebug() << "Server listening on port " << serverPort();
}

// 当一个新的客户端连接请求到达时，为其创建新的 QTcpSocket
void MyServer::newClient() {
  QTcpSocket *socket = nextPendingConnection();
  connect(socket, &QTcpSocket::readyRead, this, &MyServer::receiveData);
  messages.insert(socket, "");

  qDebug() << "New client connected: " << socket->peerAddress().toString()
           << ":" << socket->peerPort();
}

// 当客户端发来消息时，接收并处理
void MyServer::receiveData() {
  QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
  QString &str = messages[socket];
  str = socket->readAll();

  qDebug() << "当前读取的数据 -> " << str;
}

void MyServer::processMessage(QTcpSocket *socket, QDataStream &data) {}

quint16 MyServer::getUsers() { return messages.size(); }

MyServer::~MyServer() {
  for (auto begin = messages.begin(); begin != messages.end(); begin++) {
    qDebug() << "信息 -> " << begin.value();
  }
  close();
}

const QString MyServer::getMsg(QTcpSocket *socket) {
  return messages.value(socket);
}
