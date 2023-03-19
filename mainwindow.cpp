#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMovie>
#include <QImage>
#include <QPainter>
#include <QFile>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent,QSqlDatabase *db) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(db)
{
    ui->setupUi(this);
    ui->textEdit->setTextColor(QColor(4));
    connect(ui->open_file, &QAction::triggered, [this](){
        QString str = QFileDialog::getOpenFileName(this,"请打开一个文件","/home/yuri","");
        QFile file(str,this);
        file.open(QFile::ReadWrite);
        auto file_txt = file.readAll();
        ui->textEdit->setText(file_txt);
        file.close();
    });

    connect(ui->kurseni_file, &QAction::triggered, [this](){
        QString str = QFileDialog::getSaveFileName(this,"请打开一个文件","/home/yuri","");
        QFile file(str,this);
        file.open(QFile::ReadWrite);
        QString new_txt = ui->textEdit->toPlainText();
        file.write(new_txt.toUtf8());
        file.close();
    });


    connect(ui->go_back,&QAction::triggered, [this](){
        emit this->loginEnd();
    });

    connect(ui->button_search, &QPushButton::clicked, [this](){
        emit this->sqlSearch();
//        if(this->db->open()) {
//            qDebug() << "打开成功!";
//        }
    });


    auto movie = new QMovie(":/images/QQ/back.gif",QByteArray(),this);
    auto image = new QImage(":/images/QQ/back.gif");
    ui->label->resize(image->width(),image->height());
    movie->start();
    ui->label->setMovie(movie);



}

MainWindow::~MainWindow()
{
    delete ui;
}


