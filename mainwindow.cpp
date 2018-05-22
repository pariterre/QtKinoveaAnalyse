#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exportedPathButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Fichier exporté  de Kinovea"), "",
            tr("Fichier excel (*.xls);;All Files (*)"));
    ui->exportedPathEdit->setText(fileName);
}
void MainWindow::on_exportedPathEdit_textChanged(const QString &arg1)
{
    ui->exportedPathEdit->setText(arg1);
    isPathsReady();
}


void MainWindow::on_exportedConfigButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Fichier de configuration"), "",
            tr("Configuration (*.kc);;All Files (*)"));
    ui->exportedConfigEdit->setText(fileName);
}
void MainWindow::on_exportedConfigEdit_textChanged(const QString &arg1)
{
    ui->exportedConfigEdit->setText(arg1);
    isPathsReady();
}

void MainWindow::isPathsReady()
{
    QFileInfo kinoFile(ui->exportedPathEdit->text());
    QFileInfo configFile(ui->exportedConfigEdit->text());
    if (kinoFile.exists() && kinoFile.isFile() && configFile.exists() && configFile.isFile())
        ui->computeButton->setEnabled(true);
    else
        ui->computeButton->setEnabled(false);
}


void MainWindow::on_computeButton_clicked()
{

}
