#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    tcpSocket(new QTcpSocket(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpSocket->connectToHost("127.0.0.1", 1234);
    connect(tcpSocket, &QIODevice::readyRead, this, &MainWindow::readData);
}

void MainWindow::readData() {
    QByteArray localReadAll = tcpSocket->readAll();
    ui->dataLabel->setText(localReadAll);
}

MainWindow::~MainWindow()
{
    delete ui;
}
