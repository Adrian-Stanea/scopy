#ifndef BitFieldDetailedWidget_HPP
#define BitFieldDetailedWidget_HPP

#include "bitfieldoption.hpp"

#include <QFrame>
#include <QWidget>

class QLabel;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QVBoxLayout;

namespace scopy::regmap {
namespace gui {
class BitFieldDetailedWidget : public QFrame
{
	Q_OBJECT
public:
	explicit BitFieldDetailedWidget(QString name, QString access, int defaultValue, QString description, int width,
					QString notes, int regOffset, QVector<BitFieldOption *> *options,
					QWidget *parent = nullptr);
	~BitFieldDetailedWidget();

	QString getToolTip() const;
	void updateValue(QString newValue);
	QString getValue();

	int getWidth() const;

	int getRegOffset() const;

private:
	QVBoxLayout *layout;
	QString toolTip;
	int width;
	QString description;
	bool reserved;
	int regOffset;
	QString access;

	QVector<BitFieldOption *> *options;
	QLabel *value;
	QComboBox *valueComboBox = nullptr;
	QCheckBox *valueCheckBox = nullptr;
	QLineEdit *valueLineEdit = nullptr;

	void firstRead();

Q_SIGNALS:
	void valueUpdated(QString value);
};
} // namespace gui
} // namespace scopy::regmap
#endif // BitFieldDetailedWidget_HPP
