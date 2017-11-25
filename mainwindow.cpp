#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase signaldb = QSqlDatabase::addDatabase("QODBC");
    signaldb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=ANALYSIS.mdb;");
    bool  ok = signaldb.open();
    if(ok)
        qDebug()<<"Database opened";
    else
        qDebug()<<"Not opened";
    on_commandLinkButton_6_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_commandLinkButton_clicked()
{
    if(ui->label_10->text().isEmpty())
    {
        QMessageBox::information(this,"Information","Please Open a Profile to add new records to profile",QMessageBox::Ok);
        return;
    }
    QMessageBox::StandardButton new_record;
    new_record=QMessageBox::question(this,"Add New Record","Are you sure want to add a new record to "+ui->label_10->text()+" profile",QMessageBox::Yes|QMessageBox::No);
    if(new_record==QMessageBox::No)
        return;
    Record=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm A");
    QStringList Rec_Date=Record.split(" ");
    for(int c=0;c<ui->listWidget->count();c++)
    {
        QStringList item=ui->listWidget->item(c)->text().split(" ");
        if(item.at(0)==Rec_Date.at(0))
        {
            QMessageBox::warning(this,"Record Already Exist","You can able to create only one record in a day,\nEdit the another record that created today.",QMessageBox::Ok);
            return;
        }
    }
    ui->comboBox->clear();
    QSqlQuery query;
    query.exec("SELECT FEILD FROM PROFILE WHERE PROFILE_NAME='"+ui->label_10->text()+"'");
    query.next();
    while(!(query.value(0).toString().isEmpty()))
    {
        ui->comboBox->addItem(query.value(0).toString());
        query.next();
    }
    on_commandLinkButton_7_clicked();
    query.exec("INSERT INTO "+ui->label_10->text()+"_TOTAL (RECORD) VALUES ('"+Record+"')");
    ui->label_17->setText(Rec_Date.at(0));
    for(int i=0;i<ui->comboBox->count();i++)
    {
        query.exec("INSERT INTO "+ui->label_10->text()+"_"+ui->comboBox->itemText(i)+" (RECORD) VALUES ('"+Record+"')");
    }
//    query.exec("SELECT ID FROM ID WHERE RECORD='"+Record+"'");
//    query.next();
//    Record_ID=query.value(0).toInt(0);
    //ui->lineEdit->setValidator(QIntValidator(0,999,this));
    //ui->lineEdit_2->setValidator(QIntValidator(0,999,this));
    ui->lineEdit_3->setReadOnly(true);
    ui->commandLinkButton_12->setDisabled(true);
    ui->groupBox_3->setTitle("New Record");
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_commandLinkButton_8_clicked()
{
    if(ui->label_10->text().isEmpty())
    {
        QMessageBox::information(this,"Information","Please Open a Profile to view the records",QMessageBox::Ok);
        return;
    }
    ui->listWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    int cnt;
    QSqlQuery query;
    query.exec("SELECT COUNT(RECORD) FROM "+ui->label_10->text()+"_TOTAL");
    query.next();
    cnt=query.value(0).toInt(0);
    query.exec("SELECT RECORD FROM "+ui->label_10->text()+"_TOTAL");
    for(int i=0;i<cnt;i++)
    {
        query.next();
        ui->listWidget->addItem(query.value(0).toString());
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_commandLinkButton_6_clicked()
{
    ui->listWidget_2->clear();
    ui->listWidget_3->clear();
    QSqlQuery query;
    int cnt;
    query.exec("SELECT COUNT(PROFILE_NAME) FROM PROFILE");
    query.next();
    cnt=query.value(0).toInt(0);
    query.exec("SELECT PROFILE_NAME,COUNT(*) FROM PROFILE GROUP BY PROFILE_NAME");

    while(cnt!=0)
    {
        query.next();
        ui->listWidget_2->addItem(query.value(0).toString());
        cnt-=query.value(1).toInt(0);
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_commandLinkButton_9_clicked()
{
    qDebug()<<ui->listWidget_2->currentIndex().row();
    if(ui->listWidget_2->currentIndex().row()<0)
    {
        QMessageBox::warning(this,"Warning","Please select a profile to open",QMessageBox::Ok);
        return;
    }
    if((!(ui->listWidget_2->currentItem()->isSelected())))
    {
        QMessageBox::warning(this,"Warning","Please select a profile to open",QMessageBox::Ok);
        return;
    }
    ui->label_10->clear();
    ui->label_17->clear();
    ui->label_10->setText(ui->listWidget_2->currentItem()->text());
    on_commandLinkButton_8_clicked();
}

void MainWindow::on_commandLinkButton_2_clicked()
{
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->listWidget_4->clear();
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!(ui->lineEdit_5->text().isEmpty()))
    {
        for(int i=0;i<ui->listWidget_4->count();i++)
        {
            if(ui->listWidget_4->item(i)->text()==ui->lineEdit_5->text())
            {
                ui->lineEdit_5->clear();
                QMessageBox::warning(this,"Warning","Feild name already Exist",QMessageBox::Ok);
                return;
            }
        }
        ui->listWidget_4->addItem(ui->lineEdit_5->text());
        ui->lineEdit_5->clear();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->listWidget_4->takeItem(ui->listWidget_4->currentRow());
}

void MainWindow::on_commandLinkButton_10_clicked()
{
    if(ui->lineEdit_4->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning","Plese enter the profile name before saving.",QMessageBox::Ok);
        return;
    }
    for(int i=0;i<ui->listWidget_2->count();i++)
    {
        if(ui->lineEdit_4->text()==ui->listWidget_2->item(i)->text())
        {
            ui->lineEdit_4->clear();
            QMessageBox::warning(this,"Warning","Profile name already exist, Enter new profile name.",QMessageBox::Ok);
            return;
        }
    }
    if(ui->listWidget_4->count()<1)
    {
        QMessageBox::warning(this,"Warning","Profile should contain minimum one feild.",QMessageBox::Ok);
        return;
    }
    QSqlQuery query;
    for(int i=0;i<ui->listWidget_4->count();i++)
        query.exec("INSERT INTO PROFILE (PROFILE_NAME,FEILD) VALUES ('"+ui->lineEdit_4->text()+"','"+ui->listWidget_4->item(i)->text()+"')");

    for(int i=0;i<ui->listWidget_4->count();i++)
        query.exec("CREATE TABLE "+ui->lineEdit_4->text()+"_"+ui->listWidget_4->item(i)->text()+" (ID int,RECORD varchar(255),ORAI int,MUDI int,DAMAGE int,TOTAL int)");

    query.exec("CREATE TABLE "+ui->lineEdit_4->text()+"_TOTAL (ID int,RECORD varchar(255))");
    for(int i=0;i<ui->listWidget_4->count();i++)
        query.exec("ALTER TABLE "+ui->lineEdit_4->text()+"_TOTAL ADD "+ui->listWidget_4->item(i)->text()+" int");
    query.exec("ALTER TABLE "+ui->lineEdit_4->text()+"_TOTAL ADD TOTAL int,DAMAGE int,NET_TOTAL int,PRICE_OF_ONE float,AMOUNT float");
    on_commandLinkButton_6_clicked();
}

void MainWindow::on_listWidget_2_clicked(const QModelIndex &index)
{
    ui->listWidget_3->clear();
    QSqlQuery query;
    query.exec("SELECT FEILD FROM PROFILE WHERE PROFILE_NAME='"+ui->listWidget_2->item(index.row())->text()+"'");
    query.next();
    while(!(query.value(0).toString().isEmpty()))
    {
        ui->listWidget_3->addItem(query.value(0).toString());
        query.next();
    }
}

void MainWindow::on_commandLinkButton_11_clicked()
{
    on_commandLinkButton_6_clicked();
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    ui->tableWidget->setRowCount(0);
    QSqlQuery query;
    query.exec("SELECT FEILD FROM PROFILE WHERE PROFILE_NAME='"+ui->label_10->text()+"'");
    query.next();
    ui->tableWidget->setColumnCount(2);
    QStringList lab;
    lab<<"Feilds"<<"Total";
    ui->tableWidget->setHorizontalHeaderLabels(lab);
    ui->tableWidget->setColumnWidth(0,130);
    ui->tableWidget->setColumnWidth(1,80);
    for(int i=0;(!(query.value(0).toString().isEmpty()));i++)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(query.value(0).toString()));
        query.next();
    }
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        query.exec("SELECT "+ui->tableWidget->item(i,0)->text()+" FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->item(index.row())->text()+"'");
        query.next();
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(query.value(0).toString()));
    }
    int row=ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row+2);
    ui->tableWidget->setItem(row,0,new QTableWidgetItem("DAMAGES"));
    ui->tableWidget->setItem(row+1,0,new QTableWidgetItem("TOTAL"));
    query.exec("SELECT DAMAGE,TOTAL FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->item(index.row())->text()+"'");
    query.next();
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(query.value(0).toString()));
    ui->tableWidget->setItem(row+1,1,new QTableWidgetItem(query.value(1).toString()));
}

void MainWindow::on_commandLinkButton_3_clicked()
{
    if(ui->listWidget->currentIndex().row()<0)
    {
        QMessageBox::information(this,"Information","Please select a record to edit.",QMessageBox::Ok);
        return;
    }
    if(!(ui->listWidget->currentItem()->isSelected()))
    {
        QMessageBox::information(this,"Information","Please select a record to edit.",QMessageBox::Ok);
        return;
    }
    ui->label_17->setText(QStringList(ui->listWidget->currentItem()->text().split(" ")).at(0));
    ui->comboBox->clear();
    QSqlQuery query;
    query.exec("SELECT FEILD FROM PROFILE WHERE PROFILE_NAME='"+ui->label_10->text()+"'");
    query.next();
    while(!(query.value(0).toString().isEmpty()))
    {
        ui->comboBox->addItem(query.value(0).toString());
        query.next();
    }
    on_commandLinkButton_7_clicked();
    Record=ui->listWidget->currentItem()->text();
    on_comboBox_currentIndexChanged(ui->comboBox->itemText(0));
    ui->groupBox_3->setTitle("Edit Record");
    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::on_pushButton_clicked()
{
    int total=(ui->lineEdit->text().toInt(0)*200)+(ui->lineEdit_2->text().toInt(0)*2);
    ui->lineEdit_3->setText(QString::number(total));
}

void MainWindow::on_pushButton_3_clicked()
{
    on_pushButton_clicked();
    QSqlQuery query;
    int orai=0,mudi=0,ui_damage=0,ui_total=0;
    orai=ui->lineEdit->text().toInt(0);
    mudi=ui->lineEdit_2->text().toInt(0);
    ui_damage=ui->lineEdit_6->text().toInt(0);
    ui_total=ui->lineEdit_3->text().toInt(0);
    query.exec("UPDATE "+ui->label_10->text()+"_"+ui->comboBox->currentText()+" SET ORAI="+QString::number(orai)+",MUDI="+QString::number(mudi)+",DAMAGE="+QString::number(ui_damage)+",TOTAL="+QString::number(ui_total)+" WHERE RECORD='"+Record+"'");
    query.exec("UPDATE "+ui->label_10->text()+"_TOTAL SET "+ui->comboBox->currentText()+"="+QString::number(ui_total)+" WHERE RECORD='"+Record+"'");

    int total=0;
    int damage=0;
    for(int i=0;i<ui->comboBox->count();i++)
    {
        query.exec("SELECT "+ui->comboBox->itemText(i)+" FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+Record+"'");
        query.next();
        total+=query.value(0).toInt(0);
        query.exec("SELECT DAMAGE FROM "+ui->label_10->text()+"_"+ui->comboBox->itemText(i)+" WHERE RECORD='"+Record+"'");
        query.next();
        damage+=query.value(0).toInt(0);
    }

    int net_total=total-damage;
    query.exec("UPDATE "+ui->label_10->text()+"_TOTAL SET TOTAL="+QString::number(total)+",DAMAGE="+QString::number(damage)+",NET_TOTAL="+QString::number(net_total)+" WHERE RECORD='"+Record+"'");
    if(ui_total>0)
        ui->commandLinkButton_12->setEnabled(true);
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    on_commandLinkButton_7_clicked();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_3->clear();
    QSqlQuery query;
    query.exec("SELECT ORAI,MUDI,DAMAGE,TOTAL FROM "+ui->label_10->text()+"_"+arg1+" WHERE RECORD='"+Record+"'");
    query.next();
    if(query.value(0).toInt(0)!=0)
        ui->lineEdit->setText(query.value(0).toString());
    if(query.value(1).toInt(0)!=0)
        ui->lineEdit_2->setText(query.value(1).toString());
    if(query.value(2).toInt(0)!=0)
        ui->lineEdit_6->setText(query.value(2).toString());
    if(query.value(3).toInt(0)!=0)
        ui->lineEdit_3->setText(query.value(3).toString());
}

void MainWindow::on_commandLinkButton_12_clicked()
{
    Calc_page_Rset(true);
    QSqlQuery query;
    query.exec("SELECT TOTAL,DAMAGE,NET_TOTAL,PRICE_OF_ONE,AMOUNT FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+Record+"'");
    query.next();
    if(query.value(0).toInt(0)!=0)
        ui->lineEdit_7->setText(query.value(0).toString());
    if(query.value(1).toInt(0)!=0)
        ui->lineEdit_8->setText(query.value(1).toString());
    if(query.value(2).toInt(0)!=0)
        ui->lineEdit_11->setText(query.value(2).toString());
    if(query.value(3).toInt(0)!=0)
        ui->lineEdit_9->setText(query.value(3).toString());
    if(query.value(4).toInt(0)!=0)
        ui->lineEdit_10->setText(query.value(4).toString());
    on_commandLinkButton_13_clicked();
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButton_5_clicked()
{
    QSqlQuery query;
    float price=0,amount=0;
    price=ui->lineEdit_9->text().toFloat(0);
    amount=(ui->lineEdit_11->text().toInt(0))*price;
    ui->lineEdit_10->setText(QString::number(amount,'f',2));
    query.exec("UPDATE "+ui->label_10->text()+"_TOTAL SET PRICE_OF_ONE="+QString::number(price)+",AMOUNT="+QString::number(amount)+" WHERE RECORD='"+Record+"'");
}
void MainWindow::Calc_page_Rset(bool readonly)
{
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->lineEdit_10->clear();
    ui->lineEdit_11->clear();
    ui->lineEdit_7->setReadOnly(readonly);
    ui->lineEdit_8->setReadOnly(readonly);
    ui->lineEdit_9->setReadOnly(readonly);
    ui->lineEdit_10->setReadOnly(readonly);
    ui->lineEdit_11->setReadOnly(readonly);
    if(readonly)
    {
        ui->commandLinkButton_13->show();
        ui->customPlot_2->show();
    }else
    {
        ui->commandLinkButton_13->hide();
        ui->customPlot_2->hide();
    }
}

void MainWindow::on_commandLinkButton_14_clicked()
{
    Calc_page_Rset(false);
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_commandLinkButton_15_clicked()
{
    if(ui->listWidget->currentIndex().row()<0)
    {
        QMessageBox::information(this,"Information","Please select a record to delete.",QMessageBox::Ok);
        return;
    }
    if((!(ui->listWidget->currentItem()->isSelected())))
    {
        QMessageBox::information(this,"Information","Please select a record to delete.",QMessageBox::Ok);
        return;
    }
    QMessageBox::StandardButton delete_record;
    delete_record=QMessageBox::question(this,"Delete Record","Are you sure want to delete "+ui->listWidget->currentItem()->text()+" record from "+ui->label_10->text()+" profile",QMessageBox::Yes|QMessageBox::No);
    if(delete_record==QMessageBox::No)
        return;
    QSqlQuery query;
    QStringList Feils;
    query.exec("SELECT FEILD FROM PROFILE WHERE PROFILE_NAME='"+ui->label_10->text()+"'");
    query.next();
    for(int i=0;(!(query.value(0).toString().isEmpty()));i++)
    {
        Feils<<query.value(0).toString();
        query.next();
    }
    query.exec("DELETE FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->currentItem()->text()+"'");
    for(int i=0;i<Feils.count();i++)
    {
        qDebug()<<Feils.at(i);
        query.exec("DELETE FROM "+ui->label_10->text()+"_"+Feils.at(i)+" WHERE RECORD='"+ui->listWidget->currentItem()->text()+"'");
    }
    on_commandLinkButton_8_clicked();
}

void MainWindow::on_commandLinkButton_13_clicked()
{
     ui->customPlot_2->clearGraphs();
    QVector<double> x(101), y(101);//,D(101),M(101); // initialize with entries 0..100
    QSqlQuery query;
    int value=0;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        x[i] = i; // x goes from -1 to 1
        query.exec("SELECT NET_TOTAL FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
        query.next();
        y[i] =query.value(0).toDouble(0);
        if(value<y[i])
            value=y[i];
//        query.exec("SELECT DAMAGE FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
//        query.next();
//        D[i] =query.value(0).toDouble(0);
//        query.exec("SELECT PRICE_OF_ONE FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
//        query.next();
//        M[i] =query.value(0).toDouble(0);
    }

    // generate some data:


    // create graph and assign data to it:
    QCPGraph *graph1 = ui->customPlot_2->addGraph();
    graph1->setData(x, y);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 5));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));
    graph1->setName("Net-Total");

//    QCPGraph *graph2 = ui->customPlot_2->addGraph();
//    graph2->setData(x, D);
//    graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::red, 1.5), QBrush(Qt::white), 5));
//    graph2->setPen(QPen(QColor( 255, 51, 57), 2));
//    graph2->setName("Damage");

//    QCPGraph *graph3 = ui->customPlot_2->addGraph();
//    graph3->setData(x, M);
//    graph3->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::yellow, 1.5), QBrush(Qt::white), 5));
//    graph3->setPen(QPen(QColor( 254, 197, 67), 2));
//    graph3->setName("Price of one");

    // give the axes some labels:
    ui->customPlot_2->xAxis->setLabel("Periodic Records");
    ui->customPlot_2->yAxis->setLabel("No.of Counts");
    // set axes ranges, so we see all data:
    ui->customPlot_2->xAxis->setRange(0, (ui->listWidget->count()+2));
    ui->customPlot_2->yAxis->setRange(0, (value+10));
    ui->customPlot_2->legend->setVisible(true);
    //ui->customPlot_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customPlot_2->replot();

}

void MainWindow::on_commandLinkButton_7_clicked()
{
    ui->customPlot->clearGraphs();
    QVector<double> x(101), y(101),D(101);// initialize with entries 0..100
    QSqlQuery query;
    int value=0;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        x[i] = i; // x goes from -1 to 1
        query.exec("SELECT TOTAL FROM "+ui->label_10->text()+"_"+ui->comboBox->currentText()+" WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
        query.next();
        y[i] =query.value(0).toDouble(0);
        if(value<y[i])
            value=y[i];
//        query.exec("SELECT DAMAGE FROM "+ui->label_10->text()+"_"+ui->comboBox->currentText()+" WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
//        query.next();
//        D[i] =query.value(0).toDouble(0);
    }
    QCPGraph *graph1 = ui->customPlot->addGraph();
    graph1->setData(x, y);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 5));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));
    graph1->setName("Net-Total");

