#include <kafka/KafkaProducer.h>
#include <iostream>

int main(int argc, char *argv[])
{
    std::string brokers = argv[1];
    kafka::Topic topic{"topic"};

    try {
        kafka::Properties props(
        {
            {"bootstrap.servers",  brokers},
            {"enable.idempotence", "true"},
        });

        kafka::KafkaSyncProducer producer(props);

        std::cout << "% Type message value and hit enter to produce message. (empty line to quit)" << std::endl;

        for (std::string line; std::getline(std::cin, line);)
        {
            auto record = kafka::ProducerRecord(topic,
                                                kafka::NullKey,
                                                kafka::Value(line.c_str(), line.size()));

            try
            {
                kafka::Producer::RecordMetadata metadata = producer.send(record);
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
}
