#ifndef REGISTERMAPINSTRUMENT_HPP
#define REGISTERMAPINSTRUMENT_HPP

#include "scopy-regmapplugin_export.h"

#include <iio.h>

#include <QMap>
#include <QWidget>

#include <tool_view.hpp>

class QComboBox;
class QVBoxLayout;
class QTabWidget;

namespace scopy::regmap {
class RegisterMapValues;
class DeviceRegisterMap;

namespace gui {
class RegisterMapSettingsMenu;
class SearchBarWidget;
} // namespace gui

class SCOPY_REGMAPPLUGIN_EXPORT RegisterMapInstrument : public QWidget
{
	Q_OBJECT
public:
	explicit RegisterMapInstrument(QWidget *parent = nullptr);
	~RegisterMapInstrument();
	void addTab(struct iio_device *dev, QString title);
	void addTab(struct iio_device *dev, QString title, QString xmlPath);
	void addTab(QString filePath, QString title);
signals:

private:
	QVBoxLayout *layout;
	QWidget *mainWidget;
	scopy::gui::ToolView *toolView;

	RegisterMapValues *getRegisterMapValues(struct iio_device *dev);
	RegisterMapValues *getRegisterMapValues(QString filePath);

	QMap<QString, DeviceRegisterMap *> *tabs;
	bool first = false;
	QString activeRegisterMap;
	QComboBox *registerDeviceList;
	void toggleSettingsMenu(QString registerName, bool toggle);
	scopy::regmap::gui::RegisterMapSettingsMenu *settings;
	gui::SearchBarWidget *searchBarWidget;

private Q_SLOTS:
	void updateActiveRegisterMap(QString registerName);
	void toggleSearchBarVisible(bool visible);
};
} // namespace scopy::regmap
#endif // REGISTERMAPINSTRUMENT_HPP
