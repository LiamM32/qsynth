#ifndef __qsynthtuning_h
#define __qsynthtuning_h

#include <QTreeWidget>

// Column index helpers.
#define QSYNTH_TUNINGS_IN       0
#define QSYNTH_TUNINGS_NOTENUM  1
#define QSYNTH_TUNINGS_PITCHCL  2
#define QSYNTH_TUNINGS_NOTENAME 3
#define QSYNTH_TUNINGS_DETUNE   4
#define QSYNTH_TUNINGS_FREQ     5


class qsynthTuning : public QTreeWidgetItem
{
public:

    qsynthTuning(QTreeWidget *pParent);

    ~qsynthTuning();
};

typedef qsynthTuning * qsynthTuningPtr;

#endif // QSYNTHTUNING_H
