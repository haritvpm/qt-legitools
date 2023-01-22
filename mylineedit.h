#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QWidget>

#include <QLineEdit>
#include <QString>

#include <QKeyEvent>

//class MainWindow;
//#include "mainwindow.h"

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    MyLineEdit(QWidget *parent = 0);
    ~MyLineEdit();

    QString strEnglish;
   // MainWindow * mainwindow;

protected:

      void keyPressEvent ( QKeyEvent * event );


};

#endif // MYLINEEDIT_H
