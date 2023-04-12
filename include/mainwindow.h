#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./myclient.h"
#include "./myserver.h"
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <qprocess.h>

/*
登陆成功之后的，窗口主界面

*/

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent, QSqlDatabase *db);
  ~MainWindow();

  //    void saveFile();
  //    void openFile();

  // 初始化函数 对各种数据进行初始化
  bool init();

  void closeEvent(QCloseEvent *event) override;

signals:
  void loginEnd(); // 登陆结束窗口

private:
  Ui::MainWindow *ui; // ui文件
  QSqlDatabase *db; // 上面登陆的数据库指针
  QSqlQueryModel model;


  MyServer *server; // 服务端
  MyClient *client; // 客户端

// 各种初始化
  void setIcons();
  void setChangeButton();
  void setChangePage();
  void sendMessageInit();
  void dataBaseInit();
  
};
#endif // MAINWINDOW_H
