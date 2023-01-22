#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QAction>
#include <QMessageBox>
#include <QFile>

#include <QTextStream>
#include <QClipboard>
#include "numcheckdlg.h"

using namespace std;


// trim trailing spaces
void rtrim(wstring& str)
{
    size_t endpos = str.find_last_not_of(L" \t");
    size_t startpos = str.find_first_not_of(L" \t");
    if (std::string::npos != endpos)
    {
        str = str.substr(0, endpos + 1);
        str = str.substr(startpos);
    }
    else {
        str.erase(std::remove(std::begin(str), std::end(str), ' '), std::end(str));
    }
}

void ltrim(wstring& str)
{
    // trim leading spaces
    size_t startpos = str.find_first_not_of(L" \t");
    if (string::npos != startpos)
    {
        str = str.substr(startpos);
    }
}
void trim(wstring& str)
{
    ltrim(str);
    rtrim(str);

}


bool replace(std::wstring& str, const std::wstring& from, const std::wstring& to, bool all/* = false*/) {

    size_t start_pos;
    int nreplaced = 0;
    while ((start_pos = str.find(from)) != std::string::npos) {
        nreplaced++;
        str.replace(start_pos, from.length(), to);
    }

    return nreplaced > 0 ;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    QRect r = QApplication::desktop()->availableGeometry();
    QRect main_rect = this->geometry();
    /*
    QPoint pt = r.bottomRight();
    pt -= QPoint(main_rect.width()/2+r.width()/2 ,main_rect.height()+50);
    */

    QPoint pt = r.topRight();
    pt += QPoint(-main_rect.width()-50 ,(r.height()/2-main_rect.height()/2));


    //main_rect.moveTo(pt);
    this->move(pt);

    //ui->lineEdit->mainwindow = this;

    ReadMLAListFile(qApp->applicationDirPath() +  "/legitools.ini");

    /*std::map<std::wstring,std::vector<std::wstring>>::iterator iter;

    for (iter = mla_to_category.begin(); iter != mla_to_category.end(); ++iter) {

        QString strcategory = QString::fromStdWString(iter->first);

        QAction * action = ui->mainToolBar->addAction(strcategory);
        action->setObjectName(strcategory);

        action->connect(action, SIGNAL(triggered()), this, SLOT(on_actionA_triggered()));

    }
    */

}

MainWindow::~MainWindow()
{
    delete ui;
}
QString getMalName(QString strmla)
{
    //only add mal name
    int n= strmla.indexOf('=' );
    if(-1 != n){
       strmla = strmla.left(n).trimmed();
    }

    return strmla;
}
void MainWindow::AddNametoList( QString& strmla )
{

    //add shortcut number
   // strmla += " (" + QString::number(ui->listWidget->count()+1) + ")";

     strmla = QString::number(ui->listWidget->count()+1) + ") " +  strmla;

    ui->listWidget->addItem(strmla);
}

void MainWindow::on_actionA_triggered()
{
    ui->listWidget->clear();

    QObject *obj = sender();
    QString objName = obj->objectName();

    if( mla_to_category.find(objName.toStdWString()) == mla_to_category.end())
    {
        QMessageBox::information(this, "Section not found",
                                      objName,
                                      QMessageBox::Ok);
        return;
    }


    QStringList list;


    std::vector<std::wstring>& mlas = mla_to_category[objName.toStdWString()];

     for (std::vector<std::wstring>::iterator it = mlas.begin() ;
          it != mlas.end(); ++it){

          QString strmla = QString::fromStdWString(*it);

          strmla = getMalName(strmla);
          list.append(strmla);
          //AddNametoList( strmla );

     }

     if(list.count())
     {
         list.sort(Qt::CaseInsensitive);
     }

     foreach (QString strmla, list) {

         AddNametoList( strmla );
     }

     if(ui->listWidget->count() == 1){
         //only one item copy it to clipbard
         ui->listWidget->setCurrentRow(0);
         //on_listWidget_itemClicked(ui->listWidget->item(0));

     }

      ui->lineEdit->clear();
}


void MainWindow::ReadMLAListFile(QString filename)
{
    QFile file( filename );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox(QMessageBox::Information, "LegiTools",
                    "Error opening legitools.ini file", QMessageBox::Ok).exec();
        return;
    }

    mla_to_category.clear();

    QString sSection, sPreviousSection;
    std::vector<std::wstring> mlas;

    QTextStream in(&file);
    in.setCodec("UTF_16");
    QString line = in.readLine();
    while (!line.isNull())
    {
        line = line.trimmed();

        bool issection = false;
        if( line.length() > 0 && line.at(0) == '[' && ']' == line.at(line.length()-1))
        {
            sPreviousSection = sSection;
            sSection = line.mid(1,line.length()-2);
            issection = true;

        }

        if(issection)
        {
            if(sSection.compare("Settings",Qt::CaseInsensitive) == 0)
            {

            }
            else
            {
                if(!sPreviousSection.isEmpty() && mlas.size()){
                    mla_to_category[sPreviousSection.toStdWString()] = mlas ;
                }
                //letter A,B
                mlas.clear();
            }

        }
        else //reading keys within a section
        {
            if(sSection.compare("Settings",Qt::CaseInsensitive) == 0)
            {
            }
            else //party
            {
                std::wstring str = line.toStdWString();
                std::wstring strfrom = QString("ശ്രീ. ").toStdWString();
                std::wstring strto;

                replace(str,strfrom,strto,false);

                trim(str);

                if(!str.empty()){
                    mlas.push_back(str);
                }

            }

        }

        line = in.readLine();
    }

    //last item

    if(!sSection.isEmpty() && mlas.size()){
        mla_to_category[sSection.toStdWString()] = mlas ;
    }


}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //QMessageBox(QMessageBox::Information, "LegiTools",
    //            item->text(), QMessageBox::Ok).exec();

    QClipboard *clipboard = QApplication::clipboard();
   // QString originalText = clipboard->text();

    QString str = item->text();

    //remove text like line numbers added
    int n1 = str.indexOf('(');
    int n2 = str.indexOf(')');


   //there can be line number at start, like 5)
    if(n1 == -1 && n2 != -1){
        str = str.mid( n2+1 );
    } else if(n1 != -1 && n2 != -1) { //or end like (5)
        str = str.left( n1 );
    }



    str = str.trimmed();
    if(!str.isEmpty()){
        //make sure we dont replace ശ്രീമതി

        str.replace( "ശ്രീ. ","" );
        str = str.trimmed();

        if(!str.startsWith("ശ്രീമതി") && !str.startsWith("ഡോ.") &&
           !str.startsWith("പ്രൊഫ.")){
            str =  "ശ്രീ. " + str;
        }

        clipboard->setText(str);

    }



}

