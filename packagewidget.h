#pragma once

#include <dependencydialog.h>
#include <macport.h>

#include <QWidget>
#include <QSet>

class QAction;
class PackageDetail;
class PackageEntry;
class QTreeWidgetItem;

namespace Ui {
class PackageWidget;
}

class PackageWidget : public QWidget
{
    Q_OBJECT
public:
    PackageWidget(QWidget *parent = 0);
    ~PackageWidget();
    
    void changeCategory(const QString & category);
    QSet<QString> getAllCategories() { return categories_; }
    QStringList getListOfObsoletePackages();

public slots:
    void on_twPackages_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem *);
    void on_rbOnlyInstalled_toggled(bool checked);
    void on_rbMultipleInstalls_toggled(bool checked);
    void on_leFilter_textChanged(const QString & text);
    void refreshPackages();
    void showListOfObsoletePackages();

private:
    void initActions();
    void updatePackageTreeStatusBar(int count);

    PackageDetail getPackageDetail(const QString & name);

    void updateFilter();

    static QString createUninstallCommand(const QString & package, const QString & version);

private slots:
    void install();
    void uninstall();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    Ui::PackageWidget * ui_;
    DependencyDialog    dependencyDialog_;

private:
    MacPort                                   macPort_;

    QMultiMap<QString, PackageEntry>          packageEntries_;
    QMultiMap<QString, InstalledPackageEntry> installedPackages_;
    QSet<QString>                             categories_;
    QMap<QString, PackageDetail>              cachedDetails_;

    bool    showMultiples_;
    bool    showInstalled_;
    QString filterName_;
    QString filterCategory_;

    QAction * installAction_;
    QAction * uninstallAction_;
};
