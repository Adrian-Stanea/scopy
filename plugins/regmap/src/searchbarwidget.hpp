#ifndef SEARCHBARWIDGET_HPP
#define SEARCHBARWIDGET_HPP

#include "scopy-regmapplugin_export.h"

#include <QWidget>

class QHBoxLayout;
class QPushButton;
class QLineEdit;
namespace scopy::regmap {
namespace gui {
class SCOPY_REGMAPPLUGIN_EXPORT SearchBarWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SearchBarWidget(QWidget *parent = nullptr);
	~SearchBarWidget();

Q_SIGNALS:
	void requestSearch(QString searchParam);

private:
	QLineEdit *searchBar;
	QPushButton *searchButton;
	QHBoxLayout *layout;
};
} // namespace gui
} // namespace scopy::regmap
#endif // SEARCHBARWIDGET_HPP
