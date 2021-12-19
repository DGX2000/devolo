#ifndef FILEWINDOW_H
#define FILEWINDOW_H

#include <QFileSystemModel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class FileWindow; }
QT_END_NAMESPACE

class FileWindow : public QMainWindow
{
    Q_OBJECT

public:
    FileWindow(QWidget *parent = nullptr);
    ~FileWindow();

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    Ui::FileWindow *ui;
    QFileSystemModel *fsModel;
};
#endif // FILEWINDOW_H
