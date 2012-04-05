#pragma once

#include <shell.h>

#include <QObject>
#include <QMap>
#include <QPair>

typedef QPair<QString, QString> Dict;

class PackageEntry
{
public:
    QString name;
    QString version;
    QString category;
    QString packageIdentifier;
};

class InstalledPackageEntry
{
public:
    QString name;
    QString version;
    bool    isActive;
};

class PackageDetail
{
public:
    QString header;
    QList<Dict> values;
};

class MacPort : public QObject
{
    Q_OBJECT
public:
    MacPort(QObject *parent = 0);

    QList<PackageEntry> getPackageList();
    QList<InstalledPackageEntry> getInstalledPackageList();
    
    PackageDetail getPackageDetail(const QString & name);

private:
    Shell               shell;
    QMap<QString, bool> commands_;
};
