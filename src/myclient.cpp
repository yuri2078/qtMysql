#include "../include/myclient.h"
#include "../include/myserver.h"
#include <qabstractsocket.h>

MyClient::MyClient(QObject *parent) : QTcpSocket(parent) {
  // 连接报错处理
  connect(this, &MyClient::errorOccurred, this, &MyClient::onSocketError);

  // 设置默认值
  is_start = false;
  last_error = "目前没有发生错误!";
}

// 析构默认调用end函数
MyClient::~MyClient() {
  end();
}

// 开启连接，默认等待 30000ms
bool MyClient::start(const QString &hostname, quint16 port, int msecs) {
  if (is_start) {
    disconnectFromHost(); // 如果之前连接了，就先断开连接
  }

  connectToHost(hostname, port); // 连接host
  is_start = waitForConnected(msecs);
  if (is_start) {
    // 如果能够读取就读取数据并存如last_msg
    last_error = "连接 " + hostname + ": " + QString::number(port) + " 成功!";
    connect(this, &QTcpSocket::readyRead, [this]() {
      last_msg = this->readAll();
      emit this->readyRead();
    });
  }
  return is_start;
}

// 向服务器发送数据，返回发送成功数据大小，否则返回 0
quint16 MyClient::write(const QByteArray &data) {
  if (state() == MyClient::ConnectedState && MyClient::isWritable()) {
    return QTcpSocket::write(data);
  }
  last_error = "还没到可写状态";
  return 0;
}

// 结束连接函数
void MyClient::end() {
  is_start = false;
  last_error = "客户端断开连接!";
  last_msg = "";
  close();
}
/*  slots 函数 */


// 将MyClient * 绑定到对方的socket 中，方便在外部通过myclient 对服务端发送消息
void MyClient::setUser(QTcpSocket *socket) {
  MyServer *server = qobject_cast<MyServer *>(sender());
  server->insertUser(this, socket);
}

// 报错slots 处理
void MyClient::onSocketError(QAbstractSocket::SocketError socketError) {
  switch (socketError) {
  case QAbstractSocket::ConnectionRefusedError:
    last_error = "连接被拒绝!\n";
    break;
  case QAbstractSocket::RemoteHostClosedError:
    last_error = "远程主机关闭了连接!\n";
    break;
  case QAbstractSocket::HostNotFoundError:
    last_error = "主机未找到!\n";
    break;
  case QAbstractSocket::SocketAccessError:
    last_error = "套接字访问错误!\n";
    break;
  case QAbstractSocket::SocketResourceError:
    last_error = "套接字资源错误!\n";
    break;
  case QAbstractSocket::SocketTimeoutError:
    last_error = "套接字超时错误!\n";
    break;
  case QAbstractSocket::DatagramTooLargeError:
    last_error = "数据报太大错误!\n";
    break;
  case QAbstractSocket::NetworkError:
    last_error = "网络错误!\n";
    break;
  case QAbstractSocket::AddressInUseError:
    last_error = "地址正在使用错误!\n";
    break;
  case QAbstractSocket::SocketAddressNotAvailableError:
    last_error = "套接字地址不可用错误!\n";
    break;
  case QAbstractSocket::UnsupportedSocketOperationError:
    last_error = "不支持的套接字操作错误!\n";
    break;
  case QAbstractSocket::UnfinishedSocketOperationError:
    last_error = "未完成的套接字操作错误!\n";
    break;
  case QAbstractSocket::ProxyAuthenticationRequiredError:
    last_error = "代理身份验证失败错误!\n";
    break;
  case QAbstractSocket::SslHandshakeFailedError:
    last_error = "SSL握手失败错误!\n";
    break;
  case QAbstractSocket::ProxyConnectionRefusedError:
    last_error = "代理连接被拒绝错误!\n";
    break;
  case QAbstractSocket::ProxyConnectionClosedError:
    last_error = "代理连接关闭错误!\n";
    break;
  case QAbstractSocket::ProxyConnectionTimeoutError:
    last_error = "代理连接超时错误!\n";
    break;
  case QAbstractSocket::ProxyNotFoundError:
    last_error = "代理未找到错误!\n";
    break;
  case QAbstractSocket::ProxyProtocolError:
    last_error = "代理协议错误!\n";
    break;
  case QAbstractSocket::UnknownSocketError:
  default:
    last_error = "未知的套接字错误!\n";
    break;
  }
  qDebug() << last_error;
}