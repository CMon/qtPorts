#pragma once

#include <QDialog>

namespace Ui {
class CleanupDialog;
}

class CleanupDialog : public QDialog
{
    Q_OBJECT

public:
    CleanupDialog(QWidget *parent = 0);
    ~CleanupDialog();
    void setPackageRemoves(const QStringList & commands);

private:
    Ui::CleanupDialog *ui;
};

