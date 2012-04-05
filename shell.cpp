#include "shell.h"

#include <qdebug.h>

namespace {

QString PortCommand = "/opt/local/bin/port";
QString SuperUserCommand = "/usr/bin/sudo";

}

Shell::Shell(QObject *parent) :
    QObject(parent)
{
    connect(&process_, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));

}

QStringList Shell::runCommand(const QString & command, bool runAsSuperUser, const QStringList & args)
{
    stdout_.clear();

    QStringList arguments;
    QString program = PortCommand;
    if (runAsSuperUser) {
        program = SuperUserCommand;
        arguments << PortCommand;
    }

    arguments << command;
    if (!arguments.isEmpty()) arguments << args;

    process_.setReadChannel(QProcess::StandardOutput);

    process_.start(program, arguments);
    process_.waitForFinished(-1);

    return stdout_.split('\n');
}

void Shell::onReadyReadStandardOutput()
{
    stdout_.append(process_.readAllStandardOutput());
}
