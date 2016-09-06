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
	void setComplete(bool complete) { if(_complete != complete) { _complete = complete; emit completeChanged(); } }
    bool isComplete() const;
private:
	bool _complete;


};

#endif // CUSTOMWIZARDPAGE_H
