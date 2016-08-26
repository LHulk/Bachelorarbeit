#include "CalibrationWizard.h"
#include "ui_CalibrationWizard.h"

CalibrationWizard::CalibrationWizard(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::CalibrationWizard)
{
	ui->setupUi(this);

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


	ui->buttonStartIntrinsic->setEnabled(false);
	ui->buttonFindBlobs->setEnabled(false);


	ui->labelIntrinsicStatus->setText("");
	ui->labelBlobStatus->setText("");


	scene = new QGraphicsScene();

	connect(ui->viewBlob, SIGNAL(mousePressedInView(QPointF, int)), this, SLOT(clickedInBlobView(QPointF, int)));

}

CalibrationWizard::~CalibrationWizard()
{
	delete ui;
}

void CalibrationWizard::on_buttonLoadIntrinsic_clicked()
{
	fileNamesCamCalib = QFileDialog::getOpenFileNames(this, "Select calibration images", "D:/lars/Documents/gitProjects/BA-Haalck-2016/img/calibration/chessboard", "Images (*.png *.jpg *.jpeg *.PNG *.JPG, *.JPEG)" );

	if(fileNamesCamCalib.size() > 0)
	{
		ui->buttonStartIntrinsic->setEnabled(true);
		ui->labelIntrinsicStatus->setText("");
	}
	else
		ui->labelIntrinsicStatus->setText("No images selected...");
}

void CalibrationWizard::on_buttonLoadCone_clicked()
{
	fileNameConeCalib = QFileDialog::getOpenFileName(this, "Select calibration image", "D:/lars/Documents/gitProjects/BA-Haalck-2016/img/calibration/chessboard", "Images (*.png *.jpg *.jpeg *.PNG *.JPG, *.JPEG)" );

	if(fileNameConeCalib != "")
	{
		ui->buttonFindBlobs->setEnabled(true);
		ui->labelBlobStatus->setText("");
	}
	else
		ui->labelBlobStatus->setText("No images selected...");

}

void CalibrationWizard::on_buttonStartIntrinsic_clicked()
{
	ui->progressIntrinsic->setValue(0);
	ui->progressIntrinsic->setMaximum(fileNamesCamCalib.size() - 1);

	std::vector<std::vector<cv::Point2f>> imagePoints;
	std::vector<std::vector<cv::Point3f>> objectPoints;

	std::vector<cv::Point3f> currentobjectPoints = Calibration::getObjectPoints();
	cv::Size size;
	for(int i = 0; i < fileNamesCamCalib.size(); i++)
	{
		std::string fileName = fileNamesCamCalib.at(i).toStdString();
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

void CalibrationWizard::drawKeyPoints()
{
	cv::Mat imgKey;
	cv::cvtColor(grey, imgKey, CV_GRAY2BGR);

	for(const auto& pt : keyPoints)
		cv::circle(imgKey, pt, 4, cv::Scalar(0,255,0), 2);

	scene->addPixmap(QtOpencvCore::img2qpix(imgKey));
	ui->viewBlob->setScene(scene);
	ui->viewBlob->show();

	if(keyPoints.size() != Config::numCircleSamples * Config::numLineSamples)
	{
		ui->labelBlobStatus->setText(QtOpencvCore::str2qstr(std::string("") + "not the right amount blobs detected. needed: " + std::to_string(Config::numCircleSamples * Config::numLineSamples) +
									 ", found: " + std::to_string(keyPoints.size())));
		ui->wizPage2->setComplete(false);
	}
	else
	{
		ui->labelBlobStatus->setText("");
		ui->wizPage2->setComplete(true);
	}

	std::ostringstream stream;
	for(const auto& pt : keyPoints)
		stream << pt << "\n";

	ui->textFoundBlobs->setText(QtOpencvCore::str2qstr(stream.str()));

}

void CalibrationWizard::on_buttonFindBlobs_clicked()
{
	std::string fileName = fileNameConeCalib.toStdString();
	grey = cv::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
	cv::resize(grey, grey, cv::Size(1000, 1000 * grey.rows / grey.cols));
	Config::usedResWidth = 1000;
	Config::usedResHeight = 1000 * grey.rows / grey.cols;

	keyPoints = DotDetection::detectDots(grey);

	drawKeyPoints();
	ui->viewBlob->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


void CalibrationWizard::clickedInBlobView(QPointF point, int button)
{
	if(point.x() > 0 && point.y() > 0 && point.x() < grey.cols && point.y() < grey.rows)
	{
		if(button == 0)
		{
			keyPoints.push_back(cv::Point2f(point.x(), point.y()));
		}
		else
		{
			auto minElementIt = std::min_element(keyPoints.begin(), keyPoints.end(), [point](const cv::Point2f p1, const cv::Point2f p2)
			{
				return cv::norm(p1 - cv::Point2f(point.x(), point.y())) < cv::norm(p2 - cv::Point2f(point.x(), point.y()));
			});

			keyPoints.erase(minElementIt);

		}

		drawKeyPoints();
	}


}





void CalibrationWizard::on_buttonZoomPBlob_clicked()
{
	ui->viewBlob->scale(1.15, 1.15);
}

void CalibrationWizard::on_buttonZoomMBlob_clicked()
{
	ui->viewBlob->scale(1/1.15, 1/1.15);
}



