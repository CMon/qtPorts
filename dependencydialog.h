#pragma once

#include <QDialog>

namespace Ui {
class DependencyDialog;
}

class DependencyDialog : public QDialog
{
    Q_OBJECT
    
public:
    DependencyDialog(QWidget *parent = 0);
    ~DependencyDialog();
    
    void setInstallPackage(const QString & package, const QString & version);
    void showDepencencies(const QString & package, const QString & version);

private:
    Ui::DependencyDialog *ui;
};
