#ifndef STEGO_CONTAINER_ENCODED_EVENT_HPP
#define STEGO_CONTAINER_ENCODED_EVENT_HPP

#include <QEvent>
#include <kafka/ConsumerRecord.h>
#include <kafka/Types.h>

class ContainerEncodedEvent : public QEvent
{
public:
    explicit ContainerEncodedEvent(const kafka::Value &data);

    kafka::Value _data;
};

#endif //STEGO_CONTAINER_ENCODED_EVENT_HPP
