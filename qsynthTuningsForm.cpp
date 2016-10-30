// qsynthTuningsForm.cpp
//

#include "qsynthAbout.h"
#include "qsynthTuningsForm.h"

#include "qsynthTuning.h"
#include "qsynthEngine.h"

#include "qsynthMainForm.h"

#include <QShowEvent>
#include <QHideEvent>
#include <QFileDialog>
#include <QPixmap>


qsynthTuningsForm::qsynthTuningsForm (
		QWidget *pParent, Qt::WindowFlags wflags )
	: QWidget(pParent, wflags)
{
	m_ui.setupUi(this);

	m_iTuning  = 0;
	m_ppTuning = NULL;

	m_pOptions = NULL;

	//HELP: Are these needed? Copied from qsynthChannelsForm.cpp.
	m_iDirtySetup = 0;
	m_iDirtyCount = 0;

	m_pXpmLedOn  = new QPixmap(":/images/ledon1.png");
	m_pXpmLedOff = new QPixmap(":/images/ledoff1.png");

	//HELP: Am I doing this right?
	m_ui.TuningsListView->sortItems(1, Qt::DescendingOrder);
	
	QObject::connect(m_ui.TuningsListView,
		SIGNAL(customContextMenuRequested(const QPoint&)),
		SLOT(itemActivated(QTreeWidgetItem*,int)));
	QObject::connect(m_ui.TuningsListView,
		SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
		SLOT(itemActivated(QTreeWidgetItem*,int)));
}


qsynthTuningsForm::~qsynthTuningsForm (void)
{
	//	setup(NULL, NULL, false);
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

void qsynthTuningsForm::setup ( qsynthOptions *pOptions,
	qsynthEngine *pEngine, bool bTuning )
{
	m_pOptions = pOptions;
	m_pEngine  = pEngine;
	m_pSynth   = pEngine ? pEngine->pSynth : NULL;
	
	// Make caption. (ie: Qsynth Tunings)
	QString sTitle = QSYNTH_TITLE ": " + tr("Tunings");
	if (pEngine)
		sTitle += " [" + pEngine->name() + "]";
	setWindowTitle(sTitle);

	if (m_ppTuning) {
		delete [] m_ppTuning;
		m_ppTuning = NULL;
		m_iTuning = 0;
	}

	m_ui.TuningsListView->clear();
	if (m_pSynth && m_ppTuning == NULL) {
//		m_iTuning = 128; Don't think we need this, as it will always be 128.
		for (int iTune = 1; iTune < 128; iTune++) {
			qsynthTuning *pItem = new qsynthTuning(
				m_ui.TuningsListView);
			if (pItem) {
				pItem->setIcon(QSYNTH_TUNINGS_IN,
					*m_pXpmLedOff);
				pItem->setText(QSYNTH_TUNINGS_NOTENUM,
					QString::number(iTune + 1));
			}
			m_ppTuning[iTune] = pItem;
		}

		m_iDirtySetup++;
		m_iDirtySetup--;
	}
}

// Tunings item update. Derived from the Channels item update. HELP: Don't understand this well.
void qsynthTuningsForm::updateTuning ( int iTune )
{
	if (m_pSynth == NULL || m_ppTuning == NULL)
		return;
	if (iTune < 0 || iTune >= m_iTuning)
		return;

	qsynthTuning *pItem = m_ppTuning[iTune];
}

void qsynthTuningsForm::itemActivated ( QTreeWidgetItem *pItem, int )
{
	if (pItem == NULL)
		return;
	if (m_ppTuning == NULL)
		return;
	if (m_pOptions == NULL || m_pEngine == NULL || m_pSynth == NULL)
		return;
}

//WIP: Will be to open and load a sysex file.
/*void qsynthTuningsForm::openSysex (void)
{
	QStringList syxfiles = QFileDialog::getOpenFileName(
		this,
		QSYNTH_TITLE ": " + tr("sysex files"),
		"~",
		tr("sysex files (*)"));
}
*/
