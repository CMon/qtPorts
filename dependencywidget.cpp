#include "dependencywidget.h"

#include "ui_dependencywidget.h"

#include <qdebug.h>

DependencyWidget::DependencyWidget(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::DependencyWidget)
{
    ui_->setupUi(this);

}

void DependencyWidget::setBasePackage(const QString & packageName, const QString & version)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    dependancies_.clear();
    installedDeps_.clear();
    basePackage_ = packageName;

    getDependencies(packageName);
    installedDeps_ = macPort_.areInstalled(dependancies_.keys());

    updateStatusLine();
    ui_->leInstallCommand->setText(QString("sudo port install %1 %2").arg(packageName).arg(version));
    updateTree();

    QApplication::restoreOverrideCursor();
}

void DependencyWidget::hideInstallCommand(bool hide)
{
    if (hide) {
        ui_->lblInstallCommand->hide();
        ui_->leInstallCommand->hide();
    } else {
        ui_->lblInstallCommand->show();
        ui_->leInstallCommand->show();
    }
}

void DependencyWidget::getDependencies(const QString & packageName)
{
     QSet<QString> deps = macPort_.getDeps(packageName);

     dependancies_[packageName].unite(deps);

     foreach(const QString & package, deps) {
         if (dependancies_.contains(package)) continue;

         getDependencies(package);
     }
}

void DependencyWidget::updateTree()
{
    ui_->twPackages->clear();

    QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << basePackage_);
    if (installedDeps_.value(basePackage_, false)) item->setBackground(0, Qt::green);
    ui_->twPackages->addTopLevelItem(item);

    addChildren(item, dependancies_[basePackage_]);
}

void DependencyWidget::updateStatusLine()
{
    int installed = 0;
    foreach(bool inst, installedDeps_.values()) {
        if (inst) ++installed;
    }
    int needInstall = installedDeps_.count() - installed;
    ui_->lblStatusLine->setText(QString("%1 unique dependencies, %2 packages installed, %3 packages need to be installed")
                                .arg(dependancies_.count() - 1) // minus the base package
                                .arg(installed)
                                .arg(needInstall)
                                );
}

void DependencyWidget::addChildren(QTreeWidgetItem *parent, const QSet<QString> & deps)
{
    foreach(const QString & dep, deps) {
        QTreeWidgetItem * child = new QTreeWidgetItem(QStringList() << dep);
        if (installedDeps_.value(dep, false)) child->setBackground(0, Qt::green);
        parent->addChild(child);

        if (!dependancies_[dep].isEmpty()) addChildren(child, dependancies_[dep]);
    }
}
