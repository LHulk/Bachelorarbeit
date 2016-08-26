#include "CustomWizardPage.h"

CustomWizardPage::CustomWizardPage(QWidget *parent) : QWizardPage(parent), _complete(false)
{

}

bool CustomWizardPage::isComplete() const
{
	return _complete;
}



 CustomWizardPage::~CustomWizardPage()
 {

 }
