#include "../include/note.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextStream>
#include <qnamespace.h>
#include <qtextcursor.h>


Note::Note(QWidget *parent) : QTextEdit(parent) {

  if (open(":css/note.css", QIODevice::ReadOnly | QIODevice::Text)) {
    setStyleSheet(file.readAll());
    file.close();
  }

  data = new QTextStream(&file);
  
  isSave = true;
  file_name = "/home/yuri/yuriQt/temp/Untitled";
  open(file_name);
}

Note::~Note() {
  isSave = true;
  file.close();
}

bool Note::open(const QString &file_name, QIODeviceBase::OpenMode flag) {
  file.setFileName(file_name);
  if (file.open(flag)) {
    this->file_name = file_name;
    isSave = true;
    return true;
  } else {
    qDebug() << file_name << " 打开失败! 错误码: " << file.error();
    return false;
  }
}

// 打开一个文件对话框并保存文件
void Note::saveFile() {
  QString file_name = QFileDialog::getSaveFileName(this, "请选择保存的位置", "/home/yuri/yuriQt/temp", "");
  if (open(file_name, QFile::WriteOnly | QFile::Truncate)) {
    emit Note::textChanged();
    *data << toPlainText();
    file.close();
  }
}

// 打开一个文件对话框并打开文件
void Note::openFile() {
  QString file_name = QFileDialog::getOpenFileName(this, "请选择打开的位置", "/home/yuri/yuriQt/temp", "");
  emit Note::textChanged();
  open(file_name);
  setPlainText(data->readAll());
}

// 重写键盘事件
void Note::keyPressEvent(QKeyEvent *event) {
  isSave = false;
  if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S) {
    isSave = true;
    open(file_name, QFile::WriteOnly | QFile::Truncate);
    *data << toPlainText();
    file.close();
    emit textChanged();
    event->accept();
    return;
  }

  QTextCursor currentCursor = this->textCursor();
  QString line_text = textCursor().block().text();

  if (event->key() == Qt::Key_Tab) {
    insertPlainText("  ");
    event->accept(); // 标记为已经处理
  } else if (event->key() == Qt::Key_Return) {
    // 获取本行文本
    
    int count_space = 0;
    for (QChar c : line_text) {
      if (c.isSpace()) {
        count_space++;
      } else {
        break;
      }
    }

    if (toPlainText().count('{') > toPlainText().count('}')) {

      QString str = "\n" + QString(' ').repeated(count_space + 2) + "\n" + QString(' ').repeated(count_space) + "}";
      
      currentCursor.insertText(str);
      currentCursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
      currentCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
      currentCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, count_space + 2);

    } else {
      QString str = "\n" + QString(' ').repeated(count_space);
      insertPlainText(str);
    }

    event->accept();
    this->setTextCursor(currentCursor);
  } else if (event->key() == Qt::Key_ParenLeft) {
    insertPlainText("()");
    currentCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
    event->accept();
    this->setTextCursor(currentCursor);
  } else { 
    QTextEdit::keyPressEvent(event);
  }
}