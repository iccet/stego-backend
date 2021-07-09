#ifndef STEGO_CONTAINER_ENCODED_EVENT_HPP
#define STEGO_CONTAINER_ENCODED_EVENT_HPP

#include <rdkafka.h>

#include <QEvent>
#include <kafka/ConsumerRecord.h>
#include <kafka/Types.h>

#include <stg.hpp>

class ContainerEvent : public QEvent
{
public:
    explicit ContainerEvent(kafka::Topic  topic, const kafka::Value& data);

    QScopedPointer<Stg::AbstractEncoder> Encoder;
	kafka::Topic Topic;
    kafka::Value Data;
};

#endif //STEGO_CONTAINER_ENCODED_EVENT_HPP
