#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wizard = new CalibrationWizard(this);

	connect(ui->actionImport_Settings, SIGNAL(triggered(bool)), this, SLOT(triggeredImport(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonCalibrationWizard_clicked()
{
    wizard->exec();
}

void MainWindow::triggeredImport(bool checked)
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open XML config", "", "compressed XML (*.xml.gz)");
	std::string file = QtOpencvCore::qstr2str(fileName);

	cv::FileStorage fs(file, cv::FileStorage::READ);


	fs["cameraMatrix"] >> this->cameraMatrix; //not really needed
	fs["distCoeffs"] >> this->distCoeffs;		//same

	fs["remapXCam"] >> this->remapXCam;
	fs["remapYCam"] >> this->remapYCam;
	fs["isForward"] >> this->isForward;
	fs["remapXWarp"] >> this->remapXWarp;
	fs["remapYWarp"] >> this->remapYWarp;



	fs.release();
}
