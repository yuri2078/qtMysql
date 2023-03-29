#include "myserver.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent) {
  is_servering = false;
}

// 当一个新的客户端连接请求到达时，为其创建新的 QTcpSocket

void MyServer::newClient() {
  QTcpSocket *socket = nextPendingConnection();
  connect(socket, &QTcpSocket::readyRead, this, &MyServer::receiveData);
  messages.insert(socket, "");
  qDebug() << "New client connected: " << socket->peerAddress().toString() << ":" << socket->peerPort();
}

// 当客户端发来消息时，接收并处理
void MyServer::receiveData() {
  QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
  messages[socket] = socket->readAll();
  qDebug() << "当前读取的数据 -> " << messages[socket];
}

quint16 MyServer::getUsers() { return messages.size(); }

const QString MyServer::getMsg(QTcpSocket *socket) {
  return messages.value(socket);
}

bool MyServer::start(QHostAddress host, quint16 port) {
  if (listen(host, port)) {
    qDebug() << "Server listening on port " << serverPort();
    connect(this, &MyServer::newConnection, this, &MyServer::newClient);
  } else {
    return false;
  }
  return true;
}
MyServer::~MyServer() {
  for (auto begin = messages.begin(); begin != messages.end(); begin++) {
    qDebug() << "信息 -> " << begin.value();
  }
  close();
}

// QTcpSocket *socket = new QTcpSocket(this);

// connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));

// void MyClass::onSocketError(QAbstractSocket::SocketError socketError)
// {
//     switch (socketError) {
//         case QAbstractSocket::ConnectionRefusedError:
//             qDebug() << "连接被拒绝.";
//             break;
//         case QAbstractSocket::RemoteHostClosedError:
//             qDebug() << "远程主机关闭了连接.";
//             break;
//         case QAbstractSocket::HostNotFoundError:
//             qDebug() << "主机未找到.";
//             break;
//         case QAbstractSocket::SocketAccessError:
//             qDebug() << "套接字访问错误.";
//             break;
//         case QAbstractSocket::SocketResourceError:
//             qDebug() << "套接字资源错误.";
//             break;
//         case QAbstractSocket::SocketTimeoutError:
//             qDebug() << "套接字超时错误.";
//             break;
//         case QAbstractSocket::DatagramTooLargeError:
//             qDebug() << "数据报太大错误.";
//             break;
//         case QAbstractSocket::NetworkError:
//             qDebug() << "网络错误.";
//             break;
//         case QAbstractSocket::AddressInUseError:
//             qDebug() << "地址正在使用错误.";
//             break;
//         case QAbstractSocket::SocketAddressNotAvailableError:
//             qDebug() << "套接字地址不可用错误.";
//             break;
//         case QAbstractSocket::UnsupportedSocketOperationError:
//             qDebug() << "不支持的套接字操作错误.";
//             break;
//         case QAbstractSocket::UnfinishedSocketOperationError:
//             qDebug() << "未完成的套接字操作错误.";
//             break;
//         case QAbstractSocket::ProxyAuthenticationRequiredError:
//             qDebug() << "代理身份验证失败错误.";
//             break;
//         case QAbstractSocket::SslHandshakeFailedError:
//             qDebug() << "SSL握手失败错误.";
//             break;
//         case QAbstractSocket::ProxyConnectionRefusedError:
//             qDebug() << "代理连接被拒绝错误.";
//             break;
//         case QAbstractSocket::ProxyConnectionClosedError:
//             qDebug() << "代理连接关闭错误.";
//             break;
//         case QAbstractSocket::ProxyConnectionTimeoutError:
//             qDebug() << "代理连接超时错误.";
//             break;
//         case QAbstractSocket::ProxyNotFoundError:
//             qDebug() << "代理未找到错误.";
//             break;
//         case QAbstractSocket::ProxyProtocolError:
//             qDebug() << "代理协议错误.";
//             break;
//         case QAbstractSocket::UnknownSocketError:
//         default:
//             qDebug() << "未知的套接字错误.";
//             break;
//     }
// }
