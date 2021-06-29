#ifndef STEGO_BACKEND_LOGGER_HPP
#define STEGO_BACKEND_LOGGER_HPP

#include <rdkafka.h>
#include <rdkafkacpp.h>

#include <kafka/KafkaClient.h>

#include <QLoggingCategory>
#include <QMutex>
#include <QSettings>
#include <QFile>

extern QScopedPointer<QFile> logFile;

Q_DECLARE_LOGGING_CATEGORY(logDebug)
Q_DECLARE_LOGGING_CATEGORY(logInfo)
Q_DECLARE_LOGGING_CATEGORY(logWarning)
Q_DECLARE_LOGGING_CATEGORY(logCritical)

Q_DECLARE_LOGGING_CATEGORY(rdkafka)

Q_DECLARE_LOGGING_CATEGORY(producer)
Q_DECLARE_LOGGING_CATEGORY(consumer)

Q_DECLARE_LOGGING_CATEGORY(message)

void setupLogging(const QSettings &settings);

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

extern const QtMessageHandler defaultMessageHandler;

void fileMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);
void elasticMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

void defaultLogger(int level, const char* filename, int lineno, const char* msg);

#endif //STEGO_BACKEND_LOGGER_HPP
