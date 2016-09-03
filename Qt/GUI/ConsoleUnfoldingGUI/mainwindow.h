#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
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

	void on_ButtonLoadFiles_clicked();

	void on_buttonUnfold_clicked();

private:
    Ui::MainWindow *ui;
    CalibrationWizard* wizard;
	QGraphicsScene* scene;

	QStringListModel* model;
	QStringList files;

	//stored
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	cv::Mat remapXCam;
	cv::Mat remapYCam;

	bool isForward;
	cv::Mat remapXWarp;
	cv::Mat remapYWarp;

	bool hasSettings;
	bool hasFiles;
};

#endif // MAINWINDOW_H
