// qsynthTuningsForm.cpp
//

#include "qsynthAbout.h"
#include "qsynthTuningsForm.h"

#include "qsynthMainForm.h"

#include <QShowEvent>
#include <QHideEvent>


qsynthTuningsForm::qsynthTuningsForm (
        QWidget *pParent, Qt::WindowFlags wflags )
        : QWidget(pParent, wflags) //HELP: Should QWidget or QDialog be used?
{
    m_ui.setupUi(this);

    m_pOptions = NULL;

    //HELP: Are these needed? Copied from qsynthChannelsForm.cpp.
    m_iDirtySetup = 0;
    m_iDirtyCount = 0;

}


qsynthTuningsForm::~qsynthTuningsForm (void)
{

}


// Copied from qsynthChannelsForm.cpp or qsynthMessagesForm.cpp.
void qsynthTuningsForm::showEvent (QShowEvent *pShowEvent )
{
    qsynthMainForm *pMainForm = qsynthMainForm::getInstance();
    if (pMainForm)
        pMainForm->stabilizeForm();

    QWidget::showEvent(pShowEvent);
}

// Copied from qsynthChannelsForm.cpp or qsynthMessagesForm.cpp.
void qsynthTuningsForm::hideEvent ( QHideEvent *pHideEvent )
{
    QWidget::hideEvent(pHideEvent);

    qsynthMainForm *pMainForm = qsynthMainForm::getInstance();
    if (pMainForm)
        pMainForm->stabilizeForm();
}

// Copied from qsynthChannelsForm.cpp or qsynthMessagesForm.cpp.
void qsynthTuningsForm::closeEvent (QCloseEvent * /*pCloseEvent*/ )
{
    QWidget::hide();

    qsynthMainForm *pMainForm = qsynthMainForm::getInstance();
    if (pMainForm)
        pMainForm->stabilizeForm();
}
