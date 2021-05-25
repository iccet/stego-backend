#include "filters.hpp"

bool StegoEventFilter::eventFilter(QObject *sender, QEvent *event)
{
    if (event->type() == QEvent::User)
        return true;
    else
        return QObject::eventFilter(sender, event);
}
