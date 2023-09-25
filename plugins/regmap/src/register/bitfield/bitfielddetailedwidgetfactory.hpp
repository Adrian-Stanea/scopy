#ifndef BITFIELDDETAILEDWIDGETFACTORY_HPP
#define BITFIELDDETAILEDWIDGETFACTORY_HPP

#include <QObject>

namespace scopy::regmap {
class BitFieldModel;
namespace gui {
class BitFieldDetailedWidget;

class BitFieldDetailedWidgetFactory : public QObject
{
	Q_OBJECT
public:
	explicit BitFieldDetailedWidgetFactory(QObject *parent = nullptr);

	BitFieldDetailedWidget *buildWidget(BitFieldModel *model);
Q_SIGNALS:
};
} // namespace gui
} // namespace scopy::regmap
#endif // BITFIELDDETAILEDWIDGETFACTORY_HPP
