#ifndef SCOPYHOMEINFOPAGE_H
#define SCOPYHOMEINFOPAGE_H

#include "scopy-core_export.h"

#include <QWidget>

namespace Ui {
class ScopyHomeInfoPage;
}

class SCOPY_CORE_EXPORT ScopyHomeInfoPage : public QWidget
{
	Q_OBJECT

public:
	explicit ScopyHomeInfoPage(QWidget *parent = nullptr);
	~ScopyHomeInfoPage();

private:
	Ui::ScopyHomeInfoPage *ui;
};

#endif // SCOPYHOMEINFOPAGE_H
