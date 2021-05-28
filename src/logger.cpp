#include "logger.hpp"

Q_LOGGING_CATEGORY(logDebug, "DEBUG")
Q_LOGGING_CATEGORY(logInfo, "INFO")
Q_LOGGING_CATEGORY(logWarning, "WARN")
Q_LOGGING_CATEGORY(logCritical,"CRIT")

Q_LOGGING_CATEGORY(producer, "stego.producer")
Q_LOGGING_CATEGORY(consumer, "stego.consumer")

Q_LOGGING_CATEGORY(message, "stego.message")

QScopedPointer<QFile> logFile;
const QtMessageHandler defaultMessageHandler = qInstallMessageHandler(nullptr);

void setupLogging(const QSettings &settings)
{
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
}

void fileMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    QTextStream out(logFile.data());
    out << qPrintable(qFormatLogMessage(type, context, message)) << Qt::endl;
    out.flush();
}
