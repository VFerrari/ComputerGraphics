#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_drawPushButton_clicked()
{
    ui->drawingAreaWidget->setMode('d');
    ui->drawingAreaWidget->repaint();
}

void MainWindow::on_fillPushButton_clicked()
{
    ui->drawingAreaWidget->setMode('f');
    ui->drawingAreaWidget->repaint();
}

void MainWindow::on_clearPushButton_clicked()
{

    ui->drawingAreaWidget->clear();
    ui->drawingAreaWidget->repaint();
}
