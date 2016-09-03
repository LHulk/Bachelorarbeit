#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wizard = new CalibrationWizard(this);

	this->hasSettings = false;
	this->hasFiles = false;

	this->model = new QStringListModel(this);
	ui->listViewFiles->setModel(this->model);


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
	fs["cameraMatrix"] >> this->cameraMatrix;
	fs["distCoeffs"] >> this->distCoeffs;

	fs["isForward"] >> this->isForward;
	fs["remapXWarp"] >> this->remapXWarp;
	fs["remapYWarp"] >> this->remapYWarp;
	fs.release();

	this->hasSettings = true;

	if(hasSettings && hasFiles)
		ui->buttonUnfold->setEnabled(true);
}

void MainWindow::on_ButtonLoadFiles_clicked()
{
	this->files = QFileDialog::getOpenFileNames(this, "Select images", "", "Images (*.png *.jpg *.jpeg *.PNG *.JPG, *.JPEG)" );

	this->model->setStringList(files);

	this->hasFiles = true;

	if(hasSettings && hasFiles)
		ui->buttonUnfold->setEnabled(true);
}

void MainWindow::on_buttonUnfold_clicked()
{
	cv::Mat tempImg = cv::imread(QtOpencvCore::qstr2str(files.at(0)), CV_LOAD_IMAGE_GRAYSCALE);
	cv::Size imgSize = tempImg.size();

	cv::Mat optimalNew = cv::Mat();
	cv::initUndistortRectifyMap(this->cameraMatrix, this->distCoeffs, optimalNew, this->cameraMatrix, imgSize, CV_32FC1, remapXCam, remapYCam);

	for(const QString& qstr : files)
	{
		cv::Mat img = cv::imread(QtOpencvCore::qstr2str(qstr), CV_LOAD_IMAGE_GRAYSCALE);

		if(img.empty())
			continue;
		cv::Mat unwarpedCam;
		cv::remap(img, unwarpedCam, remapXCam, remapYCam, cv::INTER_CUBIC);

		cv::Mat unwarpedCone;
		cv::remap(unwarpedCam, unwarpedCone, remapXWarp, remapYWarp, cv::INTER_CUBIC);

	}

}










