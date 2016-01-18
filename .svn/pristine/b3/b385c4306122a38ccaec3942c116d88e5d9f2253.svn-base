/*******************************************************************************\
Copyright (c) 2015, FURG - Universidade Federal do Rio Grande
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Universidade Federal do Rio Grande nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL UNIVERSIDADE FEDERAL DO RIO GRANDE BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*******************************************************************************/
#ifndef MODBUSCONFIGWIDGET_H
#define MODBUSCONFIGWIDGET_H

#include <QWidget>
#include "ModbusSensorWrap.h"

namespace Ui {
class ModbusConfigWidget;
}

/// Widget para configuração da conexão Modbus.
/**
 *
 * \ingroup FieldVision
 */
class ModbusConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModbusConfigWidget(QWidget *parent = 0);

    ~ModbusConfigWidget();

    template<class TImg,class TObj>
    void setup(ModbusSensorWrap<TImg,TObj>* w)
    {
        //Conecta o botão de configuração do modbus
        QObject::connect(this,&ModbusConfigWidget::connectBtnClicked,[=](QString ip,int port,int address){
            w->connectModbus(ip.toStdString(),port,address);
        });
    }

signals:
    void connectBtnClicked(QString ip,int port,int address);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ModbusConfigWidget *ui;
};

#endif // MODBUSCONFIGWIDGET_H
