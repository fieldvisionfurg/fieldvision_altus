/*
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
*/

#ifndef PROCESSWIDGET_H
#define PROCESSWIDGET_H

#include <QGroupBox>
#include <QLabel>
#include "VisionCore.h"
#include "ImageViewerCv.h"

//Forward declaration
class ProcessControl;

namespace Ui {
class ProcessWidget;
}

/// Interface gráfica para um objeto ProcessControl. Mostra funcionalidades básicas de controle (fechar, pausar, definir cor de desenho).
/**
 *
 * \ingroup VisionGUI
 */
class ProcessWidget : public QGroupBox
{
    Q_OBJECT

public:
    /// Construtor padrão.
    explicit ProcessWidget(ProcessControl *control,QWidget *parent = 0);

    /// Destrutor padrão.
    ~ProcessWidget();

    /// Adiciona um widget.
    void addParameterWidget(QWidget *wid);

    /// Adiciona um widget com um label associado.
    void addParameterWidget(QLabel *lab, QWidget *wid);

    /// Adiciona um widget.
    void addWidget(QWidget *wid);

    /// Retorna a cor selecionada pelo usuário.
    QColor getDrawColor();

    void setProcTime(double procTime);

    int getDrawSize();

    bool drawResult();

    bool keepLog();

    void setStatus(const QString& status);


signals:
    /// Sinaliza que o botão para encerrar o processo foi clicado.
    void closeButtonClicked();

private slots:
    void on_pauseBtn_clicked();

    void on_startBtn_clicked();

    void on_colorBtn_clicked();

    void on_closeBtn_clicked();

    void on_exportLogBtn_clicked();

    void on_keepLogCheckBtn_toggled(bool checked);

    void on_resetLogBtn_clicked();

    void on_drawCheckBtn_toggled(bool checked);

    void on_syncCheckBox_toggled(bool checked);

private:
    Ui::ProcessWidget *ui;
    ProcessControl *control;
    QColor drawColor;

};

#endif // PROCESSWIDGET_H
