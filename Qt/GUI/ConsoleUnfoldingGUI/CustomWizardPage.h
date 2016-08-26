#ifndef CUSTOMWIZARDPAGE_H
#define CUSTOMWIZARDPAGE_H

#include <QObject>
#include <QWizardPage>

class CustomWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CustomWizardPage(QWidget *parent = 0);
    ~CustomWizardPage();
    bool CustomWizardPage::isComplete() const;

};

#endif // CUSTOMWIZARDPAGE_H
