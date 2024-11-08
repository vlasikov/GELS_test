#include "mainwindow.h"
#include "ui_GUI1.h"
#include "chrome_mb.h"

chrome_mb *chrome;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->setup_widget->setHidden(true);
  ui->param_widget->setHidden(true);

  QObject::connect(ui->ClearButton, SIGNAL(toggled(bool)), ui->setup_widget, SLOT(setVisible(bool)));

  chrome = new chrome_mb;

  QVector<double> x1(2) , y1(2);
  x1[0] = -10;
  y1[0] = -10;
  x1[1] = 10;
  y1[1] = -10;
  QVector<double> x2(2) , y2(2);
  x2[0] = -10;
  y2[0] = 20;
  x2[1] = 10;
  y2[1] = 20;

  // Добавляем график на полотно
  ui->graph_widget->addGraph(ui->graph_widget->xAxis, ui->graph_widget->yAxis);
  ui->graph_widget->addGraph(ui->graph_widget->xAxis, ui->graph_widget->yAxis);
  QPen pen;
  pen.setWidth(1);
  pen.setStyle(Qt::DotLine);
  pen.setColor(QColor(0,0,180));
  //    ui->graph_widget->graph(0)->setName("Confidence Band 68%");
  ui->graph_widget->graph(0)->setPen(pen);
  ui->graph_widget->graph(0)->setBrush(QColor(50,50,255,20));
  ui->graph_widget->graph(1)->setPen(pen);

  ui->graph_widget->graph(0)->setChannelFillGraph(ui->graph_widget->graph(1));
  ui->graph_widget->graph(0)->setData(x1,y1);     // Устанавливаем координаты точек графика
  ui->graph_widget->graph(1)->setData(x2,y2);     // Устанавливаем координаты точек графика

  // Устанавливаем максимальные и минимальные значения координат
  ui->graph_widget->xAxis->setRange(-50,50);
  ui->graph_widget->yAxis->setRange(-50,50);
  ui->graph_widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);// | QCP::iSelectPlottables);

  // Отрисовываем содержимое полотна
  ui->graph_widget->replot();

}

MainWindow::~MainWindow()
{
  chrome->disconnect();
  delete chrome;
  delete ui;
}


void MainWindow::on_StartButton_clicked(bool checked)
{
  if (checked)
    ui->StartButton->setText("Стоп");
  else
    ui->StartButton->setText("Старт");
}

void MainWindow::on_ConnectButton_clicked(bool checked){
  if (checked)  {
    bool res = chrome->connect(ui->ip_addr_le->text().toLocal8Bit().data());
    if (res)
      ui->ConnectButton->setChecked(true);
    else
      ui->ConnectButton->setChecked(false);
  }
  else  {
    chrome->disconnect();
  }
}

/*
 * Чтение настроек с платы
 */
void MainWindow::on_pushButton_2_clicked(){
    QString IP_addr_dev, dose_time1, dose_time2, cycle_time, cycle_pause, path_delay, operating_mode, ADC24_gain1, ADC24_gain2;

    if (chrome->readParam()){
        IP_addr_dev =   QString::number((chrome->RHReg[0])&0xFF)+"." + \
                        QString::number((chrome->RHReg[0]>>8)&0xFF) + "."  + \
                        QString::number((chrome->RHReg[1])&0xFF) + "." + \
                        QString::number((chrome->RHReg[1]>>8)&0xFF);
        ui->dev_IP->setText(IP_addr_dev);

        dose_time1  =   QString::number((chrome->RHReg[2]));
        ui->dose_time1->setText(dose_time1);

        dose_time2  =   QString::number((chrome->RHReg[3]));
        ui->dose_time2->setText(dose_time2);

        cycle_time  =   QString::number((chrome->RHReg[4]));
        ui->cycle_time->setText(cycle_time);

        cycle_pause =   QString::number((chrome->RHReg[5]));
        ui->cycle_pause->setText(cycle_pause);

        path_delay  =   QString::number((chrome->RHReg[6]));
        ui->path_delay->setText(path_delay);

        operating_mode  =   QString::number((chrome->RHReg[7]));
        ui->operating_mode->setText(operating_mode);

        ADC24_gain1 =   QString::number((chrome->RHReg[8]));
//        ui->ADC24_gain1->  setText(ADC24_gain1);
        ui->ADC24_gain1->setCurrentIndex(ui->ADC24_gain1->findText(ADC24_gain1));     // 0x01 - 0x80

        ADC24_gain2 =   QString::number((chrome->RHReg[9]));
        ui->ADC24_gain2->setCurrentIndex(ui->ADC24_gain2->findText("128"));     // 0x01 - 0x80
    }
}

