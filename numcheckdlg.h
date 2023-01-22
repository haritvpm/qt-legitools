#ifndef NUMCHECKDLG_H
#define NUMCHECKDLG_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class NumCheckDlg;
}

class NumCheckDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NumCheckDlg(QWidget *parent = 0);
    ~NumCheckDlg();

    void verify();
private slots:




    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::NumCheckDlg *ui;
    QString stroldtext;
};

#endif // NUMCHECKDLG_H
