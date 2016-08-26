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

//STD
#include <vector>

//USER
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


private slots:
    void on_buttonLoadIntrinsic_clicked();
    void on_buttonStartIntrinsic_clicked();

	//void on_buttonZoomPIntrinsic_clicked();
	//void on_buttonZoomMIntrinsic_clicked();

private:
    Ui::CalibrationWizard *ui;
    QStringList fileNames;
	QGraphicsScene* scene;

	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

};

#endif // CALIBRATIONWIZARD_H
