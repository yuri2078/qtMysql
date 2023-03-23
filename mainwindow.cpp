
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPicture>




MainWindow::MainWindow(QWidget *parent,QSqlDatabase *db) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(db)
{
    ui->setupUi(this);

    connect(ui->logout_button, &QPushButton::clicked, [this](){
        emit loginEnd();
    });

    init();
//    connect(ui->open_file, &QAction::triggered, this, &MainWindow::openFile);
//
//    connect(ui->kurseni_file, &QAction::triggered, this, &MainWindow::saveFile);
//
//    connect(ui->go_back,&QAction::triggered, [this](){
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
//                QMessageBox::critical(this, "错误", "查询数据库时发生错误：" + query.lastError().text());
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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event) {
    qApp->exit();
    QWidget::closeEvent(event);
}

void MainWindow::setIcons() {
    QPixmap pixmap;
    pixmap.load(":/images/logo.png");
    ui->logo->setPixmap(pixmap.scaled(ui->logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pixmap.load(":/images/avatar.png");
    ui->avatar_label->setPixmap(pixmap.scaled(ui->avatar_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    pixmap.load(":/images/icons/notification.png");
    ui->notification_label->setPixmap(pixmap.scaled(ui->notification_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

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
    connect(ui->home_button, &QPushButton::clicked, [this](){
        ui->mysql_button->setChecked(false);
        ui->adv_button->setChecked(false);
        ui->table_to_sql_button->setChecked(false);
        ui->note_button->setChecked(false);
    });

    connect(ui->note_button, &QPushButton::clicked, [this](){
        ui->mysql_button->setChecked(false);
        ui->adv_button->setChecked(false);
        ui->table_to_sql_button->setChecked(false);
        ui->home_button->setChecked(false);
    });


    connect(ui->mysql_button, &QPushButton::clicked, [this](){
        ui->note_button->setChecked(false);
        ui->adv_button->setChecked(false);
        ui->table_to_sql_button->setChecked(false);
        ui->home_button->setChecked(false);
    });

    connect(ui->adv_button, &QPushButton::clicked, [this](){
        ui->note_button->setChecked(false);
        ui->mysql_button->setChecked(false);
        ui->table_to_sql_button->setChecked(false);
        ui->home_button->setChecked(false);
    });

    connect(ui->table_to_sql_button, &QPushButton::clicked, [this](){
        ui->note_button->setChecked(false);
        ui->adv_button->setChecked(false);
        ui->mysql_button->setChecked(false);
        ui->home_button->setChecked(false);
    });


}

bool MainWindow::init() {
    setIcons();
    setChangeButton();
    return false;
}

//void MainWindow::saveFile() {
//    QString str = QFileDialog::getSaveFileName(this,"请打开一个文件","/home/yuri","");
//    this->ui->stackedWidget->show();
//    this->ui->stackedWidget->setCurrentIndex(0);
//    QFile file(str,this);
//    file.open(QFile::ReadWrite);
//    if(file.isOpen()){
//        QString new_txt = ui->textEdit->toPlainText();
//        file.write(new_txt.toUtf8());
//    } else {
//        qDebug() << "打开失败 -> " << file.error();
//    }
//    file.close();
//}
//
//void MainWindow::openFile() {
//    QString str = QFileDialog::getOpenFileName(this,"请打开一个文件","/home/yuri","");
//    this->ui->stackedWidget->show();
//    this->ui->stackedWidget->setCurrentIndex(0);
//    QFile file(str,this);
//    file.open(QFile::ReadWrite);
//    if(file.isOpen()){
//        auto file_txt = file.readAll();
//        ui->textEdit->setText(file_txt);
//    } else {
//        qDebug() << "打开失败 -> " << file.error();
//    }
//
//    file.close();
//}


