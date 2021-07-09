#ifndef STEGO_BACKEND_LOGGER_HPP
#define STEGO_BACKEND_LOGGER_HPP

#include <rdkafka.h>
#include <rdkafkacpp.h>

#include <elasticlient/client.h>
#include <elasticlient/logging.h>
#include <cpr/response.h>

#include <httpstatuscodes/HttpStatusCodes_Qt.h>
#include <kafka/KafkaClient.h>

#include <QLoggingCategory>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMutex>
#include <QSettings>
#include <QFile>
#include <QUuid>
#include <QMetaEnum>

extern QScopedPointer<QFile> logFile;

Q_DECLARE_LOGGING_CATEGORY(rdkafka)
Q_DECLARE_LOGGING_CATEGORY(backend)

Q_DECLARE_LOGGING_CATEGORY(producer)
Q_DECLARE_LOGGING_CATEGORY(consumer)

Q_DECLARE_LOGGING_CATEGORY(decoder)
Q_DECLARE_LOGGING_CATEGORY(encoder)

QString msgTypeToString(QtMsgType type);

void setupLogging(const QSettings &settings);

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

extern const QtMessageHandler defaultMessageHandler;

void fileMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);
void elasticMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

void logCallback(elasticlient::LogLevel logLevel, const std::string &msg);

void defaultLogger(int level, const char* filename, int lineno, const char* msg);

#endif //STEGO_BACKEND_LOGGER_HPP
