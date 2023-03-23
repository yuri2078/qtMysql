#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QMessageBox>
#include <QMovie>
#include <QFile>
#include <QPushButton>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent,QSqlDatabase *db);
    ~MainWindow();

//    void saveFile();
//    void openFile();
    bool init();


    void closeEvent(QCloseEvent *event) override;
//    void paintEvent(QPaintEvent *) override;


signals:
    void loginEnd();


private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;
    QSqlQueryModel model;
    void setIcons();
    void setChangeButton();

};
#endif // MAINWINDOW_H
