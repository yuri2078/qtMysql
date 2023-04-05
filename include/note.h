#ifndef NOTE_H
#define NOTE_H

#include <QTextEdit>

class Note : public QTextEdit {
  Q_OBJECT

public:
  Note(QWidget *parent = nullptr);
  void keyPressEvent(QKeyEvent *e) override;



public:
  void saveFile();
  void openFile();

};

#endif