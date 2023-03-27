#include "myserver.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent) {
  
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
  messages[socket] = socket->readAll();
  qDebug() << "当前读取的数据 -> " << messages[socket];
}

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

bool MyServer::start(QHostAddress host, quint16 port) {
  if (listen(host, port)) {
    qDebug() << "Server listening on port " << serverPort();
    return false;
  } else {
    connect(this, &MyServer::newConnection, this, &MyServer::newClient);
  }
    
  return true;
}