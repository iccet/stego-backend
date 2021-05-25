#ifndef STEGO_CONTAINER_ENCODED_EVENT_HPP
#define STEGO_CONTAINER_ENCODED_EVENT_HPP

#include <QEvent>

class ContainerEncodedEvent : QEvent
{
    QEvent::Type type() const;
};

#endif //STEGO_CONTAINER_ENCODED_EVENT_HPP
