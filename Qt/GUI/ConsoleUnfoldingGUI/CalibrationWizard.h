#ifndef CALIBRATIONWIZARD_H
#define CALIBRATIONWIZARD_H

//CV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

//QT
#include <QWizard>
#include <QFileDialog>

//STD
#include <vector>

//USER
#include "CustomWizardPage.h"
#include "EdgeDetection.h"
#include "Ellipse.h"
#include "Line.h"
#include "DotDetection.h"
#include "Config.h"
#include "Transformation.h"
#include "Cone.h"

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

private:
    Ui::CalibrationWizard *ui;
    QStringList fileNames;

};

#endif // CALIBRATIONWIZARD_H
