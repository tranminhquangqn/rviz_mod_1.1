#ifndef WIDGETITEM_H
#define WIDGETITEM_H

#include <QMainWindow>
#include "rviz/window_manager_interface.h"
#include "rviz/rviz_export.h"
namespace rviz
{
class RVIZ_EXPORT WidgetItem : public QMainWindow//, public WindowManagerInterface
{
    Q_OBJECT
public:
    WidgetItem(QWidget *parent = nullptr);
public Q_SLOTS:

private:

};
}

#endif // WIDGETITEM_H
