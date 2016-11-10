#ifndef MEASURE_SETTINGS_H
#define MEASURE_SETTINGS_H

#include <QWidget>
#include <memory>

namespace Ui {
	class MeasureSettings;
}

class QStandardItem;
class QStandardItemModel;

namespace adiscope {

class CapturePlot;
class DropdownSwitchList;
class MeasurementData;

class MeasurementItem
{
public:
	MeasurementItem(int id = 0, int chn_id = 0):
	m_id(id),
	m_chn_id(chn_id)
	{}

	int id() { return m_id; }
	int channel_id() { return m_chn_id; }

	bool operator ==(const MeasurementItem& other) const
	{
		bool isEqual = (this->m_id == other.m_id) &&
			(this->m_chn_id == other.m_chn_id);

		return isEqual;
	}
	bool operator !=(const MeasurementItem& other) const
	{
		return !(*this == other);
	}

private:
	int m_id;
	int m_chn_id;
};

struct StatisticSelection {
	DropdownSwitchList *dropdown;
	int posInDropdown;
	MeasurementItem measurementItem;
};

class MeasureSettings : public QWidget
{
	Q_OBJECT
public:
	explicit MeasureSettings(CapturePlot *plot, QWidget *parent = 0);

	QString channelName() const;
	void setChannelName(const QString& name);

	QColor channelUnderlineColor() const;
	void setChannelUnderlineColor(const QColor& color);

	void setHorizMeasurementActive(int idx, bool en);
	void setVertMeasurementActive(int idx, bool en);

	bool emitActivated() const;
	void setEmitActivated(bool en);

	bool emitStatsChanged()const;
	void setEmitStatsChanged(bool en);

	QList<MeasurementItem> measurementSelection();
	QList<MeasurementItem> statisticSelection();

Q_SIGNALS:
	void measurementActivated(int id, int chIdx);
	void measurementDeactivated(int id, int chIdx);
	void measurementSelectionListChanged();

	void statisticActivated(int id, int chIdx);
	void statisticDeactivated(int id, int chIdx);
	void statisticSelectionListChanged();
	void statisticsEnabled(bool en);
	void statisticsReset();

public Q_SLOTS:
	void onChannelAdded(int);
	void onChannelRemoved(int);
	void setSelectedChannel(int);

private Q_SLOTS:
	void onMeasurementPropertyChanged(QStandardItem *item);
	void on_button_measDisplayAll_toggled(bool checked);
	void on_button_measDeleteAll_toggled(bool checked);
	void onMeasurementActivated(int id, bool en);
	void onStatisticActivated(DropdownSwitchList *dropdown, int pos, int id,
		 bool en);
	void on_button_StatisticsEn_toggled(bool checked);
	void on_button_StatisticsReset_pressed();
	void on_button_statsDeleteAll_toggled(bool checked);

private:
	void addHorizontalMeasurement(const QString& name, int measurement_id);
	void addVerticalMeasurement(const QString& name, int measurement_id);
	void buildDropdownElements(int chnIdx);
	void loadMeasurementStatesFromData();
	void deleteAllMeasurements();
	void recoverAllMeasurements();
	void displayAllMeasurements();
	void disableDisplayAllMeasurements();
	void loadStatisticStatesForChannel(int chnIdx);
	void setColumnData(QStandardItemModel *model, int column, bool en);
	void setAllMeasurements(int col, bool en);
	void deleteMeasurementsOfChannel(QList<MeasurementItem>& list,
		int chnIdx);
	void deleteStatisticsOfChannel(QList<struct StatisticSelection>&
		list, int chnIdx);
	void deleteAllStatistics();
	void recoverAllStatistics();

private:
	Ui::MeasureSettings *m_ui;
	QString m_channelName;
	QColor m_chnUnderlineColor;
	DropdownSwitchList *m_horizMeasurements;
	DropdownSwitchList *m_vertMeasurements;
	bool m_emitActivated;
	bool m_emitStatsChanged;
	bool m_emitDeleteAll;
	bool m_emitStatsDeleteAll;
	bool m_are_dropdowns_filled;

	CapturePlot* m_plot;
	int m_selectedChannel;
	QList<MeasurementItem> m_selectedMeasurements;
	QList<MeasurementItem> m_deleteAllBackup;
	QList<MeasurementItem> m_displayAllBackup;

	QList<struct StatisticSelection> m_selectedStatistics;
	QList<struct StatisticSelection> m_statsDeleteAllBackup;
};
} // namespace adiscope

#endif // MEASURE_SETTINGS_H
