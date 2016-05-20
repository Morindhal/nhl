#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <iostream>


ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow),
    port(1234)
{
    ui->setupUi(this);
    centralWidget()->findChild<QLabel*>("serverState")->setText("Server is currently OFF");
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::on_pushButton_clicked()
{
    bool debug = false;

    if(!server)
    {
        qDebug() << "New server!";
        server = new Server(port, debug);
    }
    ServerWindow::updateServerState();
}

void ServerWindow::respond()
{
    int temp = 123;
}

void ServerWindow::on_pushButton_2_clicked()
{
    if(server)
    {
        qDebug() << "Kill server!";
        delete server;
        server = NULL;
    }
    ServerWindow::updateServerState();
}

void ServerWindow::updateServerState()
{
    if(!server)
        centralWidget()->findChild<QLabel*>("serverState")->setText("Server is currently OFF");
    else
        centralWidget()->findChild<QLabel*>("serverState")->setText("Server is currently ON");
}
