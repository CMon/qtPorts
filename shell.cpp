#include "shell.h"

#include <QProcess>

namespace {

QString PortCommand = "/opt/local/bin/port";
QString SuperUserCommand = "/usr/bin/sudo";

}

Shell::Shell(QObject *parent) :
    QObject(parent)
{
}

QStringList Shell::runCommand(const QString & command, bool runAsSuperUser, const QStringList & args)
{
    QStringList arguments;
    QString program = PortCommand;
    if (runAsSuperUser) {
        program = SuperUserCommand;
        arguments << PortCommand;
    }

    arguments << command;
    if (!arguments.isEmpty()) arguments << args;

    QProcess process;
    process.setReadChannel(QProcess::StandardOutput);

    process.start(program, arguments);
    process.waitForFinished();

    return QString(process.readAllStandardOutput()).split('\n');
}
