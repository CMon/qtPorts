#pragma once
#include <QObject>

#include <QStringList>

class Shell : public QObject
{
    Q_OBJECT
public:
    Shell(QObject *parent = 0);
    
    static QStringList runCommand(const QString & command, bool runAsSuperUser, const QStringList & args = QStringList());
};

