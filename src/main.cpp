#include <iostream>

#include "consumer.hpp"
#include "logger.hpp"
#include "options.hpp"

#include <QCoreApplication>
#include <QCommandLineParser>

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

    auto consumer = new ContainerConsumer(props, topic);

    consumer->listen();
    return QCoreApplication::exec();
}
