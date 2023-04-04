#include "../include/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "../include/mylineedit.h"
#include <QDateTime>
#include <QDesktopServices>
#include <QPicture>
#include <QtNetwork/QtNetwork>
#include <qabstractsocket.h>
#include <qpushbutton.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>

// 打印日志宏
#define info_label_log(str)                                                    \
  ui->info_label->insertPlainText(                                             \
      QDateTime::currentDateTime().toString("hh:mm:ss  ") + str + "\n");

MainWindow::MainWindow(QWidget *parent, QSqlDatabase *db)
    : QMainWindow(parent), ui(new Ui::MainWindow), db(db),
      server(new MyServer(this)), client(new MyClient(this)) {
  ui->setupUi(this);

  ui->stackedWidget->setCurrentIndex(0); // 设置初始界面是 主页


  // 推出登陆信号
  connect(ui->logout_button, &QPushButton::clicked, [this]() {
    emit loginEnd();
  });

  // 搜索信号
  connect(ui->search_button, &QPushButton::clicked, [this]() {
    QDesktopServices::openUrl(QUrl("https://www.bing.com/search?q=" + ui->search_edit->text()));
    
  });

  init(); // 固定初始化

  
}

MainWindow::~MainWindow() {
  delete ui;
}

// 重写关闭窗口,关闭自动退出程序
void MainWindow::closeEvent(QCloseEvent *event) {
  qApp->exit();
  QWidget::closeEvent(event);
}

// 初始化操作
bool MainWindow::init() {
  setIcons(); // 设置各种图标
  setChangeButton(); // 设置切换按钮特效
  setChangePage(); // 设置切换界面
  sendMessageInit(); // 客户端发送消息初始化

  return false;
}


