#pragma once

#include <macport.h>

#include <QMap>
#include <QSet>
#include <QWidget>

class QTreeWidgetItem;

namespace Ui {
class DependencyWidget;
}

class DependencyWidget : public QWidget
{
    Q_OBJECT
public:
    DependencyWidget(QWidget *parent = 0);
    
    void setBasePackage(const QString & packageName, const QString & version);

    void hideInstallCommand(bool hide);

private:
    void getDependencies(const QString & packageName);
    void updateTree();
    void updateStatusLine();
    void addChildren(QTreeWidgetItem * parent, const QSet<QString> &deps);

private:
    Ui::DependencyWidget * ui_;

private:
    MacPort macPort_;

    QString                       basePackage_;
    QString                       baseVersion_;
    QMap<QString, QSet<QString> > dependancies_;
    QHash<QString, bool>          installedDeps_;
};
