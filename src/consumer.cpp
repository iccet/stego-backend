#include "consumer.hpp"

ContainerConsumer::ContainerConsumer(const kafka::Properties &options,
                                     kafka::Topic topic,
                                     QObject *parent)
    : QObject(parent)
    , _consumer(options)
    , _topic(std::move(topic))
    , _thread() { }

int ContainerConsumer::listen()
{
    try
    {
        _consumer.subscribe({_topic});

        qInfo(consumer, "Reading messages from topic: %s", _topic.data());

        forever
        {
            auto records = _consumer.poll(std::chrono::milliseconds(100));
            for (const auto& record: records)
            {
                if (record.value().size() == 0) return 0;

                if (!record.error())
                {
                    qInfo(message, "%s", record.toString().data());
                    auto event = new ContainerEncodedEvent();
                    QCoreApplication::sendEvent(parent(), event);
                }
                else
                {
                    qCritical(consumer) << record.toString().data();
                }
            }
        }

    }
    catch (const kafka::KafkaException& e)
    {
        qCritical(consumer, "Unexpected exception caught: %s", e.what());
    }
    Q_UNREACHABLE();
}
