#include <rdkafka.h>

#include <kafka/KafkaProducer.h>

#include <iostream>

#include "logger.hpp"

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

    QStringList validate()
    {
        QStringList errors;

        if (brokerHosts.empty())
            errors.append("Brokers address addresses does not set.");

        foreach(auto address, brokerHosts)
        {
            bool isValidPort;
            auto split = address.split(':');
            split.last().toInt(&isValidPort);

            if (QHostAddress(split.first()).isNull())
                errors.append(QString("Bad brokers host addresses: %0.").arg(split.first()));

            if (!isValidPort)
                errors.append(QString("Bad brokers port addresses: %0.").arg(split.last()));
        }

        if (topic.empty())
            errors.append("Kafka topic does not set.");

        return errors;
    }
};

void parseOptions(QCommandLineParser &parser, Options &options)
{
    const QRegExp separator("(,|;)");
    QCommandLineOption topicOption("t", "topic", QCoreApplication::translate("main", "Kafka topic."));
    QCommandLineOption brokersOption({"b", "brokers"},
                                     QCoreApplication::translate("main", "Broker hosts."),
                                     "0.0.0.0:9092,0.0.0.0:9091");

    parser.addOptions(
    {
        brokersOption,
        topicOption,
    });

    parser.parse(QCoreApplication::arguments());

    if (parser.isSet(brokersOption))
        options.brokerHosts = parser.value(brokersOption).split(separator);

    if (parser.isSet(topicOption))
        options.topic = parser.value(topicOption).toStdString();

    auto errors = options.validate();

    if(!errors.empty())
        qFatal("Invalid program arguments:\n %s",
               errors.join('\n').toLocal8Bit().data());
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

    kafka::Properties props(
    {
        {"bootstrap.servers",  brokers},
        {"enable.idempotence", "true"},
    });

    qCDebug(logDebug()) << "test";

    try
    {
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
