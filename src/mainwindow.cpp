#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    results(new Results(this))
{
    ui->setupUi(this);
    results->setWindowModality(Qt::ApplicationModal);
    isPathsReady();
    on_computeButton_clicked();
}

MainWindow::~MainWindow()
{
    delete results;
    delete ui;
}

QFileInfo MainWindow::GetKinoFile() const
{
    return ui->exportedPathEdit->text();
}

QFileInfo MainWindow::GetConfigFile() const
{
    return ui->exportedConfigEdit->text();
}

void MainWindow::on_exportedPathButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Fichier exporté  de Kinovea"), "",
            tr("Fichier XML (*.xml);;All Files (*)"));
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
            tr("Configuration (*.xml);;All Files (*)"));
    ui->exportedConfigEdit->setText(fileName);
}
void MainWindow::on_exportedConfigEdit_textChanged(const QString &arg1)
{
    ui->exportedConfigEdit->setText(arg1);
    isPathsReady();
}

void MainWindow::isPathsReady()
{
    QFileInfo kinoFile(GetKinoFile());
    QFileInfo configFile(GetConfigFile());
    if (kinoFile.exists() && kinoFile.isFile() && configFile.exists() && configFile.isFile())
        ui->computeButton->setEnabled(true);
    else
        ui->computeButton->setEnabled(false);
}


void MainWindow::on_computeButton_clicked()
{
    QString configFilePath(GetConfigFile().absoluteFilePath());
    ProportionalModel w(configFilePath);
    results->show();
}
