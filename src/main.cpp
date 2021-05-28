#include <iostream>

#include "consumer.hpp"
#include "producer.hpp"
#include "logger.hpp"
#include "options.hpp"

#include <QCoreApplication>
#include <QSettings>
#include <QCommandLineParser>

void parseOptions(QCommandLineParser &parser, Options &options)
{
    const QRegExp separator("(,|;)");
    auto helpOption = parser.addHelpOption();
    auto versionOption = parser.addVersionOption();
    QCommandLineOption topicOption({"t", "topic"},
                                   QCoreApplication::translate("main", "Kafka topic."),
                                   "topic-string");

    QCommandLineOption brokersOption({"b", "brokers"},
                                     QCoreApplication::translate("main", "Broker hosts."),
                                     "brokers-hosts"
                                     "0.0.0.0:9092,0.0.0.0:9091");

    parser.addOptions(
    {
        brokersOption,
        topicOption,
    });

    if (!parser.parse(QCoreApplication::arguments()))
        qFatal("%s", qPrintable(parser.errorText()));

    if (parser.isSet(helpOption))
        parser.showHelp();

    if (parser.isSet(versionOption))
        parser.showVersion();

    if (parser.isSet(brokersOption))
        options.brokerHosts = parser.value(brokersOption).split(separator);

    if (parser.isSet(topicOption))
        options.topic = parser.value(topicOption).toStdString();

    auto errors = options.validate();

    if(!errors.empty())
        qFatal("Invalid program arguments:\n %s",
               qPrintable(errors.join('\n')));
}

int main(int argc, char *argv[])
{
    QCommandLineParser parser;
    Options options;
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setOrganizationName("IcCet");
    QCoreApplication::setOrganizationDomain("iccet.org");
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    QSettings settings;

    settings.beginGroup("logging");

    settings.beginGroup("file");
    settings.setValue("name", "log.txt");
    settings.setValue("enabled", true);
    settings.endGroup();

    settings.beginGroup("elastic");
    settings.setValue("enabled", false);
    settings.endGroup();

    settings.beginGroup("default");
    settings.setValue("enabled", true);
    settings.endGroup();

    settings.endGroup();

    parser.setApplicationDescription("Steganography backend based on Qt and Kafka.");

    setupLogging(settings);

    parseOptions(parser, options);

    std::string brokers = options.brokerHosts.join(';').toStdString();
    kafka::Topic topic = options.topic;

    kafka::Properties props(
    {
        {"bootstrap.servers",  brokers},
        {"enable.idempotence", "true"},
    });

    auto producer = new EncodedContainerProducer(props, topic);
    auto consumer = new ContainerConsumer(props, topic, producer);

    consumer->listen();
    return QCoreApplication::exec();
}
