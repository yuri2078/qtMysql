#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mylineedit.h"
#include <QDateTime>
#include <QDesktopServices>
#include <QPicture>
#include <QtNetwork/QtNetwork>
#include <qabstractsocket.h>
#include <qpushbutton.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>

#define info_label_log(str)                                                    \
  ui->info_label->insertPlainText(                                             \
      QDateTime::currentDateTime().toString("hh:mm:ss  ") + str + "\n");

MainWindow::MainWindow(QWidget *parent, QSqlDatabase *db)
    : QMainWindow(parent), ui(new Ui::MainWindow), db(db),
      server(new MyServer(this)), client(new MyClient(this)) {
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  connect(ui->logout_button, &QPushButton::clicked,
          [this]() { emit loginEnd(); });

  init();

  connect(ui->connect_client, &QPushButton::clicked, [this]() {
    if (client->is_start) {
      info_label_log("客户端已经启动!")
          ui->connect_client->setChecked(server->is_servering);
      return;
    }

    client->start("127.0.0.1", 2078);
    ui->connect_client->setChecked(client->is_start);
    info_label_log(client->last_error);
  });

  connect(ui->connect_server, &QPushButton::clicked, [this]() {
    if (server->is_servering) {
      ui->connect_server->setChecked(server->is_servering);
      info_label_log("服务端端已经启动!") return;
    }
    server->start();
    ui->connect_server->setChecked(server->is_servering);
    info_label_log(server->last_error);
  });

  connect(ui->search_button, &QPushButton::clicked, [this]() {
    QDesktopServices::openUrl(
        QUrl("https://www.bing.com/search?q=" + ui->search_edit->text()));
  });

  auto send_client = new MyLineEdit(ui->frame);
  send_client->setPlaceholderText("给服务器发送消息!");
  
  connect(send_client, &MyLineEdit::sendMessage, [this, send_client]() {
    this->client->write(send_client->text().toUtf8());
    send_client->setText("");
  });
  send_client->move(60, 40);
  send_client->resize(500, 50);

  auto send_server = new MyLineEdit(ui->frame);
  send_server->setPlaceholderText("给客户端发送消息!");
  connect(send_server, &MyLineEdit::sendMessage, [this, send_server]() {
    server->write(client, send_server->text().toUtf8());
    send_server->setText("");
  });
  send_server->move(60, 160);
  send_server->resize(500, 50);

  // 将server 服务端与client 客户端连接
  connect(server, &MyServer::newUser, client, &MyClient::setUser);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::closeEvent(QCloseEvent *event) {
  qApp->exit();
  QWidget::closeEvent(event);
}

void MainWindow::setIcons() {
  QPixmap pixmap;
  pixmap.load(":/images/logo.png");
  ui->logo->setPixmap(pixmap.scaled(ui->logo->size(), Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
  pixmap.load(":/images/avatar.png");
  ui->avatar_label->setPixmap(pixmap.scaled(
      ui->avatar_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  pixmap.load(":/images/icons/notification.png");
  ui->notification_label->setPixmap(
      pixmap.scaled(ui->notification_label->size(), Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));

  pixmap.load(":/images/icons/github.png");
  ui->label_1->setPixmap(pixmap.scaled(ui->label_1->size(), Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));

  pixmap.load(":/images/icons/bilibili.png");
  ui->label_2->setPixmap(pixmap.scaled(ui->label_2->size(), Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));

  pixmap.load(":/images/icons/qq.png");
  ui->label_3->setPixmap(pixmap.scaled(ui->label_3->size(), Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));

  pixmap.load(":/images/icons/wechat.png");
  ui->label_4->setPixmap(pixmap.scaled(ui->label_4->size(), Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));

  pixmap.load(":/images/left_bg.jpg");
  ui->left_bg->setPixmap(pixmap.scaled(ui->left_bg->size(), Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));

  pixmap.load(":/images/search.png");
  ui->search_logo->setPixmap(pixmap);

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

bool MainWindow::init() {
  setIcons();
  setChangeButton();
  setChangePage();
  sendMessageInit();

  return false;
}

void MainWindow::setChangePage() {
  connect(ui->home_button, &QPushButton::clicked,
          [this]() { ui->stackedWidget->setCurrentIndex(0); });

  connect(ui->note_button, &QPushButton::clicked,
          [this]() { ui->stackedWidget->setCurrentIndex(1); });

  connect(ui->mysql_button, &QPushButton::clicked,
          [this]() { ui->stackedWidget->setCurrentIndex(2); });

  connect(ui->adv_button, &QPushButton::clicked,
          [this]() { ui->stackedWidget->setCurrentIndex(3); });
}

void MainWindow::sendMessageInit() {
  // ui->send_client->setPlaceholderText("给服务器发送消息");
  // ui->send_server->setPlaceholderText("给客户端发送消息");

  // connect(ui->send_client, &QTextEdit::enterEvent(QEnterEvent *event), const
  // char *amember)
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