//    QCPGraph *graph2 = ui->customPlot->addGraph();
//    graph2->setData(x, D);
//    graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::red, 1.5), QBrush(Qt::white), 5));
//    graph2->setPen(QPen(QColor( 255, 51, 57), 2));
//    graph2->setName("Damage");

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Periodic Records");
    ui->customPlot->yAxis->setLabel("No.of Counts");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, (ui->listWidget->count()+2));
    ui->customPlot->yAxis->setRange(0, (value+10));
    ui->customPlot->legend->setVisible(true);
    //ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customPlot->replot();
}

void MainWindow::on_commandLinkButton_5_clicked()
{
    if(ui->label_10->text().isEmpty())
    {
        QMessageBox::information(this,"Information","Please Open a Profile.",QMessageBox::Ok);
        return;
    }
    ui->customPlot_3->clearGraphs();
    ui->customPlot_4->clearGraphs();
    ui->customPlot_5->clearGraphs();
    QVector<double> x(101), y(101),D(101),M(101); // initialize with entries 0..100
    QSqlQuery query;
    int Total=0,damage=0,price=0;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        x[i] = i; // x goes from -1 to 1
        query.exec("SELECT NET_TOTAL FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
        query.next();
        y[i] =query.value(0).toDouble(0);
        if(Total<y[i])
            Total=y[i];
        query.exec("SELECT DAMAGE FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
        query.next();
        D[i] =query.value(0).toDouble(0);
        if(damage<D[i])
            damage=D[i];
        query.exec("SELECT PRICE_OF_ONE FROM "+ui->label_10->text()+"_TOTAL WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
        query.next();
        M[i] =query.value(0).toDouble(0);
        if(price<M[i])
            price=M[i];
    }

    // generate some data:


    // create graph and assign data to it:
    QCPGraph *graph1 = ui->customPlot_3->addGraph();
    graph1->setData(x, y);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 5));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));
    graph1->setName("Net-Total");

    QCPGraph *graph2 = ui->customPlot_4->addGraph();
    graph2->setData(x, D);
    graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::red, 1.5), QBrush(Qt::white), 5));
    graph2->setPen(QPen(QColor( 255, 51, 57), 2));
    graph2->setName("Damage");

    QCPGraph *graph3 = ui->customPlot_5->addGraph();
    graph3->setData(x, M);
    graph3->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::yellow, 1.5), QBrush(Qt::white), 5));
    graph3->setPen(QPen(QColor( 254, 197, 67), 2));
    graph3->setName("Price of one");

    QCustomPlot *custplot[3]={ui->customPlot_3,ui->customPlot_4,ui->customPlot_5};
    int value[3]={Total,damage,price};
    for(int i=0;i<3;i++)
    {
        // give the axes some labels:
        custplot[i]->xAxis->setLabel("Periodic Records");
        custplot[i]->yAxis->setLabel("No.of Counts");
        // set axes ranges, so we see all data:
        custplot[i]->xAxis->setRange(0, (ui->listWidget->count()+2));
        custplot[i]->yAxis->setRange(0, (value[i]+10));
        custplot[i]->legend->setVisible(true);
        custplot[i]->replot();
    }
    custplot[2]->xAxis->setLabel("Periodic Records");
    custplot[2]->yAxis->setLabel("Price");
    ui->comboBox_2->clear();
    query.exec("SELECT FEILD FROM PROFILE WHERE PROFILE_NAME='"+ui->label_10->text()+"'");
    query.next();
    while(!(query.value(0).toString().isEmpty()))
    {
        ui->comboBox_2->addItem(query.value(0).toString());
        query.next();
    }
    on_comboBox_2_activated(ui->comboBox_2->itemText(0));
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
    ui->customPlot_6->clearGraphs();
    ui->customPlot_7->clearGraphs();
    QVector<double> x(101), y(101),D(101);// initialize with entries 0..100
    QSqlQuery query;
    int Total=0,Damage=0;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        x[i] = i; // x goes from -1 to 1
        query.exec("SELECT TOTAL FROM "+ui->label_10->text()+"_"+arg1+" WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
        query.next();
        y[i] =query.value(0).toDouble(0);
        if(Total<y[i])
            Total=y[i];
        query.exec("SELECT DAMAGE FROM "+ui->label_10->text()+"_"+arg1+" WHERE RECORD='"+ui->listWidget->item(i)->text()+"'");
        query.next();
        D[i] =query.value(0).toDouble(0);
        if(Damage<D[i])
            Damage=D[i];
    }
    QCPGraph *graph1 = ui->customPlot_6->addGraph();
    graph1->setData(x, y);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 5));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));
    graph1->setName("Net-Total");

    QCPGraph *graph2 = ui->customPlot_7->addGraph();
    graph2->setData(x, D);
    graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::red, 1.5), QBrush(Qt::white), 5));
    graph2->setPen(QPen(QColor( 255, 51, 57), 2));
    graph2->setName("Damage");

    QCustomPlot *CustPlot[2]={ui->customPlot_6,ui->customPlot_7};
    int value[2]={Total,Damage};
    for(int i=0;i<2;i++)
    {
        // give the axes some labels:
        CustPlot[i]->xAxis->setLabel("Periodic Records");
        CustPlot[i]->yAxis->setLabel("No.of Counts");
        // set axes ranges, so we see all data:
        CustPlot[i]->xAxis->setRange(0, (ui->listWidget->count()+2));
        CustPlot[i]->yAxis->setRange(0, (value[i]+10));
        CustPlot[i]->legend->setVisible(true);
        CustPlot[i]->replot();
    }
}
