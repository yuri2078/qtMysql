#include "../include/mylineedit.h"
#include <QEventTransition>
#include <QKeyEvent>
#include <QPropertyAnimation>

MyLineEdit::MyLineEdit(QWidget *parent) :
  QLineEdit(parent), label(new QLabel(this)), machine(new QStateMachine()),
  fous_in(new QState), fous_out(new QState) {
  machine->addState(fous_in);
  machine->addState(fous_out);
  machine->setInitialState(fous_out);

  auto transition = new QEventTransition(this, QEvent::FocusIn);
  transition->setTargetState(fous_in);
  fous_out->addTransition(transition);

  auto animal = new QPropertyAnimation(label, "pos");
  animal->setDuration(700);
  animal->setEasingCurve(QEasingCurve::OutCubic);
  transition->addAnimation(animal);

  transition = new QEventTransition(this, QEvent::FocusOut);
  transition->setTargetState(fous_out);
  fous_in->addTransition(transition);
  animal = new QPropertyAnimation(label, "pos");
  animal->setDuration(700);
  animal->setEasingCurve(QEasingCurve::OutCubic);
  transition->addAnimation(animal);
  setEnableLabel(false);

  setDefaultStyle();
  machine->start();

  connect(this, &MyLineEdit::textChanged, [this]() {
    if (text().isEmpty()) {
      fous_out->assignProperty(label, "pos", QPointF(0, QFontMetrics(label->font()).height() / 2.0 - 2));
    } else {
      fous_out->assignProperty(label, "pos", QPointF(0, -QFontMetrics(label->font()).height() * 0.6));
    }
  });
}

void MyLineEdit::setEnableLabel(bool has_label) {
  this->has_label = has_label;
  if (has_label) {
    setContentsMargins(0, QFontMetrics(label->font()).height(), 0, 0);
    label->setFixedSize(width(), height());
    QFont font = label->font();
    label->setFont(QFont("Fira Code Medium", height() / 4));
    label->show();
  } else {
    label->hide();
    setContentsMargins(0, 0, 0, 0);
    label->setFixedSize(width(), height());
  }
  fous_out->assignProperty(label, "pos", QPointF(0, QFontMetrics(label->font()).height() / 2.0 - 2));
  fous_in->assignProperty(label, "pos", QPointF(0, -QFontMetrics(label->font()).height() * 0.6));
}

void MyLineEdit::focusOutEvent(QFocusEvent *event) {
  QLineEdit::focusOutEvent(event);
}

void MyLineEdit::focusInEvent(QFocusEvent *event) {
  QLineEdit::focusInEvent(event);
}

void MyLineEdit::setDefaultStyle() {
  setStyleSheet("MyLineEdit { \
        border: none;\
        border-bottom: 2px solid #999999;\
        background-color: transparent;\
        font-size: 18px;\
        color: #333333;\
    }\
    MyLineEdit:hover {\
        border-bottom: 2px solid #3EB1E8;\
    }\
    MyLineEdit:focus {\
        border-bottom: 2px solid #5EBD3E;\
    }");
  // label->setFixedSize(width(), height());
  label->setStyleSheet("background-color: transparent; color: gray;border:none;");
}

void MyLineEdit::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Return && !text().isEmpty()) {
    emit sendMessage();
  } else {
    QLineEdit::keyPressEvent(event);
  }
}