#ifndef HOVERWIDGET_H
#define HOVERWIDGET_H

#include "scopy-gui_export.h"
#include "utils.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QWidget>

namespace scopy {
enum HoverPosition
{
	HP_LEFT,
	HP_TOPLEFT,
	HP_TOP,
	HP_TOPRIGHT,
	HP_RIGHT,
	HP_BOTTOMRIGHT,
	HP_BOTTOM,
	HP_BOTTOMLEFT,
	HP_CENTER,
};

class SCOPY_GUI_EXPORT HoverWidget : public QWidget
{
	Q_OBJECT
	QWIDGET_PAINT_EVENT_HELPER
public:
	HoverWidget(QWidget *content = nullptr, QWidget *anchor = nullptr, QWidget *parent = nullptr);
	~HoverWidget();

	bool eventFilter(QObject *watched, QEvent *event) override;
	QPoint getAnchorOffset();
	void setAnchorOffset(QPoint);
	HoverPosition getAnchorPos();
	void setAnchorPos(HoverPosition);
	HoverPosition getContentPos();
	void setContentPos(HoverPosition);

public:
	void moveToAnchor();
	void setContent(QWidget *widget);
	void setAnchor(QWidget *anchor);
	void setParent(QWidget *parent);

private:
	QHBoxLayout *m_lay;
	QWidget *m_parent;
	QWidget *m_anchor;
	QWidget *m_content;
	QWidget *m_container;
	QPoint m_anchorOffset;
	HoverPosition m_anchorPos;
	HoverPosition m_contentPos;

	// QWidget interface
protected:
	void showEvent(QShowEvent *event) override;
};
} // namespace scopy
#endif // HOVERWIDGET_H
