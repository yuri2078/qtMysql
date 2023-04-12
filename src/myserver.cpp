#include "../include/myserver.h"

MyServer::MyServer(QObject *parent) :
  QTcpServer(parent) {
  last_error = "初始化";
  is_servering = false;
}

// 析构函数，调用end 函数，释放资源
MyServer::~MyServer() {
  end();
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
  emit readyRead();
  // qDebug() << "来自客户端 -> " << messages[socket];
}

// 通过myclient 返回数据
const QString MyServer::getMsg(MyClient *socket) {
  if (user.find(socket) == user.end()) {
    last_error = "没有对应的tcpSocket 连接";
    return "";
  }
  return messages.value(user[socket]);
}

// 监听指定ip地址的端口，默认监听任意ip地址的 2078 端口
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

// 结束所有连接，并且释放所有资源
void MyServer::end() {
  disconnect(this, &MyServer::newConnection, this, &MyServer::newClient); // 断开连接
  is_servering = false;

  for (auto client : user) {
    disconnect(client, &QTcpSocket::readyRead, this, &MyServer::receiveData); // 断开连接
    client->close();                                                          // 关闭连接
  }

  user.clear();     // 清空用户
  messages.clear(); // 清空消息
  close();
}

// 向指定的myclient 发送数据
quint16 MyServer::write(MyClient *client, const QByteArray &data) {
  if (is_servering == false) {
    last_error = "服务端未在运行!";
    return 0;
  }

  if (user.find(client) != user.end()) {
    return user[client]->write(data);
  } else {
    last_error = "没有对应用户";
  }
  return 0;
}

void MyServer::insertUser(MyClient *client, QTcpSocket *socket) {
  if (user.find(client) == user.end()) {
    user[client] = socket;
  }
}
