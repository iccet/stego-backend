#include "producer.hpp"

bool EncodedContainerProducer::event(QEvent *event)
{
    if (event->type() == QEvent::User)
    {
        auto *containerEncodedEvent = dynamic_cast<ContainerEncodedEvent *>(event);
        return true;
    }

    qCDebug(logDebug()) << "test";

    try
    {
        std::cout << "% Type message value and hit enter to produce message. (empty line to quit)" << std::endl;

        for (std::string line; std::getline(std::cin, line);)
        {
            auto record = kafka::ProducerRecord(_topic,
                                                kafka::NullKey,
                                                kafka::Value(line.c_str(), line.size()));

            try
            {
                kafka::Producer::RecordMetadata metadata = _producer.send(record);
                std::cout << "% Message delivered: " << metadata.toString() << std::endl;
            }
            catch (const kafka::KafkaException& e)
            {
                std::cerr << "% Message delivery failed: " << e.error().message() << std::endl;
            }

            if (line.empty()) break;
        }

    }
    catch (const kafka::KafkaException& e)
    {
        std::cerr << "% Unexpected exception caught: " << e.what() << std::endl;
    }

    return QObject::event(event);
}
