#ifndef STEGO_BACKEND_OPTIONS_HPP
#define STEGO_BACKEND_OPTIONS_HPP

#include "kafka/Types.h"

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QDnsLookup>

struct Options
{
    Options() : type(QDnsLookup::A) { }

    QDnsLookup::Type type;
    kafka::Topic topic;
    QStringList brokerHosts;

    QStringList validate();

};

#endif //STEGO_BACKEND_OPTIONS_HPP
