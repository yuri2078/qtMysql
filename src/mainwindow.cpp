#include "../include/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "../include/mylineedit.h"
#include "../include/note.h"
#include <QDateTime>
#include <QDesktopServices>
#include <QPicture>
#include <QtNetwork/QtNetwork>
#include <qabstractsocket.h>
#include <qprocess.h>
#include <qpushbutton.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <QSqlRecord>
#include <QStandardItemModel>
#include <QProcess>

// 打印日志宏
#define info_label_log(str)        \
  ui->info_label->insertPlainText( \
    QDateTime::currentDateTime().toString("hh:mm:ss  ") + str + "\n");

MainWindow::MainWindow(QWidget *parent, QMysql *db) :
  QMainWindow(parent), ui(new Ui::MainWindow), db(db),
  server(new MyServer(this)), client(new MyClient(this)), student_system(nullptr) {
  ui->setupUi(this);
  setWindowIcon(QIcon(":/images/login/icon.png"));

  ui->stackedWidget->setCurrentIndex(0); // 设置初始界面是 主页

  QFile file(":css/mainWindow.css", this);
  file.open(QFile::ReadOnly | QFile::Text);
  setStyleSheet(file.readAll());
  file.close();

  // 发出登陆信号
  connect(ui->logout_button, &QPushButton::clicked, [this]() {
    emit loginEnd();
  });

  // 搜索信号
  connect(ui->search_button, &QPushButton::clicked, [this]() {
    QDesktopServices::openUrl(QUrl("https://www.bing.com/search?q=" + ui->search_edit->text()));
  });

  init(); // 固定初始化
  ui->file_name->setText(ui->main_edit->file_name);

  connect(ui->open_file, &QAction::triggered, ui->main_edit, &Note::openFile);
  connect(ui->save_file, &QAction::triggered, ui->main_edit, &Note::saveFile);
  connect(ui->main_edit, &Note::textChanged, [=]() {
    QString sava = ui->main_edit->isSave ? "" : " *";
    ui->file_name->setText(ui->main_edit->file_name + sava);
  });
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
  setIcons();        // 设置各种图标
  setChangeButton(); // 设置切换按钮特效
  setChangePage();   // 设置切换界面
  sendMessageInit(); // 客户端发送消息初始化
  dataBaseInit();    // 数据库读写初始化

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
    ui->stackedWidget->setCurrentIndex(2); // mysql
  });

  connect(ui->adv_button, &QPushButton::clicked, [this]() {
    ui->stackedWidget->setCurrentIndex(3); // c/s
  });

  connect(ui->table_to_sql_button, &QPushButton::clicked, [this]() {
    ui->stackedWidget->setCurrentIndex(4); // 学生管理
    if (student_system == nullptr) {
      student_system = new StudentSystemLogin(ui->page_5);
      connect(student_system, &StudentSystemLogin::readyLogin, [this](const QString username, const QString password) {
        QSqlQuery query = this->db->exec("select * from student_system.user");
        while (query.next()) {
          qDebug() << query.value("username");
          qDebug() << query.value("password");
        }
      });
    }
    student_system->show();
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

void MainWindow::dataBaseInit() {
  ui->edit_search->setEnableLabel(true);
  ui->edit_search->label->setText("请输入MySql 查询语句");
  connect(ui->button_search, &QPushButton::clicked, [this]() {
    if (this->db->open()) {
      db->exec("use db_student;");
      model.setQuery(db->exec(ui->edit_search->text()));
      // 将查询结果显示在TableView控件中
      ui->table_view->setModel(&model);
      this->ui->table_view->show();
      ui->table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 平分列宽
      ui->table_view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);   // 平分行高
    } else {
      qDebug() << "打开失败!";
    }
  });
}
