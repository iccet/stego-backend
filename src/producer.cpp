#include "producer.hpp"

EncodedContainerProducer::EncodedContainerProducer(const kafka::Properties &options)
    : QObject()
    , kafka::KafkaSyncProducer(options)
{ }

bool EncodedContainerProducer::event(QEvent *event)
{
    if (event->type() != QEvent::User)
        return QObject::event(event);

    auto *containerEvent = dynamic_cast<ContainerEvent*>(event);

	auto &data = containerEvent->Data;
	auto &encoder = containerEvent->Encoder;

    auto container = encoder->decode((const uchar*)data.data(),
									 static_cast<int>(data.size()));

    auto record = kafka::ProducerRecord(containerEvent->Topic,
                                        kafka::NullKey, 
										kafka::Value(container.data(), container.size()));

    try
    {
        auto metadata = send(record);
        qInfo(backend, "Message delivered: %s", metadata.toString().data());
    }
    catch (const kafka::KafkaException& e)
    {
        qCritical(producer, "Message delivery failed: %s", e.error().message().c_str());
    }

    return QObject::event(event);
}
