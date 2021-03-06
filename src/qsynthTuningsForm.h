#ifndef __qsynthTuningsForm_h
#define __qsynthTuningsForm_h

#include "ui_qsynthTuningsForm.h"

#include <fluidsynth.h>
#include <QObject>


class qsynthOptions;
class qsynthEngine;

class qsynthTuning;

class QPixmap;


class qsynthTuningsForm : public QWidget
{
	Q_OBJECT

public:

	qsynthTuningsForm(QWidget *pParent = 0, Qt::WindowFlags wflags = 0);
	~qsynthTuningsForm();

	void setup(qsynthOptions *pOptions, qsynthEngine *pEngine, bool bTuning/*TODO:Or change to bNew or bPreset*/);

	void setKeyOn(int iKey, bool bOn);
	void resetAllKeyTunings(bool bTuning);
	void updateKeyTuning(int iKey, double tuningcents);
	
	friend int fluid_synth_tuning_dump(fluid_synth_t* synth, int bank, int prog, char* name, int len, double* pitch);
	
	double tuningcents[128];
	double* pTuningcents;	

public slots:

	void itemActivated(QTreeWidgetItem*,int);
	
	void updateAllKeyTunings();

//	void contextMenuRequested(const QPoint&); //HELP: What is this for?

protected:

	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);
	void closeEvent(QCloseEvent *);
	
	void stabilizeForm();

	void openSysex();

private:

	Ui::qsynthTuningsForm m_ui;

	int m_iTuning = 128;

	qsynthTuning **m_ppTuning;

	qsynthOptions *m_pOptions;
	qsynthEngine  *m_pEngine;

	fluid_synth_t *m_pSynth;
	
//	double tuningcents[128];
//	double* pTuningcents;

	int m_iDirtySetup;
	int m_iDirtyCount;

	QPixmap *m_pXpmLedOn;
	QPixmap *m_pXpmLedOff;
};

#endif // __qsynthTuningsForm_h
