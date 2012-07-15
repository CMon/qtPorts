#pragma once

#include <QMainWindow>
#include <QSet>

class QTreeWidgetItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_twCategories_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem *);
    void showCleaningTasks();

private:
    void initCategories();

private:
    Ui::MainWindow * ui_;

private:
    QSet<QString> categories_;
};
