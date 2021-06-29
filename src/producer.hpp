#ifndef STEGO_BACKEND_PRODUCER_HPP
#define STEGO_BACKEND_PRODUCER_HPP

#include <QObject>
#include <QEvent>
#include <QDebug>

#include <iostream>

#include <rdkafka.h>

#include <kafka/KafkaProducer.h>

#include "logger.hpp"
#include "container_encoded_event.hpp"
#include "options.hpp"

class EncodedContainerProducer : public QObject, public kafka::KafkaSyncProducer
{
    Q_OBJECT

public:
    explicit EncodedContainerProducer(const kafka::Properties &options, kafka::Topic topic);

    bool event(QEvent *event) override;

private:
    kafka::Topic _topic;
};

#endif //STEGO_BACKEND_PRODUCER_HPP
