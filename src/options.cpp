#include "options.hpp"

QStringList Options::validate()
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
