#include "myserver.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent) {
  is_servering = false;
}

// 当一个新的客户端连接请求到达时，为其创建新的 QTcpSocket

void MyServer::newClient() {
  QTcpSocket *socket = nextPendingConnection();
  connect(socket, &QTcpSocket::readyRead, this, &MyServer::receiveData);
  emit newUser(socket);

  messages.insert(socket, "");
  last_error = "New client connected: " + socket->peerAddress().toString() + ":" + socket->peerPort();
}

// 当客户端发来消息时，接收并处理
void MyServer::receiveData() {
  QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
  messages[socket] = socket->readAll();
  qDebug() << "来自客户端 -> " << messages[socket];
}

quint16 MyServer::getUsers() { return messages.size(); }

const QString MyServer::getMsg(QTcpSocket *socket) {
  return messages.value(socket);
}


bool MyServer::start(QHostAddress host, quint16 port) {
  is_servering = listen(host, port);
  if (is_servering) {
    last_error = "Server listening on port " + QString::number(serverPort());
    connect(this, &MyServer::newConnection, this, &MyServer::newClient);
  } else {
    last_error = "Server listening on port " + QString::number(serverPort()) + " is error!";
  }
  return is_servering;
}

quint16 MyServer::write(MyClient *client, const QByteArray &data) {
  if (user.find(client) != user.end()) {
    return user[client]->write(data);
  } else {
    qDebug() << "没有对应用户";
  }
  
  return -1;
}

void MyServer::insertUser(MyClient *client, QTcpSocket * socket) {
  if (user.find(client) == user.end()) {
    user[client] = socket;
  }
}

MyServer::~MyServer() {
  for (auto begin = messages.begin(); begin != messages.end(); begin++) {
    qDebug() << "信息 -> " << begin.value();
  }
  close();
}

// QTcpSocket *socket = new QTcpSocket(this);


