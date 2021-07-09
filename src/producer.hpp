#ifndef STEGO_BACKEND_PRODUCER_HPP
#define STEGO_BACKEND_PRODUCER_HPP

#include <QObject>
#include <QEvent>
#include <QDebug>

#include <iostream>

#include <rdkafka.h>

#include <kafka/KafkaProducer.h>

#include "containerevent.hpp"
#include "logger.hpp"
#include "options.hpp"

#include <stg.hpp>

class EncodedContainerProducer : public QObject, public kafka::KafkaSyncProducer
{
    Q_OBJECT

public:
    explicit EncodedContainerProducer(const kafka::Properties &options);

    bool event(QEvent *event) override;
};

#endif //STEGO_BACKEND_PRODUCER_HPP
