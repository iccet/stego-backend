#include "container_encoded_event.hpp"

ContainerEncodedEvent::ContainerEncodedEvent(const kafka::Value &data)
    : QEvent(QEvent::User)
    , _data(data)
{

}
