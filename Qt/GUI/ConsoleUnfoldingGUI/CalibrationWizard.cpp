#include "CalibrationWizard.h"
#include "ui_CalibrationWizard.h"

CalibrationWizard::CalibrationWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CalibrationWizard)
{
    ui->setupUi(this);

    ui->buttonStartIntrinsic->setEnabled(false);
}

CalibrationWizard::~CalibrationWizard()
{
    delete ui;
}

void CalibrationWizard::on_buttonLoadIntrinsic_clicked()
{
    fileNames = QFileDialog::getOpenFileNames(this, "Select calibration images", "", "Images (*.png *.jpg *.jpeg *.PNG *.JPG, *.JPEG)" );

    if(fileNames.size() > 0)
    {
        ui->buttonStartIntrinsic->setEnabled(true);
        ui->label_intrinsic_status->setText("");
    }
    else
        ui->label_intrinsic_status->setText("No images selected...");
}

void CalibrationWizard::on_buttonStartIntrinsic_clicked()
{
    fileNames[0]
    for(int i = 0; i < fileNames.size(); i++)
    {

    }
}
