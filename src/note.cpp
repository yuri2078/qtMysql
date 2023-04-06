#include "../include/note.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QTextBlock>
#include <qnamespace.h>
#include <qtextcursor.h>

Note::Note(QWidget *parent) : QTextEdit(parent) {
  QFile file(":css/note.css");
  file.open(QIODevice::ReadOnly);
  setStyleSheet(file.readAll());
  file.close();
  move(50, 50);
}

// 打开一个文件对话框并保存文件
void Note::saveFile() {
  QString file_name = QFileDialog::getSaveFileName(this, "请打开一个文件", "/home/yuri", "");
  QFile file(file_name, this);
  file.open(QFile::WriteOnly);
  if (file.isOpen()) {
    file.write(toPlainText().toUtf8());
  } else {
    qDebug() << "打开失败 -> " << file.error();
  }
  file.close();
}

// 打开一个文件对话框并打开文件
void Note::openFile() {
  QString file_name = QFileDialog::getOpenFileName(this, "请选择保存的位置", "/home/yuri", "");
  QFile file(file_name, this);
  file.open(QFile::ReadOnly);
  if (file.isOpen()) {
    setPlainText(file.readAll());
  } else {
    qDebug() << "打开失败 -> " << file.error();
  }
  file.close();
}

// 重写键盘事件
void Note::keyPressEvent(QKeyEvent *event) {
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

    if (line_text.count('{') > line_text.count('}')) {

      QString str = "\n" + QString(' ').repeated(count_space + 2) + "\n" + QString(' ').repeated(count_space) + "}";
      
      currentCursor.insertText(str);
      currentCursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
      currentCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
      currentCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, count_space + 2);
      
    } else {
      insertPlainText("\n" + QString(' ').repeated(count_space));
    }

    event->accept();
  } else if (event->key() == Qt::Key_ParenLeft) {
    insertPlainText("()");
    currentCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
  }
  else {
    QTextEdit::keyPressEvent(event);
  }
  this->setTextCursor(currentCursor);
}