void MainWindow::setIcons() {

  // 设置一些图片 根据label 控件显示
  QPixmap pixmap;
  pixmap.load(":/images/logo.png");
  ui->logo->setPixmap(pixmap.scaled(ui->logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  
  pixmap.load(":/images/avatar.png");
  ui->avatar_label->setPixmap(pixmap.scaled(ui->avatar_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  pixmap.load(":/images/icons/notification.png");
  ui->notification_label->setPixmap(
  pixmap.scaled(ui->notification_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  pixmap.load(":/images/icons/github.png");
  ui->label_1->setPixmap(pixmap.scaled(ui->label_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  pixmap.load(":/images/icons/bilibili.png");
  ui->label_2->setPixmap(pixmap.scaled(ui->label_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  pixmap.load(":/images/icons/qq.png");
  ui->label_3->setPixmap(pixmap.scaled(ui->label_3->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  pixmap.load(":/images/icons/wechat.png");
  ui->label_4->setPixmap(pixmap.scaled(ui->label_4->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  pixmap.load(":/images/left_bg.jpg");
  ui->left_bg->setPixmap(pixmap.scaled(ui->left_bg->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  pixmap.load(":/images/search.png");
  ui->search_logo->setPixmap(pixmap);

  // 设置一些图标
  ui->home_button->setIcon(QIcon(":/images/icons/首页.png"));
  ui->note_button->setIcon(QIcon(":/images/icons/便签.png"));
  ui->mysql_button->setIcon(QIcon(":/images/icons/日历.png"));
  ui->adv_button->setIcon(QIcon(":/images/icons/转账.png"));
  ui->table_to_sql_button->setIcon(QIcon(":/images/icons/合作.png"));
  ui->attachment_button->setIcon(QIcon(":/images/icons/attachment.png"));
  ui->button_1->setIcon(QIcon(":/images/icons/attachment.png"));
  ui->button_2->setIcon(QIcon(":/images/icons/attachment.png"));
  ui->button_3->setIcon(QIcon(":/images/icons/attachment.png"));
  ui->button_4->setIcon(QIcon(":/images/icons/attachment.png"));
  ui->search_icon->setIcon(QIcon(":/images/icons/search.png"));
}

// 设置切换到别的按钮是，另外的按钮处于默认状态
void MainWindow::setChangeButton() {
  connect(ui->home_button, &QPushButton::clicked, [this]() {
    ui->mysql_button->setChecked(false);
    ui->adv_button->setChecked(false);
    ui->table_to_sql_button->setChecked(false);
    ui->note_button->setChecked(false);
  });

  connect(ui->note_button, &QPushButton::clicked, [this]() {
    ui->mysql_button->setChecked(false);
    ui->adv_button->setChecked(false);
    ui->table_to_sql_button->setChecked(false);
    ui->home_button->setChecked(false);
  });

  connect(ui->mysql_button, &QPushButton::clicked, [this]() {
    ui->note_button->setChecked(false);
    ui->adv_button->setChecked(false);
    ui->table_to_sql_button->setChecked(false);
    ui->home_button->setChecked(false);
  });

  connect(ui->adv_button, &QPushButton::clicked, [this]() {
    ui->note_button->setChecked(false);
    ui->mysql_button->setChecked(false);
    ui->table_to_sql_button->setChecked(false);
    ui->home_button->setChecked(false);
  });

  connect(ui->table_to_sql_button, &QPushButton::clicked, [this]() {
    ui->note_button->setChecked(false);
    ui->adv_button->setChecked(false);
    ui->mysql_button->setChecked(false);
    ui->home_button->setChecked(false);
  });
}


// 设置根据按钮切换到不同的界面
void MainWindow::setChangePage() {
  
  connect(ui->home_button, &QPushButton::clicked, [this]() {
    ui->stackedWidget->setCurrentIndex(0); // 主页
  });

  connect(ui->note_button, &QPushButton::clicked, [this]() {
    ui->stackedWidget->setCurrentIndex(1); // 笔记
  });

  connect(ui->mysql_button, &QPushButton::clicked, [this]() {
    ui->stackedWidget->setCurrentIndex(2); // mysql 测试
  });

  connect(ui->adv_button, &QPushButton::clicked, [this]() {
    ui->stackedWidget->setCurrentIndex(3); // c/s 测试
  });
}

// 客户端/服务端初始化
void MainWindow::sendMessageInit() {
  // 客户端连接按钮初始化
  // 如果没有连接就进行连接，默认 本地 2078 端口
  // 如果已经连接了，就断开连接
  connect(ui->connect_client, &QPushButton::clicked, [this]() {
    if (client->is_start) {
      client->end(); // 调用end 结束连接
      info_label_log("客户端已经关闭!")
      ui->connect_client->setChecked(false); // 设置按钮为未连接状态
    } else {
      client->start("127.0.0.1", 2078);
      ui->connect_client->setChecked(client->is_start);
      info_label_log(client->last_error);
    }
  });

  // 服务端连接按钮初始化
  // 如果没有连接就进行连接，默认 监听本地 2078 端口
  // 如果已经连接了，就断开连接
  connect(ui->connect_server, &QPushButton::clicked, [this]() {
    if (server->is_servering) {
      server->end();
      ui->connect_server->setChecked(false);
      info_label_log("服务端端已经关闭!")
    } else {
      server->start();
      ui->connect_server->setChecked(server->is_servering);
      info_label_log(server->last_error);
    }
  });


  // 两个发送消息框定义， 他们都重写回车按钮，按下回车机会发送消息
  auto send_client = new MyLineEdit(ui->frame);
  send_client->setPlaceholderText("给服务器发送消息!");

  // 发送消息，如果发送失败就打印日志
  connect(send_client, &MyLineEdit::sendMessage, [this, send_client]() {
    if (this->client->write(send_client->text().toUtf8()) == 0) {
      info_label_log(client->last_error);
    }
    send_client->setText("");
  });

  // 设置位置
  send_client->move(60, 40);
  send_client->resize(500, 50);

  auto send_server = new MyLineEdit(ui->frame);
  send_server->setPlaceholderText("给客户端发送消息!");
  connect(send_server, &MyLineEdit::sendMessage, [this, send_server]() {
    if (server->write(client, send_server->text().toUtf8()) == 0) {
      info_label_log(server->last_error)
    }
     send_server->setText("");
  });

  // 打印接受到消息
  connect(client, &MyClient::readyRead, [this]() {
    info_label_log("服务端 -> " + client->last_msg)
  });

  connect(server, &MyServer::readyRead, [this]() {
    info_label_log("客户端 -> " + server->getMsg(client))
  });

  send_server->move(60, 160);
  send_server->resize(500, 50);

  // 将server 服务端与client 客户端连接，接受到新连接就将他门重新连接
  connect(server, &MyServer::newUser, client, &MyClient::setUser);
}
// void MainWindow::saveFile() {
//     QString str =
//     QFileDialog::getSaveFileName(this,"请打开一个文件","/home/yuri","");
//     this->ui->stackedWidget->show();
//     this->ui->stackedWidget->setCurrentIndex(0);
//     QFile file(str,this);
//     file.open(QFile::ReadWrite);
//     if(file.isOpen()){
//         QString new_txt = ui->textEdit->toPlainText();
//         file.write(new_txt.toUtf8());
//     } else {
//         qDebug() << "打开失败 -> " << file.error();
//     }
//     file.close();
// }
//
// void MainWindow::openFile() {
//     QString str =
//     QFileDialog::getOpenFileName(this,"请打开一个文件","/home/yuri","");
//     this->ui->stackedWidget->show();
//     this->ui->stackedWidget->setCurrentIndex(0);
//     QFile file(str,this);
//     file.open(QFile::ReadWrite);
//     if(file.isOpen()){
//         auto file_txt = file.readAll();
//         ui->textEdit->setText(file_txt);
//     } else {
//         qDebug() << "打开失败 -> " << file.error();
//     }
//
//     file.close();
// }
//    connect(ui->open_file, &QAction::triggered, this,
//    &MainWindow::openFile);
//
//    connect(ui->kurseni_file, &QAction::triggered, this,
//    &MainWindow::saveFile);
//
//    connect(ui->go_back,&QAction::triggered, [this]() {
//        emit this->loginEnd();
//    });
//
//    connect(this->ui->search_data, &QAction::triggered,[this](){
//        this->ui->stackedWidget->show();
//        this->ui->stackedWidget->setCurrentIndex(1);
//    });
//
//    this->ui->table_view->hide();
//    this->ui->stackedWidget->close();
//
//    connect(ui->button_search, &QPushButton::clicked, [this](){
//        if(this->db->open()) {
//            this->db->setDatabaseName("miku");
//            QSqlQuery query;
//            if(!query.exec(this->ui->edit_search->text())) {
//                // 数据库查询失败，弹出错误提示框
//                QMessageBox::critical(this, "错误", "查询数据库时发生错误："
//                + query.lastError().text());
//            } else {
//                model.setQuery(this->ui->edit_search->text());
//                // 将查询结果显示在TableView控件中
//                ui->table_view->setModel(&model);
//                this->ui->table_view->show();
//            }
//
//        }else{
//            qDebug() << "打开失败!";
//        }
//    });
//
//    connect(ui->create_file,&QAction::triggered,[this](){
//        this->ui->stackedWidget->show();
//        this->ui->stackedWidget->setCurrentIndex(0);
//    });