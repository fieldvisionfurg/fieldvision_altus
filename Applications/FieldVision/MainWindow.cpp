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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <VisionImplementationCv.h>

#include "FrameServerControlWidget.h"
#include "DetectorControl.h"
#include "TrackerControl.h"
#include "FeatureTrackerControl.h"
#include "AnalogMeterControl.h"
#include "FireTrackerControl.h"
#include "PresenceTrackerControl.h"
#include "LevelTrackerControl.h"
#include "DiffractionLevelTrackerControl.h"
#include "AnalogMeterControlNF.h"
#include <QLabel>
#include "qlayout.h"
#include "qpainter.h"
#include <QSlider>
#include <functional>
#include <QScrollBar>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QColorDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include "FrameServerVimba.h"
#include "GeneralSettings.h"
#include "AboutDialog.h"

using namespace Viscv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    generalSettings(this),
    aboutDialog(this),
    fsCtrls()
{
    ui->setupUi(this);
    ui->toolBox->removeItem(0);

    this->videoCount = 0;

    //Actions
    connect(ui->actionWebCAM, SIGNAL(triggered()), this, SLOT(addVideoOpenCv()));
    connect(ui->actionFirewire_Camera, SIGNAL(triggered()), this, SLOT(addFireWireCamera()));
    connect(ui->actionVideo_Local, SIGNAL(triggered()), this, SLOT(addLocal()));
    connect(ui->actionVideo_Url, SIGNAL(triggered()), this, SLOT(addUrl()));
    connect(ui->actionAnalogMeterDetector, SIGNAL(triggered()), this, SLOT(addAnalogMeterDetector()));
    connect(ui->actionFeatureTracker, SIGNAL(triggered()), this, SLOT(addFeatureTrackerKLT()));
    connect(ui->actionPresenceTracker, SIGNAL(triggered()), this, SLOT(addPresenceTracker()));
    connect(ui->actionLevelTracker, SIGNAL(triggered()), this, SLOT(addLevelTracker()));
    connect(ui->actionFireTrackerCT, SIGNAL(triggered()), this, SLOT(addFireTracker()));
    connect(ui->actionDiffractionLevelTracker,SIGNAL(triggered()), this, SLOT(addDiffractionLevelTracker()) );
    connect(ui->actionGeneral_Settings, SIGNAL(triggered()), &generalSettings, SLOT(exec()));
    connect(ui->actionAbout_FieldVision, SIGNAL(triggered()), &aboutDialog, SLOT(exec()));
    connect(ui->actionAnalog_Meter_Non_Frontal, SIGNAL(triggered()), this, SLOT(addAnalogMeterNonFrontal()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

FrameServerControlWidget *MainWindow::chooseFrameServer()
{
    if(fsCtrls.empty()){
        QMessageBox::warning(this,"No video source available.","You have to add some video source before adding any processing method.");
        return NULL;
    }
    if(fsCtrls.size()==1)
        return fsCtrls.front();

    // Mostra lista de frame servers disponíveis
    QStringList vl;
    for(FrameServerControlWidget* fs : fsCtrls)
        vl.push_back(fs->getName());
    bool ok;
    QString choice = QInputDialog::getItem(this,"Choose video source","Video source name",vl,0,false,&ok);

    // Retorna ponteiro para frame server escolhido
    FrameServerControlWidget* returnPtr=NULL;
    if (ok && !choice.isEmpty()){
        for(FrameServerControlWidget* fs : fsCtrls){
            if(fs->getName()==choice){
                returnPtr=fs;
                break;
            }
        }
    }
    return returnPtr;
}
void MainWindow::addUrl(){
    bool ok;
    QString url = QInputDialog::getText(this,"Specify camera URL","Camera URL",QLineEdit::Normal , "http://IPADDRESS/video.mjpg",&ok);
    if(ok)
        this->addVideoOpenCv(url.toStdString());
}
void MainWindow::addLocal(){
    QString path = QFileDialog::getOpenFileName(this, tr("Video path"),
                                                "..\\..\\Arquivos" );
    this->addVideoOpenCv(path.toStdString());
}

void MainWindow::addWidgetInSourceArea(QWidget *widgetPtr)
{
    this->ui->sourceArea->layout()->addWidget(widgetPtr);
}

void MainWindow::addWidgetInProcessArea(QWidget *widgetPtr,const QString name)
{

    //this->ui->processLayout->addWidget(widgetPtr,Qt::AlignTop);
    int index = this->ui->toolBox->addItem(widgetPtr,name);
    this->ui->toolBox->setCurrentIndex(index);

}

void MainWindow::addWidgetInVideoArea(QWidget *widgetPtr)
{
    int numOfChild=this->ui->videoArea->children().size()-1;
    int col = numOfChild % 2;
    if(col==0)
        this->ui->videoLayout0->addWidget(widgetPtr);
    else
        this->ui->videoLayout1->addWidget(widgetPtr);

}

/// Função executada a partir da escolha no menu Adicionar->Video
void MainWindow::addVideoOpenCv(std::string path)
{
    //Cria frame server
    FrameServerCv* frameServer = new FrameServerCv(path);

    //Cria o controle do frame server (controlWidget + frame server + async + img viewer)
    FrameServerControlWidget* fc = new FrameServerControlWidget(frameServer,generalSettings.getStartVideoFilePaused());
    QFileInfo file(QString::fromStdString(path));
    fc->setName(file.fileName());
    addFrameServerControl(fc);
}

void MainWindow::addVideoOpenCv()
{
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    //Usuario deve poder escolher entre webcams
    QSpinBox camNumber;
    form.addRow(new QLabel("Camera number"),&camNumber);

    //Resolucao da camera
    QSpinBox width,height;
    width.setRange(30,4000);
    width.setValue(640);
    height.setRange(30,4000);
    height.setValue(480);
    form.addRow(new QLabel("Width"),&width);
    form.addRow(new QLabel("Heigth"),&height);
    form.addRow(new QLabel("Note: Some resolutions may not be supported."));

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        //Cria frame server
        FrameServerCv* frameServer = new FrameServerCv(camNumber.value(),width.value(),height.value());

        //Cria o controle do frame server (widget + frame server + async + img viewer)
        FrameServerControlWidget* fc = new FrameServerControlWidget(frameServer,generalSettings.getStartWebcamPaused());
        fc->setName(QString("Webcam ").append(QString::number(camNumber.value())));

        addFrameServerControl(fc);
    }
}

void MainWindow::addFireWireCamera()
{
    //Cria frame server
    FrameServerVimba* frameServer = new FrameServerVimba();
    if(frameServer->hasNext()==false){
        QMessageBox::critical(this,"Error","No camera detected.");
        return;
    }

    //Cria o controle do frame server (widget + frame server + async + img viewer)
    FrameServerControlWidget* fc = new FrameServerControlWidget(frameServer);
    fc->setName(QString("FireWire Camera"));

    addFrameServerControl(fc);
}


/// FunÃ§Ã£o executada a partir da escolha no menu Adicionar->AnalogMeterDetector
void MainWindow::addAnalogMeterDetector()
{
    // Se existir mais de um frame server, o usuÃ¡rio deve poder escolher qual deles será usado
    FrameServerControlWidget* frameServerControl = this->chooseFrameServer();
    if(frameServerControl==NULL)
        return;

    AnalogMeterControl* detControl = new AnalogMeterControl(frameServerControl);

    //Adiciona widget de controle
    this->addWidgetInProcessArea(detControl->getWidget(),detControl->getName());
}


/// Adiciona FeatureTrackerKLT
void MainWindow::addFeatureTrackerKLT()
{
    // Se existir mais de um frame server, o usuÃ¡rio deve poder escolher qual deles será usado
    FrameServerControlWidget* frameServerControl = this->chooseFrameServer();
    if(frameServerControl==NULL)
        return;

    FeatureTrackerControl* trackerControl= new FeatureTrackerControl(frameServerControl);

    //Adiciona widget de controle
    this->addWidgetInProcessArea(trackerControl->getWidget(),trackerControl->getName());

}

/// Adiciona PresenceTracker
void MainWindow::addPresenceTracker()
{
    // Se existir mais de um frame server, o usuÃ¡rio deve poder escolher qual deles será usado
    FrameServerControlWidget* frameServerControl = this->chooseFrameServer();
    if(frameServerControl==NULL)
        return;

    PresenceTrackerControl* trackerControl= new PresenceTrackerControl(frameServerControl);

    //Adiciona widget de controle
    this->addWidgetInProcessArea(trackerControl->getWidget(),trackerControl->getName());
}

void MainWindow::addFireTracker()
{
    // Se existir mais de um frame server, o usuÃ¡rio deve poder escolher qual deles será usado
    FrameServerControlWidget* frameServerControl = this->chooseFrameServer();
    if(frameServerControl==NULL)
        return;

    FireTrackerControl* trackerControl= new FireTrackerControl(frameServerControl);

    //Adiciona widget de controle
    this->addWidgetInProcessArea(trackerControl->getWidget(),trackerControl->getName());
}

void MainWindow::addAnalogMeterNonFrontal()
{
    // Se existir mais de um frame server, o usuÃ¡rio deve poder escolher qual deles será usado
    FrameServerControlWidget* frameServerControl = this->chooseFrameServer();
    if(frameServerControl==NULL)
        return;

    AnalogMeterControlNF* trackControl = new AnalogMeterControlNF(frameServerControl);

    //Adiciona widget de controle
    this->addWidgetInProcessArea(trackControl->getWidget(),trackControl->getName());

}

/// Adiciona LevelTracker
void MainWindow::addLevelTracker()
{
    // Se existir mais de um frame server, o usuÃ¡rio deve poder escolher qual deles será usado
    FrameServerControlWidget* frameServerControl = this->chooseFrameServer();
    if(frameServerControl==NULL)
        return;

    LevelTrackerControl* trackerControl= new LevelTrackerControl(frameServerControl);

    //Adiciona widget de controle
    this->addWidgetInProcessArea(trackerControl->getWidget(),trackerControl->getName());
}

/// Adiciona LevelTracker
void MainWindow::addDiffractionLevelTracker()
{
    // Se existir mais de um frame server, o usuÃ¡rio deve poder escolher qual deles será usado
    FrameServerControlWidget* frameServerControl = this->chooseFrameServer();
    if(frameServerControl==NULL)
        return;

    DiffractionLevelTrackerControl* trackerControl= new DiffractionLevelTrackerControl(frameServerControl);

    //Adiciona widget de controle
    this->addWidgetInProcessArea(trackerControl->getWidget(),trackerControl->getName());
}


void MainWindow::addFrameServerControl(FrameServerControlWidget *control)
{
    //Remove controle da lista quando é fechado
    QObject::connect(control,&FrameServerControlWidget::destroyed,[=](){
        try{
            fsCtrls.remove(control);
        }
        catch(std::exception& e){
            std::cout<<e.what();
        }
    });

    //Adiciona image viewer e widget do frameserver na janela principal
    this->addWidgetInSourceArea(control);
    this->addWidgetInVideoArea(control->getImgViewer());

    this->fsCtrls.push_back(control);

}
