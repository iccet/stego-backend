#include "container_encoded_event.hpp"

QEvent::Type ContainerEncodedEvent::type() const
{
    return QEvent::User;
}
