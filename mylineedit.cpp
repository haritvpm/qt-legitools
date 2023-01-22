#include "mylineedit.h"

#include <QLocale>
#include <QApplication>



MyLineEdit::MyLineEdit(QWidget *parent)
 : QLineEdit(parent)
{
 //   setLocale(QLocale::English);
}

MyLineEdit::~MyLineEdit()
{}


void MyLineEdit::keyPressEvent(QKeyEvent* event)
{

//    QString keyBoardLang = QGuiApplication::inputMethod()->

    //default
  /*  if(QLocale::C == keyBoardLang)
    {
         QLineEdit::keyPressEvent(event);
         return;
    }*/

   // event->accept();

    bool someotherkey =false;

    if(!(event->modifiers() & Qt::ControlModifier))
    {
        switch( event->nativeScanCode() )
        {
       /*
        case Qt::Key_A: strEnglish += "A"; break;
        case Qt::Key_B: strEnglish += "B"; break;
        case Qt::Key_C: strEnglish += "C"; break;
        case Qt::Key_D: strEnglish += "D"; break;
        case Qt::Key_E: strEnglish += "E"; break;
        case Qt::Key_F: strEnglish += "F"; break;
        case Qt::Key_G: strEnglish += "G"; break;
        case Qt::Key_H: strEnglish += "H"; break;
        case Qt::Key_I: strEnglish += "I"; break;
        case Qt::Key_J: strEnglish += "J"; break;
        case Qt::Key_K: strEnglish += "K"; break;
        case Qt::Key_L: strEnglish += "L"; break;
        case Qt::Key_M: strEnglish += "M"; break;
        case Qt::Key_N: strEnglish += "N"; break;
        case Qt::Key_O: strEnglish += "O"; break;
        case Qt::Key_P: strEnglish += "P"; break;
        case Qt::Key_Q: strEnglish += "Q"; break;
        case Qt::Key_R: strEnglish += "R"; break;
        case Qt::Key_S: strEnglish += "S"; break;
        case Qt::Key_T: strEnglish += "T"; break;
        case Qt::Key_U: strEnglish += "U"; break;
        case Qt::Key_V: strEnglish += "V"; break;
        case Qt::Key_W: strEnglish += "W"; break;
        case Qt::Key_X: strEnglish += "X"; break;
        case Qt::Key_Y: strEnglish += "Y"; break;
        case Qt::Key_Z: strEnglish += "Z"; break;*/

        case 24: strEnglish += "q"; break;
        case 25: strEnglish += "w"; break;
        case 26: strEnglish += "e"; break;
        case 27: strEnglish += "r"; break;
        case 28: strEnglish += "t"; break;
        case 29: strEnglish += "y"; break;
        case 30: strEnglish += "u"; break;
        case 31: strEnglish += "i"; break;
        case 32: strEnglish += "o"; break;
        case 33: strEnglish += "p"; break;

        case 38: strEnglish += "a"; break;
        case 39: strEnglish += "s"; break;
        case 40: strEnglish += "d"; break;
        case 41: strEnglish += "f"; break;
        case 42: strEnglish += "g"; break;
        case 43: strEnglish += "h"; break;
        case 44: strEnglish += "j"; break;
        case 45: strEnglish += "k"; break;
        case 46: strEnglish += "l"; break;

        case 52: strEnglish += "z"; break;
        case 53: strEnglish += "x"; break;
        case 54: strEnglish += "c"; break;
        case 55: strEnglish += "v"; break;
        case 56: strEnglish += "b"; break;
        case 57: strEnglish += "n"; break;
        case 58: strEnglish += "m"; break;
        case 65: strEnglish += " "; break;
        default: someotherkey = true;
        }
    }



    if(someotherkey)
    {
        if( event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
        {
            strEnglish += event->text();
            someotherkey = false;
        }
        else
        if( event->key() == Qt::Key_Backspace && strEnglish.length())
        {
            strEnglish = strEnglish.left( strEnglish.length()-1 );
            someotherkey = false;
        }
    }

    //suppose mal keyboard and english typing
    //user presses some other key like coma by accident
    //then hits backspace
    //our english needs toget rid of last key typed,
    //not the existing english char
 //   strEnglish +=  QString::number( event->nativeScanCode() );

    if(someotherkey /*&& !(event->modifiers() & Qt::ShiftModifier)*/){
        if( event->key() != Qt::Key_Shift &&
            event->key() != Qt::Key_CapsLock &&

            event->key() != Qt::Key_Delete  )
        {
            //make sure there is only one space
           // strEnglish = strEnglish.trimmed();

            strEnglish += "*";

        }
    }

   QLineEdit::keyPressEvent(event);

   if( this->text().length() == 0 ){
        strEnglish.clear();
   }

   //mainwindow->setWindowTitle( strEnglish );
}