void MainWindow::on_listWidget_itemActivated(QListWidgetItem *item)
{
  //  on_listWidget_itemClicked(item);
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    //crashes
     //on_listWidget_itemClicked(current);
}

void MainWindow::on_listWidget_itemEntered(QListWidgetItem *item)
{
     //on_listWidget_itemClicked(item);
}

void MainWindow::on_listWidget_itemSelectionChanged()
{
    on_listWidget_itemClicked(ui->listWidget->currentItem());
}

void MainWindow::on_lineEdit_editingFinished()
{
    ui->lineEdit->clear();
    ui->lineEdit->strEnglish.clear();
    this->setWindowTitle("");
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if( ui->lineEdit->text().length() ==0 )
    {
        ui->lineEdit->strEnglish.clear();

    }

     setWindowTitle(  ui->lineEdit->strEnglish );
}

void MainWindow::on_actionCheck_Nums_triggered()
{


    NumCheckDlg dlg;
    if(dlg.exec() != QDialog::Accepted)
        return;
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    //QMessageBox(QMessageBox::Information, "LegiTools",
    //                arg1, QMessageBox::Ok).exec();


    QString str = ui->lineEdit->text();
    str = str.trimmed();

    QString strenglishwhenmalkey = ui->lineEdit->strEnglish.trimmed();
    strenglishwhenmalkey.replace("*","");

    //remove number from enlish too
    QRegularExpression rx("[0-9]+");
    QRegularExpressionMatch match = rx.match(strenglishwhenmalkey);
    if ( match.hasMatch()  )
    {
        QString sNumber= match.captured(0); // matched == "150"
        strenglishwhenmalkey.replace(sNumber,"");
       // num = sNumber.toInt();
    }
    strenglishwhenmalkey = strenglishwhenmalkey.trimmed();



    //sanitize
    str.remove('.');
    str.remove(',');
    str.remove('\'');
    str.remove(';');
    str.remove(']');
    str.remove('[');
    str.remove('\\');


    int num = -1;

    QRegularExpression rex("[0-9]+");
    match = rex.match(str);
    if ( match.hasMatch()  )
    {
        QString sNumber= match.captured(0); // matched == "150"
        str.replace(sNumber,"");
        num = sNumber.toInt();

    }

    str = str.trimmed();



    QString strlower = str.toLower();

    //when we load from toolbar, we clear empty.
    //prevent loading all in that case
    if(str.isEmpty() && num == -1){
       // return;
    }

    QStringList list;
    if(!str.isEmpty()){
       // return;
       ui->listWidget->clear();
    }

    std::map<std::wstring,std::vector<std::wstring>>::iterator iter;

    for (iter = mla_to_category.begin(); iter != mla_to_category.end(); ++iter) {

        QString strcategory = QString::fromStdWString(iter->first);

        std::vector<std::wstring>& mlas = mla_to_category[strcategory.toStdWString()];

        for (std::vector<std::wstring>::iterator it = mlas.begin(); it != mlas.end(); ++it){

             QString strmla = QString::fromStdWString(*it);
             QString strmlalower = strmla.toLower();

             if(str.isEmpty()){
                 if(num == -1){
                    // strmla = getMalName(strmla);
                   // AddNametoList(strmla);
                 }
             } else {

                 if(strmla.contains(str) ||
                    strmlalower.contains(strlower) ||
                    strmlalower.contains(strenglishwhenmalkey) )
                 {
                      strmla = getMalName(strmla);
                      list.append(strmla);
                      //AddNametoList( strmla );


                 }
             }
         }
    }


    if(list.count())
    {
        list.sort(Qt::CaseInsensitive);
    }

    foreach (QString strmla, list) {

        AddNametoList( strmla );
    }



    int prevrow = ui->listWidget->currentRow();

    if(ui->listWidget->count() == 1){
        //only one item copy it to clipbard
        ui->listWidget->setCurrentRow(0);
        if(prevrow ==0)  //setCurrentRow will do the same if sel changed
            on_listWidget_itemClicked(ui->listWidget->item(0));
    } else if(num >=1 && num <= ui->listWidget->count() )  {
        //user typed a number too. if so copy that index item
        ui->listWidget->setCurrentRow(num-1);
        if(prevrow == num-1) //setCurrentRow will do the same if sel changed
            on_listWidget_itemClicked(ui->listWidget->item(num-1));
    }

    if( ui->lineEdit->text().length() ==0 )
    {
        ui->lineEdit->strEnglish.clear();

    }

    setWindowTitle(  ui->lineEdit->strEnglish );
}

void MainWindow::on_actionCheck_Numbers_triggered()
{
on_actionCheck_Nums_triggered();
}
