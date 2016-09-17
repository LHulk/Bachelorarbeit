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
	this->hasOutput = false;

	evalStatus();

	this->modelIn = new QStringListModel(this);
	ui->listViewFiles->setModel(this->modelIn);
	this->modelOut = new QStringListModel(this);
	ui->listViewFilesOut->setModel(this->modelOut);


	connect(ui->actionImport_Settings, SIGNAL(triggered(bool)), this, SLOT(triggeredImport(bool)));
	connect(ui->actionCalibration_Wizard, SIGNAL(triggered(bool)), this, SLOT(triggeredCalibration(bool)));
	connect(ui->actionSet_Output_Dir, SIGNAL(triggered(bool)), this, SLOT(triggeredSetOutputDir(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::triggeredCalibration(bool checked)
{
    wizard->exec();
}

void MainWindow::triggeredImport(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open XML config", "", "compressed XML (*.xml.gz)", nullptr ,QFileDialog::DontUseNativeDialog);
	if(fileName.length() <= 0)
		return;

	std::string file = QtOpencvCore::qstr2str(fileName);

	cv::FileStorage fs(file, cv::FileStorage::READ);
	fs["cameraMatrix"] >> this->cameraMatrix;
	fs["distCoeffs"] >> this->distCoeffs;

	fs["isForward"] >> this->isForward;
	fs["remapXWarp"] >> this->remapXWarp;
	fs["remapYWarp"] >> this->remapYWarp;
	fs.release();

	this->hasSettings = true;
	evalStatus();
}

void MainWindow::on_ButtonLoadFiles_clicked()
{
    this->files = QFileDialog::getOpenFileNames(this, "Select images", "", "Images (*.png *.jpg *.jpeg *.PNG *.JPG, *.JPEG)", nullptr ,QFileDialog::DontUseNativeDialog);
	if(this->files.size() <= 0)
		return;

	this->modelIn->setStringList(files);

	this->hasFiles = true;
	evalStatus();
}


void MainWindow::on_buttonUnfold_clicked()
{
	cv::Mat tempImg = cv::imread(QtOpencvCore::qstr2str(files.at(0)), CV_LOAD_IMAGE_GRAYSCALE);
	cv::Size imgSize = tempImg.size();

	cv::Mat optimalNew = cv::Mat();
	cv::initUndistortRectifyMap(this->cameraMatrix, this->distCoeffs, optimalNew, this->cameraMatrix, imgSize, CV_32FC1, remapXCam, remapYCam);

	ui->progressBarMain->setValue(0);
	ui->progressBarMain->setMaximum(files.size());
	int counter = 0;
	QStringList processedFiles;
	for(const QString& qstr : files)
	{
		cv::Mat img = cv::imread(QtOpencvCore::qstr2str(qstr), CV_LOAD_IMAGE_GRAYSCALE);

		if(img.empty())
			continue;
		cv::Mat unwarpedCam;
		cv::remap(img, unwarpedCam, remapXCam, remapYCam, cv::INTER_CUBIC);

		cv::Mat unwarpedCone;
		cv::remap(unwarpedCam, unwarpedCone, remapXWarp, remapYWarp, cv::INTER_CUBIC);

		std::string outputFile = outputDir + "/" + QtOpencvCore::qstr2str(qstr.section("/", -1, -1));

		cv::imwrite(outputFile, unwarpedCone);

		processedFiles.push_back(QtOpencvCore::str2qstr(outputFile));
		this->modelOut->setStringList(processedFiles);
		ui->progressBarMain->setValue(++counter);
		QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

	}

}

void MainWindow::triggeredSetOutputDir(bool checked)
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setNameFilter(tr("Directory"));
	dialog.setOptions(QFileDialog::DontUseNativeDialog);

	QStringList fileNames;
	if(dialog.exec())
		fileNames = dialog.selectedFiles();

	QString file;
	if(fileNames.size() > 0)
		file = fileNames.at(0);

	outputDir = QtOpencvCore::qstr2str(file);

	hasOutput = true;
	evalStatus();
}

void MainWindow::evalStatus()
{
	ui->labelStatusMain->setText("");
	QString status;
	if(!hasSettings)
		status += "no settings loaded. ";
	if(!hasFiles)
		status += "no files loaded. ";
	if(!hasOutput)
		status += "no ouput dir specified. ";

	ui->labelStatusMain->setText(status);
	if(hasSettings && hasFiles && hasOutput)
		ui->buttonUnfold->setEnabled(true);


}












