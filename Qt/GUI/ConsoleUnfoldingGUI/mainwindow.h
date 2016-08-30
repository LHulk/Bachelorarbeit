#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CalibrationWizard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_buttonCalibrationWizard_clicked();
	void triggeredImport(bool);

private:
    Ui::MainWindow *ui;
    CalibrationWizard* wizard;

	//stored
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	cv::Mat remapXCam;
	cv::Mat remapYCam;

	bool isForward;
	cv::Mat remapXWarp;
	cv::Mat remapYWarp;
};

#endif // MAINWINDOW_H
