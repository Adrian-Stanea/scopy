#include "registerdetailedwidget.hpp"

#include "../verticalscrollarea.hpp"
#include "bitfield/bitfielddetailedwidget.hpp"
#include "bitfield/bitfielddetailedwidgetfactory.hpp"
#include "qdebug.h"
#include "registermodel.hpp"
#include "utils.hpp"

#include <QtMath>
#include <qboxlayout.h>
#include <qlabel.h>

using namespace scopy;
using namespace regmap;
using namespace regmap::gui;

RegisterDetailedWidget::RegisterDetailedWidget(RegisterModel *regModel, QWidget *parent)
	: QWidget{parent}
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);

	QGridLayout *bitFieldsWidgetLayout = new QGridLayout();
	bitFieldsWidgetLayout->setMargin(0);
	bitFieldsWidgetLayout->setSpacing(0);

	bitFieldList = new QVector<BitFieldDetailedWidget *>();
	BitFieldDetailedWidgetFactory bitFieldDetailedWidgetFactory;
	QWidget *bitFieldsWidget = new QWidget();
	bitFieldsWidget->setLayout(bitFieldsWidgetLayout);
	VerticalScrollArea *scrollArea = new VerticalScrollArea();
	scrollArea->setWidget(bitFieldsWidget);
	layout->addWidget(scrollArea);
	int currentBitFieldCount = 0;
	int row = 0;
	int col = 0;
	for(int i = regModel->getBitFields()->size() - 1; i >= 0; --i) {
		BitFieldDetailedWidget *bitFieldDetailedWidget =
			bitFieldDetailedWidgetFactory.buildWidget(regModel->getBitFields()->at(i));
		bitFieldList->push_back(bitFieldDetailedWidget);

		bitFieldsWidgetLayout->addWidget(bitFieldDetailedWidget, row, col);

		bitFieldsWidgetLayout->setColumnStretch(col, 1);
		col++;

		currentBitFieldCount += bitFieldDetailedWidget->getWidth();

		if(col > scopy::regmap::Utils::getBitsPerRow()) {
			row++;
			col = 0;
		}
		QObject::connect(bitFieldDetailedWidget, &BitFieldDetailedWidget::valueUpdated, this,
				 [=]() { Q_EMIT bitFieldValueChanged(getBitFieldsValue()); });
	}
}

void RegisterDetailedWidget::updateBitFieldsValue(uint32_t value)
{
	int regOffset = 0;
	for(int i = bitFieldList->length() - 1; i >= 0; --i) {
		bitFieldList->at(i)->blockSignals(true);

		int width = bitFieldList->at(i)->getWidth();
		int bfVal = (((1 << (regOffset + width)) - 1) & value) >> regOffset;
		QString bitFieldValue = QString::number(bfVal, 16);
		bitFieldList->at(i)->updateValue(bitFieldValue);
		regOffset += width;

		bitFieldList->at(i)->blockSignals(false);
	}
}

QString RegisterDetailedWidget::getBitFieldsValue()
{
	int result = 0;
	for(int i = 0; i < bitFieldList->length(); ++i) {
		bool ok;
		int aux = bitFieldList->at(i)->getValue().toInt(&ok, 16);
		int regOffset = bitFieldList->at(i)->getRegOffset();
		result += qPow(2, regOffset) * aux;
		qDebug() << "aux = " << aux << " result = " << result;
	}
	return QString::number(result, 16);
	;
}
