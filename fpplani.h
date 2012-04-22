#ifndef FPPLANI_H
#define FPPLANI_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class FPPlani;
}

class FPPlani : public QWidget
{
    Q_OBJECT
    
public:
    explicit FPPlani(QWidget *parent = 0);
    ~FPPlani();
    void paintEvent(QPaintEvent *);
private slots:
    void setBev(int value);
    void setSlots(int value);
    void on_horizontalSliderBev_valueChanged(int value);

    void on_horizontalSliderSlots_valueChanged(int value);


    void on_checkBoxMyriOrbit_toggled(bool checked);

    void on_checkBoxMyriFP_toggled(bool checked);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::FPPlani *ui;
    void calc();

};

#endif // FPPLANI_H
