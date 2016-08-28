#ifndef CALIBRATIONWIZARD_H
#define CALIBRATIONWIZARD_H

//CV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

//QT
#include <QWizard>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDebug>
#include <QtAlgorithms>

//STD
#include <vector>

//USER
#include "CustomGraphicsView.h"
#include "QtOpencvCore.hpp"
#include "CustomWizardPage.h"
#include "EdgeDetection.h"
#include "Ellipse.h"
#include "Line.h"
#include "DotDetection.h"
#include "Config.h"
#include "Transformation.h"
#include "Cone.h"
#include "Calibration.h"
#include <thread>

namespace Ui {
class CalibrationWizard;
}

class CalibrationWizard : public QWizard
{
	Q_OBJECT

public:
	explicit CalibrationWizard(QWidget *parent = 0);
	~CalibrationWizard();


private:
	void drawKeyPoints();
	void refresh();

private slots:
	void on_buttonLoadIntrinsic_clicked();
	void on_buttonStartIntrinsic_clicked();

	void on_buttonZoomPBlob_clicked();
	void on_buttonZoomMBlob_clicked();

	void on_buttonLoadCone_clicked();

	void on_buttonFindBlobs_clicked();
	void clickedInBlobView(QPointF point, int button);

	void on_buttonGetMappings_clicked();

	void on_buttonUnfold_clicked();

private:
	Ui::CalibrationWizard *ui;
	QStringList fileNamesCamCalib;
	QString fileNameConeCalib;
	QGraphicsScene* scene1;
	QGraphicsScene* scene2;

	Cone cone;

	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	cv::Mat remapXCam;
	cv::Mat remapYCam;

	cv::Mat grey;
	cv::Mat greyOriginal;
	std::vector<cv::Point2f> keyPoints;

	cv::Mat projectionMatrix;

	cv::Mat remapXWarp;
	cv::Mat remapYWarp;
	bool isForward;




};

#endif // CALIBRATIONWIZARD_H
