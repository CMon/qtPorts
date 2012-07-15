#include "macport.h"

#include <shell.h>

#include <QStringList>
#include <QRegExp>

#include <qdebug.h>

namespace {


QMap<QString, bool> initCommands () {

    QStringList availableCommands;
    availableCommands << "activate" << "archive"   << "archivefetch" << "build"      << "cat"        << "cd"        << "checksum"
                      << "clean"    << "configure" << "contents"     << "deactivate" << "dependents" << "deps"      << "destroot"
                      << "dir"      << "distcheck" << "distfiles"    << "dmg"        << "dpkg"       << "echo"      << "ed"
                      << "edit"     << "exit"      << "extract"      << "fetch"      << "file"       << "gohome"    << "help"
                      << "info"     << "install"   << "installed"    << "lint"       << "list"       << "livecheck" << "load"
                      << "location" << "log"       << "logfile"      << "mdmg"       << "mirror"     << "mpkg"      << "notes"
                      << "outdated" << "patch"     << "pkg"          << "platform"   << "portpkg"    << "provides"  << "quit"
                      << "rdependents" << "rdeps"  << "rpm"          << "search"     << "select"     << "selfupdate" << "setrequested"
                      << "space"    << "srpm"      << "submit"       << "sync"       << "test"       << "unarchive" << "uninstall"
                      << "unload"   << "unsetrequested" << "upgrade" << "url"        << "usage"      << "variants"  << "version"
                      << "work";

    QMap<QString, bool> retval;
    foreach (const QString & command, availableCommands) {
        retval[command] = false;
    }

    // now set those who need su permissions
    retval["install"] = true;
    retval["uninstall"] = true;

    return retval;
}

}

MacPort::MacPort(QObject *parent) :
    QObject(parent),
    commands_(initCommands())
{
}

QList<PackageEntry> MacPort::getPackageList()
{
    const QStringList shellOutput = shell.runCommand("list", commands_["list"]);

    QList<PackageEntry> retval;
    foreach (const QString & line, shellOutput) {
        if (line.isEmpty()) continue;

        QStringList elements = line.split(QRegExp("\\s+"));
        if (elements.size() != 3) {
            qWarning() << "list elements (line: " << line << ") have a wrong size: " << elements.size();
            continue;
        }

        PackageEntry entry;
        entry.name     = elements[0];
        entry.version  = elements[1];
        entry.category = elements[2].split("/")[0];
        entry.packageIdentifier = elements[2];

        retval << entry;
    }

    return retval;
}

QList<InstalledPackageEntry> MacPort::getInstalledPackageList()
{
    QStringList shellOutput = shell.runCommand("installed", commands_["list"]);

    // first line is: The follwoing ports are currently ... -> drop line
    shellOutput.takeFirst();

    QList<InstalledPackageEntry> retval;
    foreach (const QString & line, shellOutput) {
        if (line.isEmpty()) continue;

        QStringList elements = line.trimmed().split(QRegExp("\\s+"));
        if (elements.size() > 3 || elements.size() <= 0) {
            qWarning() << "list elements (line: " << line << ") have a wrong size: " << elements.size();
            continue;
        }

        InstalledPackageEntry entry;
        entry.name     = elements[0];
        entry.version  = elements[1];
        if (elements.size() == 3) entry.isActive = true;
        else                      entry.isActive = false;

        retval << entry;
    }

    return retval;
}

PackageDetail MacPort::getPackageDetail(const QString &name)
{
    QStringList shellOutput = shell.runCommand("info", commands_["info"], QStringList() << name);

    PackageDetail detail;
    detail.header = shellOutput.takeFirst();
    foreach(const QString & line, shellOutput) {
        if (line.trimmed().isEmpty()) continue;

        QStringList parts = line.split(QRegExp(":\\s+"));
        if (parts.size() < 2) {
            qWarning() << "wrong detail part found (line: " << line << ")";
            continue;
        }

        detail.values.append(qMakePair(parts.takeFirst(), parts.join(": ")));
    }

    return detail;
}

bool MacPort::isInstalled(const QString & packageName, const QString & version)
{
    QStringList shellOutput = shell.runCommand("installed", commands_["installed"], QStringList() << packageName << version);

    const QString firstLine = shellOutput.takeFirst();
    if (firstLine.startsWith("None of the specified ports are installed.")) return false;

    return true;
}

QHash<QString, bool> MacPort::areInstalled(const QStringList & packages)
{
    QStringList shellOutput = shell.runCommand("installed", commands_["installed"], packages);
    shellOutput.takeFirst(); // remove the header

    QHash<QString, bool> installedPackages;
    foreach(const QString & package, packages) installedPackages[package] = false;

    foreach(const QString & line, shellOutput) {
        const QString package = line.trimmed().split(QRegExp("\\s")).first();
        if (package.isEmpty()) continue;

        if (!installedPackages.contains(package)) {
            qWarning() << "Found package we did not ask for?? package: " << package << "; search for: " << packages;
            continue;
        }
        installedPackages[package] = true;
    }

    return installedPackages;
}

QSet<QString> MacPort::getDeps(const QString & packageName)
{
    QStringList shellOutput = shell.runCommand("deps", commands_["deps"], QStringList() << packageName);

    QSet<QString> dependencies;
    foreach (const QString & line, shellOutput) {
        if (line.startsWith("Library Dependencies") ||
                line.startsWith("Extract Dependencies") ||
                line.startsWith("Build Dependencies"))
        {
            const QStringList parts = line.split(QRegExp(":\\s+"));
            if (parts.size() < 2) {
                qWarning() << "wrong dependancy part found (line: " << line << ")";
                continue;
            }

            foreach (const QString & dep, parts.at(1).split(',')) {
                dependencies << dep.trimmed();
            }
        }
    }

    return dependencies;
}

