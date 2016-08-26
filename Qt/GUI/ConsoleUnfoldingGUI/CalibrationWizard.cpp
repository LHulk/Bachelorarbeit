#include "CalibrationWizard.h"
#include "ui_CalibrationWizard.h"

CalibrationWizard::CalibrationWizard(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::CalibrationWizard)
{
	ui->setupUi(this);

	ui->buttonStartIntrinsic->setEnabled(false);

	//set default values
	ui->spinPatternLineSamples->setValue(Config::numLineSamples);
	ui->spinPatternCircleSamples->setValue(Config::numCircleSamples);

	ui->spinPatternCols->setValue(Config::patternPointsPerCol);
	ui->spinPatternRows->setValue(Config::patternPointsPerRow);
	ui->spinPatternSquareSize->setValue(Config::patternSquareSize);

	ui->spinSlantRes->setValue(Config::resSlantHeight);

	ui->spinConer->setValue(Config::radiusInner);
	ui->spinConeR->setValue(Config::radiusOuter);
	ui->spinConeh->setValue(Config::height);
	ui->checkConeIsToTip->setChecked(Config::whichHeight == CENTER_TO_TIP);

	scene = new QGraphicsScene();

}

CalibrationWizard::~CalibrationWizard()
{
	delete ui;
}

void CalibrationWizard::on_buttonLoadIntrinsic_clicked()
{
	fileNames = QFileDialog::getOpenFileNames(this, "Select calibration images", "D:/lars/Documents/gitProjects/BA-Haalck-2016/img/calibration/chessboard", "Images (*.png *.jpg *.jpeg *.PNG *.JPG, *.JPEG)" );

	if(fileNames.size() > 0)
	{
		ui->buttonStartIntrinsic->setEnabled(true);
		ui->labelIntrinsicStatus->setText("");
	}
	else
		ui->labelIntrinsicStatus->setText("No images selected...");
}

void CalibrationWizard::on_buttonStartIntrinsic_clicked()
{
	ui->progressIntrinsic->setValue(0);
	ui->progressIntrinsic->setMaximum(fileNames.size() - 1);

	std::vector<std::vector<cv::Point2f>> imagePoints;
	std::vector<std::vector<cv::Point3f>> objectPoints;

	std::vector<cv::Point3f> currentobjectPoints = Calibration::getObjectPoints();
	cv::Size size;
	for(int i = 0; i < fileNames.size(); i++)
	{
		std::string fileName = fileNames.at(i).toStdString();
		cv::Mat img = cv::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
		cv::resize(img, img, cv::Size(1000, 1000 * img.rows / img.cols));
		if(i == 0)
			size = img.size();

		cv::Mat imgWithCorners;
		std::vector<cv::Point2f> currentImagePoints = Calibration::getCorners(img, imgWithCorners);

		/*scene->addPixmap(QtOpencvCore::img2qpix(imgWithCorners));
		ui->viewIntrinsic->setScene(scene);
		ui->viewIntrinsic->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
		ui->viewIntrinsic->show();*/

		imagePoints.push_back(currentImagePoints);
		objectPoints.push_back(currentobjectPoints);
		ui->progressIntrinsic->setValue(i);
	}

	Calibration::getIntrinsicParamters(imagePoints, objectPoints, this->cameraMatrix, this->distCoeffs, size);

	std::ostringstream stream;
	stream << "Camera Matrix: \n" << cameraMatrix << "\n\n" << "Distortion coefficients: \n" << distCoeffs;

	ui->textCameraMatrix->setText(QtOpencvCore::str2qstr(stream.str()));

	ui->wizPage1->setComplete(true);


}





/*void CalibrationWizard::on_buttonZoomPIntrinsic_clicked()
{
	ui->viewIntrinsic->scale(1.15, 1.15);
}

void CalibrationWizard::on_buttonZoomMIntrinsic_clicked()
{
	ui->viewIntrinsic->scale(1/1.15, 1/1.15);
}*/
