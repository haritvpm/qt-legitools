#include "numcheckdlg.h"
#include "ui_numcheckdlg.h"
#include <QApplication>
#include <QDesktopWidget>
#include <algorithm>
#include <QPushButton>


NumCheckDlg::NumCheckDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumCheckDlg)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    QRect r = QApplication::desktop()->availableGeometry();
    QRect main_rect = this->geometry();
    QPoint pt = r.topRight();
    pt += QPoint(-15,70);

    main_rect.moveTopRight(pt);
    this->move(main_rect.topLeft());

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Back");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Check");

}

NumCheckDlg::~NumCheckDlg()
{
    delete ui;
}

void NumCheckDlg::verify()
{

    const QString s = ui->plainTextEdit->toPlainText();

    //check only if this is not our first output
    if(s.trimmed().startsWith("From")){
       return;
    }

   // QString strdebug;

    QStringList list = s.split("\n", QString::SkipEmptyParts);

    std::vector<int> v;

    //should check 781 മുതൽ 785, 786
    for ( QString s: list) {

       QStringList list2 = s.split(",", QString::SkipEmptyParts);
       for (QString n: list2) {

           int muthal = n.indexOf("മുത");

           //insert missing numbers
           if(-1 != muthal)
           {
                int start = n.left(muthal).toInt();

                int space = n.indexOf( ' ', muthal );

                QString strend = n.mid( space ).trimmed();

                int space2 = strend.indexOf( ' ' );

                if(-1 != space2){
                    strend = strend.left(space2);
                }

                int end = strend.toInt() ;

                for(int i=start; i <= end; i++)
                {
                     v.push_back(i);
                }

                continue;
           }

           QRegularExpression re("[^0-9,]"); //replace everythingother than digits and comma
           n.replace(re, "");

           QString strnum = n.trimmed();

       //    strdebug += "\n" + strnum;

           if(!strnum.isEmpty()){
               int num = strnum.toInt();
               if(num != 0)
               v.push_back(num );
           }
       }
    }

    std::sort( v.begin(), v.end(), [] (int const& a, int const &b){return a < b;});

    QString sout;

    if(v.size() >= 2  ){
        sout += "From " + QString::number(v[0]) +
                " to " + QString::number(v[ v.size()-1 ])
                + "\n\n";


        //check if continous

        for(int i= v[0]; i <= v[ v.size()-1 ]; i++)
        {
          int c = std::count( v.begin(), v.end(), i );

          if( 0 == c){
             sout += " Num " + QString::number(i) + " not found \n";
          }

          if( c > 1){
             sout += " Multple " + QString::number(i)+ " \n";
          }
        }

        //check if duplicates

    }

    sout +=  "\n";

    for(int i=0; i < v.size(); i++){
      //  sout += QString::number(v[i]) + "\n";
    }


    //set old text only if this is not our output
    //if(!s.trimmed().startsWith("From"))
    {
        stroldtext = s;
    }

    ui->plainTextEdit->setPlainText(sout );

}


void NumCheckDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    if( reinterpret_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Reset) ){
           ui->plainTextEdit->clear();
           stroldtext.clear();
        }
    else if( reinterpret_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Ok)){
        verify();
     }
    else if( reinterpret_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Cancel)){
       ui->plainTextEdit->setPlainText(stroldtext);
     }
    else if(reinterpret_cast<QPushButton*>(button) == ui->buttonBox->button(QDialogButtonBox::Close) ){
        QDialog::close();
     }
}
