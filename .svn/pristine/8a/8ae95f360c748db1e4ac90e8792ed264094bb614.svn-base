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

#include "ProcessWidget.h"
#include "ProcessControl.h"
#include "QColorDialog"
#include "ui_ProcessWidget.h"

#include <QFileDialog>


ProcessWidget::ProcessWidget(ProcessControl* control_,QWidget *parent) :
    QGroupBox(parent),
    control(control_),
    drawColor(QColor("Red")),
    ui(new Ui::ProcessWidget)
{
    ui->setupUi(this);
    ui->closeBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    ui->pauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->startBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->closeBtn->setText("");
    ui->pauseBtn->setText("");
    ui->startBtn->setText("");
    ui->colorBtn->setText("");

    QImage img(16,16,QImage::Format_RGB32);
    img.fill(Qt::GlobalColor(Qt::red));
    ui->colorBtn->setIcon(QIcon(QPixmap::fromImage(img)));
}

ProcessWidget::~ProcessWidget()
{
    delete ui;
}

void ProcessWidget::addParameterWidget(QWidget *wid)
{
    ui->methodParametersGroup->layout()->addWidget(wid);
}

void ProcessWidget::addParameterWidget(QLabel *lab, QWidget *wid)
{
    QWidget* w = new QWidget();
    QHBoxLayout* lay = new QHBoxLayout(w);
    lay->addWidget(lab);
    lay->addWidget(wid);
    ui->methodParametersGroup->layout()->addWidget(w);
}

void ProcessWidget::addWidget(QWidget *wid){
    ui->verticalLayout->addWidget(wid);
}


QColor ProcessWidget::getDrawColor()
{
    return drawColor;
}

void ProcessWidget::setProcTime(double procTime)
{
    ui->procTimeLabel->setText(QString::number(procTime)+ " ms");
    ui->maxProcessingFpsLabel->setText(QString::number(1000/procTime));
}

int ProcessWidget::getDrawSize()
{
    return ui->drawHistSize->value();
}

bool ProcessWidget::keepLog()
{
    return ui->keepLogCheckBtn->isChecked();
}

void ProcessWidget::setStatus(const QString &status)
{
    ui->statusLabel->setText(status);
}

bool ProcessWidget::drawResult()
{
    return ui->drawCheckBtn->isChecked();
}

void ProcessWidget::on_pauseBtn_clicked()
{
    control->pause();
}

void ProcessWidget::on_startBtn_clicked()
{
    control->start();
}

void ProcessWidget::on_colorBtn_clicked()
{
    drawColor = QColorDialog::getColor(drawColor,this,tr("Select drawing color."));
    QImage img(16,16,QImage::Format_RGB32);
    img.fill(drawColor);
    ui->colorBtn->setIcon(QIcon(QPixmap::fromImage(img)));
}

void ProcessWidget::on_closeBtn_clicked()
{
    this->hide();
    emit closeButtonClicked();
}

void ProcessWidget::on_exportLogBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Choose log file name."));
    control->exportLog(fileName.toStdString());
}

void ProcessWidget::on_keepLogCheckBtn_toggled(bool checked)
{
    ui->exportLogBtn->setEnabled(checked);
    ui->extortCsvRadioBtn->setEnabled(checked);
    ui->resetLogBtn->setEnabled(checked);
}

void ProcessWidget::on_resetLogBtn_clicked()
{
    control->resetLog();
}

void ProcessWidget::on_drawCheckBtn_toggled(bool checked)
{
    ui->drawHistSize->setEnabled(checked);
}

void ProcessWidget::on_syncCheckBox_toggled(bool checked)
{
    control->sync(checked);
}
