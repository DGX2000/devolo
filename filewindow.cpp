#include "filewindow.h"
#include "./ui_filewindow.h"

#include <QDebug>
#include <QFileSystemModel>

FileWindow::FileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileWindow)
{
    ui->setupUi(this);

    QFileSystemModel *fsModel = new QFileSystemModel(this);
    fsModel->setRootPath(QDir::homePath());

    ui->treeView->setModel(fsModel);
    ui->treeView->setRootIndex(fsModel->index(QDir::homePath()));

    // hide file type and access date
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    // resize to fit the name sizes
    ui->treeView->setColumnWidth(0, 3*this->size().width()/4);
    ui->treeView->sortByColumn(0, Qt::SortOrder::AscendingOrder);
}

FileWindow::~FileWindow()
{
    delete ui;
}


void FileWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    auto data = index.data();

    qDebug("breakpoint");
}

