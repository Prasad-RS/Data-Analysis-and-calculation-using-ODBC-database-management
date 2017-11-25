#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qmessagebox.h>
#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int Record_ID;
    QString Record;

private slots:
    void on_commandLinkButton_clicked();

    void on_commandLinkButton_8_clicked();

    void on_commandLinkButton_6_clicked();

    void on_commandLinkButton_9_clicked();

    void on_commandLinkButton_2_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_commandLinkButton_10_clicked();

    void on_listWidget_2_clicked(const QModelIndex &index);

    void on_commandLinkButton_11_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_commandLinkButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_commandLinkButton_12_clicked();

    void on_pushButton_5_clicked();

    void Calc_page_Rset(bool readonly);

    void on_commandLinkButton_14_clicked();

    void on_commandLinkButton_15_clicked();

    void on_commandLinkButton_13_clicked();

    void on_commandLinkButton_7_clicked();

    void on_commandLinkButton_5_clicked();

    void on_comboBox_2_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
