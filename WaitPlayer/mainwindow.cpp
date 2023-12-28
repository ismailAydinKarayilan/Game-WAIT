#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mSocket.connectToHost(QHostAddress::LocalHost,1234);

    connect(&mSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QByteArray data = ui->lineEdit->text().toUtf8();
    mSocket.write(data);
}

void MainWindow::readyRead()
{
    static int i = 0;
    QString input = mSocket.readAll();

    QStringList inputList = input.split(" ");
    if(inputList[0] == QString("gameStarting")){

        i++;

    }
    else if(i ==1){


        for (QString item : inputList){

            if(item.contains("gameStarted")){
                i--;
                if (item != "gameStarted"){
                    item.remove("gameStarted");
                    cardList.append(item);

                }
                QString output = cardList[0];
                for (int i = 1; i < cardList.size();i++){
                    output.append(" " + cardList[i].toStdString());
                }
                ui->label->setText(output);
            }
            else{
                cardList.append(input.split("n")[0]);
            }
        }

    }
    else if(inputList[0].contains("gameStarting")){

        inputList.pop_back();
        i++;
        for (QString item : inputList){
            if(item.contains("gameStarted")){
                i--;
                if (item != "gameStarted"){
                    item.remove("gameStarted");
                    cardList.append(item);

                }
                QString output = cardList[0];
                for (int i = 1; i < cardList.size();i++){
                    output.append(" " + cardList[i].toStdString());
                }
                ui->label->setText(output);
            }
            cardList.append(input.split("n")[0]);
        }


    }
    else
    {
        ui->label->setText(input);
    }
}



