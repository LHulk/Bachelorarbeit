#ifndef CALIBRATIONWIZARD_H
#define CALIBRATIONWIZARD_H

#include <QWizard>

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
    Ui::CalibrationWizard *ui;
};

#endif // CALIBRATIONWIZARD_H
