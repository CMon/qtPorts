#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cleanupdialog.h>

namespace {
QString AllCategory = "All";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    ui_->twCategories->sortByColumn(0, Qt::AscendingOrder);

    initCategories();

    QAction * refreshButton = new QAction(QIcon(":refresh.png"), "Update package list", this);
    connect(refreshButton, SIGNAL(triggered()), ui_->wPackageWidget, SLOT(refreshPackages()));

    QAction * showRemovePackagesListButton = new QAction(QIcon(":list.png"), "Show list of duplicate packages to remove", this);
    connect(showRemovePackagesListButton, SIGNAL(triggered()), ui_->wPackageWidget, SLOT(showListOfObsoletePackages()));

    QAction * showCleaningTasksButton = new QAction(QIcon(":clean.png"), "Show commands to clean your ports", this);
    connect(showCleaningTasksButton, SIGNAL(triggered()), this, SLOT(showCleaningTasks()));

    ui_->mainToolBar->addAction(refreshButton);
    ui_->mainToolBar->addAction(showRemovePackagesListButton);
    ui_->mainToolBar->addAction(showCleaningTasksButton);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::on_twCategories_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem */*previous*/)
{
    if (!current) return;

    const QString category = current->text(0);
    if (category == AllCategory) ui_->wPackageWidget->changeCategory("");
    else                         ui_->wPackageWidget->changeCategory(current->text(0));
}

void MainWindow::showCleaningTasks()
{
    CleanupDialog cd;
    cd.setPackageRemoves(ui_->wPackageWidget->getListOfObsoletePackages());
    cd.exec();
}

void MainWindow::initCategories()
{
    if (categories_.isEmpty()) categories_ = ui_->wPackageWidget->getAllCategories();

    ui_->twCategories->clear();

    foreach (const QString & cat, categories_) {
        QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << cat);
        ui_->twCategories->addTopLevelItem(item);
    }

    QTreeWidgetItem * allItem = new QTreeWidgetItem(QStringList() << AllCategory);
    ui_->twCategories->insertTopLevelItem(0, allItem);
    ui_->twCategories->setCurrentItem(allItem);
}
