#ifndef STEGO_BACKEND_FILTERS_HPP
#define STEGO_BACKEND_FILTERS_HPP

#include <QObject>
#include <QEvent>

class StegoEventFilter : public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *sender, QEvent *event) override;
};

#endif //STEGO_BACKEND_FILTERS_HPP
