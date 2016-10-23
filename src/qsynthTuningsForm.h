#ifndef __qsynthTuningsForm_h
#define __qsynthTuningsForm_h

#include "ui_qsynthTuningsForm.h"

#include <fluidsynth.h>
#include <QObject>


class qsynthOptions;
class qsynthEngine;

class QPixmap;


class qsynthTuningsForm : public QWidget
{
    Q_OBJECT
public:

    qsynthTuningsForm(QWidget *pParent = 0, Qt::WindowFlags wflags = 0);
    ~qsynthTuningsForm();

    void setup(qsynthOptions *pOptions, qsynthEngine *pEngine, bool bNew/*TODO:Or change to bPreset*/);

signals:

public slots:

    void contextMenuRequested(const QPoint&); //TODO: What is this for?

protected:

    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void closeEvent(QCloseEvent *);

private:

    Ui::qsynthTuningsForm m_ui;

    qsynthOptions *m_pOptions;

    int m_iDirtySetup;
    int m_iDirtyCount;
};

#endif // __qsynthTuningsForm_h
