#include "htp.h"
#include "ui_htp.h"

HTP::HTP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HTP)
{
    ui->setupUi(this);
}

HTP::~HTP()
{
    delete ui;
}
