#include "logger.hpp"

Q_LOGGING_CATEGORY(logDebug, "DEBUG")
Q_LOGGING_CATEGORY(logInfo, "INFO")
Q_LOGGING_CATEGORY(logWarning, "WARN")
Q_LOGGING_CATEGORY(logCritical,"CRIT")

Q_LOGGING_CATEGORY(rdkafka, "rdkafka")
Q_LOGGING_CATEGORY(producer, "stego.producer")
Q_LOGGING_CATEGORY(consumer, "stego.consumer")

Q_LOGGING_CATEGORY(message, "stego.message")

QScopedPointer<QFile> logFile;
const QtMessageHandler defaultMessageHandler = qInstallMessageHandler(nullptr);

void setupLogging(const QSettings &settings)
{
    kafka::KafkaClient::setGlobalLogger(defaultLogger);
    elasticlient::setLogFunction(logCallback);
    logFile.reset(new QFile(settings.value("logging/file/name").toString()));

    if (Q_UNLIKELY(!logFile.data()->open(QFile::Append | QIODevice::Text | QIODevice::WriteOnly)))
        qWarning(logWarning, "Log file %s not found", qPrintable(logFile.data()->fileName()));

    qInstallMessageHandler(messageHandler);
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    static const QSettings settings;

    if(Q_LIKELY(settings.value("logging/default/enabled").value<bool>()))
        defaultMessageHandler(type, context, message);

    if(settings.value("logging/file/enabled").value<bool>() && Q_LIKELY(logFile.data()->isOpen()))
        fileMessageHandler(type, context, message);

    if(settings.value("logging/elastic/enabled").value<bool>())
        elasticMessageHandler(type, context, message);
}

void fileMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    QTextStream out(logFile.data());
    out << qPrintable(qFormatLogMessage(type, context, message)) << endl;
    out.flush();
}

void elasticMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    elasticlient::Client client({"http://localhost:9200/"});

    QJsonObject body { { "message", qPrintable(qFormatLogMessage(type, context, message)) } };
    QJsonDocument doc(body);

    cpr::Response response = client.index("stego-backend",
                                          "docType",
                                          "docId",
                                          doc.toJson(QJsonDocument::Compact).data());

    switch (response.status_code)
    {
        case HttpStatus::Code::OK:
        case HttpStatus::Code::Created: break;
        default: Q_ASSERT(false);
    }
}

void logCallback(elasticlient::LogLevel logLevel, const std::string &msg)
{
    Q_UNUSED(logLevel);
    Q_UNUSED(msg);
    return;
    using elasticlient::LogLevel;
    switch (logLevel)
    {
        case LogLevel::FATAL:
        case LogLevel::ERROR:
        case LogLevel::WARNING:
        case LogLevel::INFO:
        case LogLevel::DEBUG:
        default: Q_UNREACHABLE();
    }
}

void defaultLogger(int level, const char* /*filename*/, int /*lineno*/, const char* msg)
{
    switch (level)
    {
        case LOG_EMERG: qCCritical(rdkafka, "%s", msg); break;
        case LOG_CRIT: qCritical(rdkafka, "%s", msg); break;

        case LOG_ALERT:
        case LOG_ERR: qCWarning(rdkafka, "%s", msg); break;
        case LOG_WARNING: qWarning(rdkafka, "%s", msg); break;

        case LOG_NOTICE: qCInfo(rdkafka, "%s", msg); break;
        case LOG_INFO: qInfo(rdkafka, "%s", msg); break;

        case LOG_DEBUG: qDebug(rdkafka, "%s", msg); break;
        default: Q_UNREACHABLE();
    }
}
