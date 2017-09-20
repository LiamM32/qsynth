// qsynthTuningsForm.cpp
//

#include "qsynthAbout.h"
#include "qsynthTuningsForm.h"

#include "qsynthTuning.h"
#include "qsynthEngine.h"

#include "qsynthMainForm.h"

#include <QValidator>  //Testing the need for all four of these.
#include <QHeaderView>
#include <QMessageBox>
#include <QFileInfo>

#include <QShowEvent>
#include <QHideEvent>
#include <QFileDialog>
#include <QPixmap>
#include <QMenu> //testing need


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
	
	// Tunings list view...
	QHeaderView *pHeader = m_ui.TuningsListView->header();
	pHeader->setDefaultAlignment(Qt::AlignLeft);
#if QT_VERSION >=0x050000
	pHeader->setSectionsMovable(false);
#else
	pHeader->setMovable(false);
#endif
	pHeader->setStretchLastSection(true);
	
	pHeader->resizeSection(0, 24);			// In.

	//HELP: Am I doing this right?
	m_ui.TuningsListView->sortItems(1, Qt::DescendingOrder);
	
	// UI connections
/*	QObject::connect(m_ui.TuningsListView,
		SIGNAL(customContextMenuRequested(const QPoint&)),
		SLOT(contextMenuRequested(QTreeWidgetItem*,int)));*/
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
	qsynthEngine *pEngine, bool bTuning ) //HELP: Do I need bTuning?
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
		m_iTuning = 128; //Don't think we need this, as it will always be 128.
		m_ppTuning = new qsynthTuningPtr [m_iTuning];
		for (int iTune = 0; iTune < 128; iTune++) {
			qsynthTuning *pItem = new qsynthTuning(
				m_ui.TuningsListView);
			if (pItem) {
				pItem->setIcon(QSYNTH_TUNINGS_IN,
					*m_pXpmLedOff);
				pItem->setText(QSYNTH_TUNINGS_KEYNUM,
					QString::number(iTune + 1));
			}
			m_ppTuning[iTune] = pItem;
		}

		m_iDirtySetup++;
		m_iDirtySetup--;
		resetAllKeyTunings();
	}
}

// Tunings item update. Derived from the Channels item update. HELP: Don't understand this well.
void qsynthTuningsForm::updateKeyTuning (int iKey, double keytuningcents )
{
	if (m_pSynth == NULL || m_ppTuning == NULL)
		return;
	if (iKey < 0 || iKey >= m_iTuning)
		return;

	/* Because the tuning features in fluidsynth are designed to hold multiple tunings,
	 * but this isn't needed most of the time, Qsynth could eventually have a "default" tuning
	 * initialized at bank 0 prog 0 which all channels are set to by default. */
	
	qsynthTuning *pItem = m_ppTuning[iKey];
	
	
	pItem->setText(QSYNTH_TUNINGS_DETUNE,
		QString::number(keytuningcents));
}

void qsynthTuningsForm::updateAllKeyTunings ()
{
	double tuningcents[128];
	int tuningBank = 0; //HELP: Don't really know how tuning bank should be determined.
	int tuningProg = 0; //HELP: Don't know what the "program" should be either.
	::fluid_synth_tuning_dump(m_pSynth, tuningBank, tuningProg, NULL, 0, tuningcents);
	for (int iKey = 0; iKey < 128; iKey++)
		updateKeyTuning(iKey, tuningcents[iKey]);
	
	m_ui.TuningsListView->update();
	m_ui.TuningsListView->sortItems(1, Qt::DescendingOrder);
	
	stabilizeForm();
}

void qsynthTuningsForm::resetAllKeyTunings ()
{
	if (m_pEngine == NULL)
		return;
	
	qsynthSetup *pSetup = m_pEngine->setup();
	if (pSetup == NULL)
		return;
	
	updateAllKeyTunings();
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

// Stabilize current form state.
void qsynthTuningsForm::stabilizeForm (void)
{
	if (m_pEngine == NULL)
		return;

	qsynthSetup *pSetup = m_pEngine->setup();
	if (pSetup == NULL)
		return;
}
