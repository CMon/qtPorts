#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    ui_->mainToolBar->addAction(refreshButton);
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
