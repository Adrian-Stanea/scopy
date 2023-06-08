#include "griiofloatchannelsrc.h"
#include "grlog.h"
#include "gnuradio/blocks/int_to_float.h"
#include "gnuradio/blocks/short_to_float.h"
#include "gnuradio/blocks/copy.h"

using namespace scopy::grutil;
GRIIOFloatChannelSrc::GRIIOFloatChannelSrc(GRIIODeviceSource *dev, QString channelName, QObject *parent) :
	GRIIOChannel(channelName, dev, parent)
{
}

void GRIIOFloatChannelSrc::build_blks(GRTopBlock *top)
{
	qDebug(SCOPY_GR_UTIL)<<"Building GRIIOFloatChannelSrc";
	dev->addChannel(this);

	auto fmt = dev->getChannelFormat(channelName);
	switch(fmt->length) {
	case 16:
		x2f = gr::blocks::short_to_float::make();
		break;
	case 32:
		x2f = gr::blocks::int_to_float::make();
		break;
	default:
		x2f = gr::blocks::copy::make(sizeof(float));
		break;
	}

	end_blk = x2f;
	start_blk.append(x2f);
}

void GRIIOFloatChannelSrc::destroy_blks(GRTopBlock *top)
{
	dev->removeChannel(this);
	x2f = nullptr;
	end_blk = nullptr;
	start_blk.clear();
}
