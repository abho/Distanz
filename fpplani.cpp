#include "fpplani.h"
#include "ui_fpplani.h"
#include <QDebug>

FPPlani::FPPlani(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPPlani)
{
    ui->setupUi(this);

    setBev(ui->horizontalSliderBev->value());
    setSlots(ui->horizontalSliderSlots->value());
    calc();
}

FPPlani::~FPPlani()
{
    delete ui;
}

void FPPlani::paintEvent(QPaintEvent *)
{

         QStyleOption opt;
         opt.init(this);
         QPainter p(this);
         style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}



void FPPlani::calc()
{
    double slot = ui->horizontalSliderSlots->value();
    double bev =ui->horizontalSliderBev->value();
    double bevTmp;
    double fpBoden = ui->checkBoxMyriFP->isChecked()?27:25;
    double fpOrbit = ui->checkBoxMyriOrbit->isChecked()?28:25;
    double fp= 0;
    double result = 0;
    double steuer = ui->comboBox->currentText()=="normal"?1.0:1.1;
    for (int var = 0; var < 13; ++var) {

        bevTmp = bev+(12-var);
        int tmpFP  =  (slot*fpBoden*bevTmp+(var*fpOrbit*bevTmp)+((12-var)*6*bevTmp))*steuer;
        qDebug() << var << tmpFP;

        if(tmpFP > fp){
            result = var;
            fp = tmpFP;
            ui->labelNew->setText(QString::number(tmpFP));

        }
        if(var ==0)
            ui->labelOld->setText(QString::number(tmpFP));
    }


    ui->labelFP->setText(QString::number(result));
}

void FPPlani::setBev(int value)
{
    ui->labelBev->setText(QString::number(value));
}

void FPPlani::setSlots(int value)
{
    ui->labelSlots->setText(QString::number(value));
}



void FPPlani::on_horizontalSliderBev_valueChanged(int value)
{
    setBev(value);
    calc();
}

void FPPlani::on_horizontalSliderSlots_valueChanged(int value)
{
    setSlots(value);
    calc();
}



void FPPlani::on_checkBoxMyriOrbit_toggled(bool checked)
{
    calc();
}

void FPPlani::on_checkBoxMyriFP_toggled(bool checked)
{
    calc();
}

void FPPlani::on_comboBox_currentIndexChanged(int index)
{
    calc();
}
