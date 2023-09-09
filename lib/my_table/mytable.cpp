#include "mytable.h"
#include "./ui_mytable.h"
#include <QTableWidget>

#include <QFile>
#include <qpushbutton.h>

MyTable::MyTable(QWidget *parent) :
  QWidget(parent), ui(new Ui::MyTable) {
  ui->setupUi(this);
  setWindowTitle("system_login");
  // 创建 QTableWidget 对象
  QTableWidget *tableWidget = ui->tableWidget;

  // 设置表格的行数和列数
  tableWidget->setRowCount(3);
  tableWidget->setColumnCount(2);

  // 设置表格的标题行和标题列
  tableWidget->setHorizontalHeaderLabels({"Column 1", "Column 2"});
  tableWidget->setVerticalHeaderLabels({"Row 1", "Row 2", "Row 3"});

  // 在表格中添加数据
  tableWidget->setItem(0, 0, new QTableWidgetItem("Value 1"));
  tableWidget->setItem(0, 1, new QTableWidgetItem("Value 2"));
  tableWidget->setItem(1, 0, new QTableWidgetItem("Value 3"));
  tableWidget->setItem(1, 1, new QTableWidgetItem("Value 4"));
  tableWidget->setItem(2, 0, new QTableWidgetItem("Value 5"));
  tableWidget->setItem(2, 1, new QTableWidgetItem("Value 6"));

  // 调整表格大小，使其适应内容
  tableWidget->resizeColumnsToContents();
  tableWidget->resizeRowsToContents();

  // 显示表格
  tableWidget->show();
  connect(ui->pushButton, &QPushButton::clicked, [this]() {
    QTableWidget *tableWidget = ui->tableWidget;
    tableWidget->setRowCount(tableWidget->rowCount() + 1);
  });

  connect(ui->pushButton_2, &QPushButton::clicked, [this]() {
    QTableWidget *tableWidget = ui->tableWidget;
    tableWidget->setColumnCount(tableWidget->columnCount() + 1);
  });

  tableWidget->show();
  connect(ui->pushButton_3, &QPushButton::clicked, [this]() {
    QTableWidget *tableWidget = ui->tableWidget;
    tableWidget->setRowCount(tableWidget->rowCount() - 1);
  });

  connect(ui->pushButton_4, &QPushButton::clicked, [this]() {
    QTableWidget *tableWidget = ui->tableWidget;
    tableWidget->setColumnCount(tableWidget->columnCount() - 1);
  });

  connect(ui->tableWidget, &QTableWidget::cellChanged, [this](int row, int col) {
    qDebug() << row << "行 " << col << " 列发生变化!";
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
  });
}

MyTable::~MyTable() {
  delete ui;
}
