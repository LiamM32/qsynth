#include "qsynthAbout.h"
#include "qsynthTuning.h"


// qsynthTuning - Note tuning item class.
//

// Constructor
qsynthTuning::qsynthTuning ( QTreeWidget *pParent )
    : QTreeWidgetItem(pParent)
{
}


qsynthTuning::~qsynthTuning(void)
{
}


bool qsynthTuning::operator<(const QTreeWidgetItem& other) const
{
	int iColumn = QTreeWidgetItem::treeWidget()->sortColumn();
	const QString& s1 = text(iColumn);
	const QString& s2 = other.text(iColumn);
	if (iColumn == QSYNTH_TUNINGS_KEYNUM ||
		iColumn == QSYNTH_TUNINGS_FREQ)	{
		return (s1.toInt() < s2.toInt());
	} else {
		return (s1 < s2);
	}
}
