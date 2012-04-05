#include "packagewidget.h"
#include "ui_packagewidget.h"

#include <macport.h>
#include <util.h>

#include <QMenu>
#include <QAction>
#include <QEvent>
#include <QContextMenuEvent>

#include <qdebug.h>

Q_DECLARE_METATYPE(PackageEntry)

namespace {
enum PackagesTreeColumns {
    ColPackageName = 0,
    ColPackageVersion,
    ColPackageCategory,
    ColPackageInstalled
};

enum DataRole {
    PackageEntryRole = Qt::UserRole + 1
};

}

PackageWidget::PackageWidget(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::PackageWidget),
    showMultiples_(false),
    showInstalled_(false)
{
    ui_->setupUi(this);

    ui_->twPackages->sortByColumn(0, Qt::AscendingOrder);

    refreshPackages();

    initActions();
}

PackageWidget::~PackageWidget()
{
    delete ui_;
    delete installAction_;
    delete uninstallAction_;
}

void PackageWidget::changeCategory(const QString &category)
{
    filterCategory_ = category;
    updateFilter();
}

void PackageWidget::on_twPackages_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem */*previous*/)
{
    if (!current) return;

    const QString tableHeader = "<table><tr><th colspan=\"2\" align=\"left\">%1</th></tr>";
    const QString tableRow    = "<tr><td>%1: </td><td>%2</td></tr>";
    const QString tableEnd    = "<table>";

    const PackageDetail details = getPackageDetail(current->text(0));

    QString body = tableHeader.arg(details.header);
    foreach (const Dict & elem, details.values) {
        body += tableRow.arg(elem.first).arg(elem.second);
    }

    body += tableEnd;

    ui_->tbDetails->setText(body);
}

void PackageWidget::on_rbOnlyInstalled_toggled(bool checked)
{
    showInstalled_ = checked;
    updateFilter();
}

void PackageWidget::on_rbMultipleInstalls_toggled(bool checked)
{
    showMultiples_ = checked;
    updateFilter();
}

void PackageWidget::on_leFilter_textChanged(const QString &text)
{
    filterName_ = text;
    updateFilter();
}

void PackageWidget::refreshPackages()
{
    installedPackages_.clear();
    packageEntries_.clear();
    categories_.clear();
    ui_->twPackages->clear();

    foreach (const InstalledPackageEntry & entry, macPort_.getInstalledPackageList()) {
        installedPackages_.insert(entry.name, entry);
    }

    foreach (const PackageEntry & entry, macPort_.getPackageList()) {
        packageEntries_.insert(entry.category, entry);
        categories_.insert(entry.category);

        // insert into treewidget
        const bool isInstalled = installedPackages_.contains(entry.name);

        const QString isInstalledString = isInstalled ? QString("%1 (%2)").arg(Util::boolToString(isInstalled)).arg(installedPackages_.values(entry.name).size()) : "";
        QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << entry.name << entry.version << entry.category << isInstalledString);
        item->setData(0, PackageEntryRole, QVariant::fromValue(entry));
        ui_->twPackages->addTopLevelItem(item);

        if (isInstalled) {
            foreach(const InstalledPackageEntry & iEntry, installedPackages_.values(entry.name)) {
                QTreeWidgetItem * iItem = new QTreeWidgetItem(QStringList() << iEntry.name << iEntry.version);
                PackageEntry ipEntry = entry;
                ipEntry.version = iEntry.version;
                iItem->setData(0, PackageEntryRole, QVariant::fromValue(ipEntry));

                if (iEntry.isActive) {
                    iItem->setBackground(ColPackageVersion, Qt::green);
                }
                item->addChild(iItem);
            }
        }
    }

    updatePackageTreeStatusBar(ui_->twPackages->topLevelItemCount());
}

void PackageWidget::initActions()
{
    installAction_ = new QAction("Install", this);
    connect(installAction_, SIGNAL(triggered()), this, SLOT(install()));

    uninstallAction_ = new QAction("Uninstall", this);
    connect(uninstallAction_, SIGNAL(triggered()), this, SLOT(uninstall()));
}

void PackageWidget::updatePackageTreeStatusBar(int count)
{
    ui_->lblPackageListStatusBar->setText(QString("%1 packages").arg(count));
}

PackageDetail PackageWidget::getPackageDetail(const QString &name)
{
    if (!cachedDetails_.contains(name)) {
        cachedDetails_[name] = macPort_.getPackageDetail(name);
    }

    return cachedDetails_[name];
}

void PackageWidget::updateFilter()
{
    int count = 0;
    for (int i = 0; i < ui_->twPackages->topLevelItemCount(); ++i) {
        QTreeWidgetItem * item = ui_->twPackages->topLevelItem(i);
        const PackageEntry entry = item->data(0, PackageEntryRole).value<PackageEntry>();

        if (showMultiples_ && item->childCount() <= 1) {
            // only show those who have more then 1 childs
            item->setHidden(true);
            continue;
        }

        if (showInstalled_ && item->data(ColPackageInstalled, Qt::DisplayRole).toString().isEmpty()) {
            item->setHidden(true);
            continue;
        }

        if (!filterName_.isEmpty() && !entry.name.contains(QRegExp(filterName_))) {
            item->setHidden(true);
            continue;
        }

        if (!filterCategory_.isEmpty() && entry.category != filterCategory_) {
            item->setHidden(true);
            continue;
        }

        ++count;
        item->setHidden(false);
    }

    updatePackageTreeStatusBar(count);
}

void PackageWidget::install()
{
    QTreeWidgetItem * item = ui_->twPackages->currentItem();
    const PackageEntry entry = item->data(0, PackageEntryRole).value<PackageEntry>();

    qDebug () << "Install: " << entry.name << entry.version;
}

void PackageWidget::uninstall()
{
    QTreeWidgetItem * item = ui_->twPackages->currentItem();
    const PackageEntry entry = item->data(0, PackageEntryRole).value<PackageEntry>();

    qDebug () << "Uninstall: " << entry.name << entry.version;
}

void PackageWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

#warning check if already installed most recent version if so gray out install action
    menu.addAction(installAction_);

#warning check if installed and if not disable the uninstall action
//    if (showInstalled_ && item->data(ColPackageInstalled, Qt::DisplayRole).toString().isEmpty()) {

    menu.addAction(uninstallAction_);
    menu.exec(event->globalPos());}
