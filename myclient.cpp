#include "myclient.h"
#include <qabstractsocket.h>

MyClient::MyClient(QObject *parent) : QTcpSocket(parent) {
  // 连接报错处理
  connect(this, &MyClient::errorOccurred, this, &MyClient::onSocketError);
  is_start = false;
  last_error = "目前没有发生错误!";
}

MyClient::~MyClient() {}

bool MyClient::start(const QString &hostname, quint16 port, int msecs) {
  if (is_start) {
    disconnectFromHost();
  }
  connectToHost(hostname, port); // 连接host
  is_start = waitForConnected(msecs);
  if (is_start) {
    qDebug() << "连接 " << hostname << ":" << port << " 成功捏!";
    last_error = "连接 " + hostname + ": " + QString::number(port) + " 成功!";
    return true;
  } else {
    return false;
  }

  return true;
}

quint16 MyClient::write(const QByteArray &data) {
  if (state() == MyClient::ConnectedState && MyClient::isWritable()) {
    return QTcpSocket::write(data);
  }
  return -1;
}

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