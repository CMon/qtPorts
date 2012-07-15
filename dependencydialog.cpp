#include "dependencydialog.h"
#include "ui_dependencydialog.h"

DependencyDialog::DependencyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DependencyDialog)
{
    ui->setupUi(this);
}

DependencyDialog::~DependencyDialog()
{
    delete ui;
}

void DependencyDialog::setInstallPackage(const QString &package, const QString &version)
{
    ui->widget->setBasePackage(package, version);
    ui->widget->hideInstallCommand(false);
}

void DependencyDialog::showDepencencies(const QString &package, const QString &version)
{
    ui->widget->setBasePackage(package, version);
    ui->widget->hideInstallCommand(false);
}
