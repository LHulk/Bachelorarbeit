#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wizard = new CalibrationWizard(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonCalibrationWizard_clicked()
{
    wizard->exec();
}
