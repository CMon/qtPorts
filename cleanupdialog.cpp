#include "cleanupdialog.h"
#include "ui_cleanupdialog.h"

#include <QTextDocument>

CleanupDialog::CleanupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CleanupDialog)
{
    ui->setupUi(this);
    setModal(true);

}

CleanupDialog::~CleanupDialog()
{
    delete ui;
}

void CleanupDialog::setPackageRemoves(const QStringList &commands)
{
    QString text;
    text += "<table>"
            "<tr><td colspan=\"2\">Clean all temporary files with:</td></tr>"
            "<tr><td width=\"10px\"></td><td>" "sudo port clean --all" "</td></tr>";
    if (!commands.isEmpty()) {
        text +=
            "<tr></tr>"
            "<tr><td colspan=\"2\">Copy and paste the following commands to remove duplicate packages:</td></tr>";
        foreach (const QString & command, commands) {
            text += "<tr><td width=\"10px\"></td><td>" + command + "</td></tr>";
        }
    }
    text += "</table>";

    ui->tbTasks->setHtml(text);
}
