#include "devicefactory.h"

#include "iiodeviceimpl.h"

using namespace scopy;

DeviceImpl *DeviceFactory::build(QString param, PluginManager *pm, QString category, QObject *parent)
{
	if(category.compare("iio") == 0) {
		return new IIODeviceImpl(param, pm, parent);
	} else {
		return new DeviceImpl(param, pm, category, parent);
	}
}
