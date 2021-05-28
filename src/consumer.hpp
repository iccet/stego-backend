#ifndef STEGO_BACKEND_CONSUMER_HPP
#define STEGO_BACKEND_CONSUMER_HPP

#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include <utility>

#include <rdkafka.h>

#include <kafka/KafkaConsumer.h>

#include "container_encoded_event.hpp"
#include "logger.hpp"
#include "options.hpp"

class ContainerConsumer : public QObject
{
    Q_OBJECT

public:
    explicit ContainerConsumer(const kafka::Properties &options,
                               kafka::Topic topic,
                               QObject *parent);

    int listen();

private:
    kafka::KafkaAutoCommitConsumer _consumer;
    kafka::Topic _topic;
    QThread *_thread;
};

#endif //STEGO_BACKEND_CONSUMER_HPP
