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

private slots:
    void on_buttonLoadIntrinsic_clicked();
    void on_buttonStartIntrinsic_clicked();

	void on_buttonZoomPBlob_clicked();
	void on_buttonZoomMBlob_clicked();

	void on_buttonLoadCone_clicked();

	void on_buttonFindBlobs_clicked();
	void clickedInBlobView(QPointF point, int button);

private:
    Ui::CalibrationWizard *ui;
	QStringList fileNamesCamCalib;
	QString fileNameConeCalib;
	QGraphicsScene* scene;

	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

	cv::Mat grey;
	std::vector<cv::Point2f> keyPoints;

};

#endif // CALIBRATIONWIZARD_H
