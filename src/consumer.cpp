#include "consumer.hpp"

int ContainerConsumer::listen()
{
    try
    {
        _consumer.subscribe({_topic});

        qInfo(consumer(), "Reading messages from topic: %s", _topic.data());

        forever
        {
            auto records = _consumer.poll(std::chrono::milliseconds(100));
            for (const auto& record: records)
            {
                if (record.value().size() == 0) return 0;

                if (!record.error())
                {
                    std::cout << "% Got a new message..." << std::endl;
                    std::cout << "    Topic    : " << record.topic() << std::endl;
                    std::cout << "    Partition: " << record.partition() << std::endl;
                    std::cout << "    Offset   : " << record.offset() << std::endl;
                    std::cout << "    Timestamp: " << record.timestamp().toString() << std::endl;
                    std::cout << "    Headers  : " << kafka::toString(record.headers()) << std::endl;
                    std::cout << "    Key   [" << record.key().toString() << "]" << std::endl;
                    std::cout << "    Value [" << record.value().toString() << "]" << std::endl;
                }
                else
                {
                    qCritical(consumer()) << record.toString().data();
                }
            }
        }

    }
    catch (const kafka::KafkaException& e)
    {
        qCritical(consumer(), "Unexpected exception caught: %s", e.what());
    }
}
