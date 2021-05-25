#ifndef STEGO_BACKEND_CONSUMER_HPP
#define STEGO_BACKEND_CONSUMER_HPP

#include <QObject>
#include <QThread>
#include <utility>

#include <rdkafka.h>

#include <kafka/KafkaConsumer.h>

#include "options.hpp"

class ContainerConsumer : QObject
{
    Q_OBJECT

public:
    explicit ContainerConsumer(const kafka::Properties &options, kafka::Topic topic)
    : QObject()
    , _consumer(options)
    , _topic(std::move(topic)) { }

    int listen();

private:
    kafka::KafkaAutoCommitConsumer _consumer;
    kafka::Topic _topic;
    QThread *_thread;
};

#endif //STEGO_BACKEND_CONSUMER_HPP
