#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void ReadMLAListFile(QString filename);
private slots:
    void on_actionA_triggered();
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget_itemEntered(QListWidgetItem *item);

    void on_listWidget_itemSelectionChanged();

    void on_lineEdit_editingFinished();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_actionCheck_Nums_triggered();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_actionCheck_Numbers_triggered();

private:
    Ui::MainWindow *ui;
    std::map<std::wstring, std::vector<std::wstring>> mla_to_category;
    void AddNametoList(QString &strmla);
};

#endif // MAINWINDOW_H
