#pragma once
#include <QObject>

#include <QStringList>
#include <QProcess>

class Shell : public QObject
{
    Q_OBJECT
public:
    Shell(QObject *parent = 0);
    
    QStringList runCommand(const QString & command, bool runAsSuperUser, const QStringList & args = QStringList());

private slots:
    void onReadyReadStandardOutput();

private:
    QProcess process_;
    QString  stdout_;
};

