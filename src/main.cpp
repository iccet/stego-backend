#include <rdkafka.h>

#include <kafka/KafkaProducer.h>

#include <iostream>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QDnsLookup>

struct Options
{
    Options() : type(QDnsLookup::A) { }

    QDnsLookup::Type type;
    kafka::Topic topic;
    QStringList brokerHosts;
};

void parseOptions(QCommandLineParser &parser, Options &options)
{
    QCommandLineOption topicOption("t", "topic", QCoreApplication::translate("main", "Kafka topic."));
    QCommandLineOption brokersOption({"b", "brokers"},
                                     QCoreApplication::translate("main", "Broker hosts."),
                                     "0.0.0.0:9092;0.0.0.0:9091");

    parser.addOptions(
    {
        brokersOption,
        topicOption,
    });

    parser.parse(QCoreApplication::arguments());

    if (parser.isSet(brokersOption))
    {
        const auto host = parser.value(brokersOption);

        options.brokerHosts = host.split(';');
        // TODO validate host pattern

        if (options.brokerHosts.empty())
            qFatal("Bad brokers host addresses: %s.", host.data());
    }

    if (options.brokerHosts.empty())
        qFatal("Brokers host addresses does not set.");

    if (parser.isSet(topicOption))
        options.topic = parser.value(topicOption).toStdString();

    if (options.topic.empty())
        qFatal("Kafka topic does not set.");
}

int main(int argc, char *argv[])
{
    QCommandLineParser parser;
    Options options;
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    parser.setApplicationDescription("Steganography backend based on Qt and Kafka.");
    parser.addHelpOption();
    parser.addVersionOption();

    parseOptions(parser, options);

    std::string brokers = options.brokerHosts.join(';').toStdString();
    kafka::Topic topic = options.topic;

    try
    {
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

    return QCoreApplication::exec();
}
