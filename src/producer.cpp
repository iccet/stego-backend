#include "producer.hpp"

EncodedContainerProducer::EncodedContainerProducer(const kafka::Properties &options,
                                                   kafka::Topic topic)
    : QObject()
    , kafka::KafkaSyncProducer(options)
    , _topic(std::move(topic))
{ }

bool EncodedContainerProducer::event(QEvent *event)
{
    if (event->type() != QEvent::User)
        return QObject::event(event);

    auto *containerEncodedEvent = dynamic_cast<ContainerEncodedEvent *>(event);

    auto record = kafka::ProducerRecord(_topic,
                                        kafka::NullKey,
                                        containerEncodedEvent->_data);

    try
    {
        auto metadata = send(record);
        qInfo(message, "Message delivered: %s", metadata.toString().data());
    }
    catch (const kafka::KafkaException& e)
    {
        qCritical(producer, "Message delivery failed: %s", e.error().message().c_str());
    }

}
