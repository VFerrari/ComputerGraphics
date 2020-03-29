#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_drawPushButton_clicked(){
    ui->drawingAreaWidget->setMode('d');
    ui->drawingAreaWidget->repaint();
    ui->fillPushButton->setEnabled(true);
}

void MainWindow::on_fillPushButton_clicked(){
    ui->drawingAreaWidget->setMode('f');
    ui->drawingAreaWidget->update();
}

void MainWindow::on_colorToolButton_clicked(){
    QColor color;
    color = QColorDialog::getColor(Qt::white, this);
    ui->drawingAreaWidget->setColor(color);
    ui->drawingAreaWidget->update();
}

void MainWindow::on_clearPushButton_clicked(){
    ui->drawingAreaWidget->clear();
    ui->drawingAreaWidget->update();
    ui->fillPushButton->setEnabled(false);
}
