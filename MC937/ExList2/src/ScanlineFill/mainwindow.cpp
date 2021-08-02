/*
 * MC937 - Computer Graphics
 * Exercise List 2 - Exercise 2: Scanline Fill Algorithm.
 *
 * Qt application to draw a polygon and fill using the Scanline Fill Algorithm.
 * MainWindow: Main Window of the application. Mostly interacts with buttons.
 *
 * Author:
 *      Victor Ferreira Ferrari - RA 187890
 *
 * Last Modified: 29/03/2020
 */

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

// Draw button: draws edges, without filling.
void MainWindow::on_drawPushButton_clicked(){
    ui->drawingAreaWidget->setMode('d');
    ui->drawingAreaWidget->repaint();
    changeFill(true);
}

// Fill button: fills polygon using the Scanline Fill Algorithm.
void MainWindow::on_fillPushButton_clicked(){
    ui->drawingAreaWidget->setMode('f');
    ui->drawingAreaWidget->update();
}

// Enables or disables fill button.
void MainWindow::changeFill(bool state){
    ui->fillPushButton->setEnabled(state);
}

// Color button: chooses pen color for drawing and filling.
void MainWindow::on_colorToolButton_clicked(){
    QColor color;
    color = QColorDialog::getColor(Qt::white, this);
    ui->drawingAreaWidget->setColor(color);
    ui->drawingAreaWidget->update();
}

// Clear button: clears the drawing area, and disables fill button.
void MainWindow::on_clearPushButton_clicked(){
    ui->drawingAreaWidget->clear();
    ui->drawingAreaWidget->update();
    changeFill(false);
}
