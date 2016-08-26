#include "CalibrationWizard.h"
#include "ui_CalibrationWizard.h"

CalibrationWizard::CalibrationWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CalibrationWizard)
{
    ui->setupUi(this);
}

CalibrationWizard::~CalibrationWizard()
{
    delete ui;
}
