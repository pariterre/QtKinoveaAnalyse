#include "results.h"
#include "ui_results.h"

Results::Results(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Results)
{
    ui->setupUi(this);
}

Results::~Results()
{
    delete ui;
}

const MainWindow & Results::GetMainWindow()
{
    return *dynamic_cast<MainWindow*>(this->parent());
}

void Results::on_NextWidget_clicked()
{
    ui->stackedWidget->setCurrentIndex((ui->stackedWidget->currentIndex() + 1) % ui->stackedWidget->count());
    QFileInfo tata(GetMainWindow().GetConfigFile());
    QFileInfo tata2(GetMainWindow().GetConfigFile());
}

