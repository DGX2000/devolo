#include "filewindow.h"
#include "./ui_filewindow.h"

#include "dc6dialog.h"

FileWindow::FileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileWindow)
{
    ui->setupUi(this);

    fsModel = new QFileSystemModel(this);
    fsModel->setRootPath(QDir::homePath());

    ui->treeView->setModel(fsModel);
    ui->treeView->setRootIndex(fsModel->index(QDir::homePath()));

    // hide file size, file type and access date
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    // resize to fit the name sizes
    ui->treeView->setColumnWidth(0, this->size().width());
    ui->treeView->sortByColumn(0, Qt::SortOrder::AscendingOrder);
}

FileWindow::~FileWindow()
{
    delete ui;
}


void FileWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    auto pathString = fsModel->filePath(index);

    if(pathString.toLower().endsWith("dc6"))
    {
        DC6Dialog dialog(pathString);
        dialog.exec();
    }
}

