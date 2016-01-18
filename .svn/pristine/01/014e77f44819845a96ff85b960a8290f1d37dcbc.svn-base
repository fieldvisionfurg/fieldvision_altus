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

#include "ProcessControl.h"
#include "QHBoxLayout"
#include "QLineEdit"
#include "QFileDialog"
#include "QPushButton"
#include "QSlider"
#include "QDial"
#include "QDoubleSpinBox"


#include <QCheckBox>

ProcessControl::ProcessControl()
{
}

ProcessControl::ProcessControl(FrameServerControlWidget* frameServerControl_)
    :frameServerControl(frameServerControl_)
    ,widget(new ProcessWidget(this))
{

    // Processa botao de encerrar processo
    QObject::connect(widget,&ProcessWidget::closeButtonClicked,[&](){
        this->~ProcessControl();
    });

    // Conecta a atualização da imagem com o redesenhamento dos resultados
    QMetaObject::Connection connDraw = QObject::connect(frameServerControl->getImgViewer(),   // quando imageViewer emitir
                     &ImageViewerCv::imageRedraw,     //sinal que imagem foi redesenhada
                     frameServerControl->getImgViewer(),
                     [=](QPainter *painter)
                     {
                            if(widget->drawResult())
                                this->drawResults(painter);
                     },                        //desenha resultado
                     Qt::DirectConnection          //imediatamente
                     );
    // O Qt não desconecta funções lambda, então cria uma lista para desconetar no destrutor.
    connections.push_back(connDraw);
}

ProcessControl::~ProcessControl()
{
    for(QMetaObject::Connection& conn : connections)
        QObject::disconnect(conn);
    delete widget;
}

void ProcessControl::drawResults(QPainter *painter)
{

}

void ProcessControl::resetLog()
{

}

void ProcessControl::setStatus(const QString& status)
{
    widget->setStatus(status);
}

bool ProcessControl::exportLog(std::string fileName)
{
    return false;
}

ProcessWidget* ProcessControl::getWidget() const
{
    return widget;
}

QColor ProcessControl::getDrawColor()
{
    return widget->getDrawColor();
}

void ProcessControl::addNumericParameter(const QString name, double& param, const double min, const double max,const double initial, const int steps, const int style){
    std::function<void (double)> setParam = [&](double param_){
        param=param_;
    };
    addNumericParameter(name,setParam,min,max,initial,steps,style);
}

void ProcessControl::addNumericParameter(const QString name, int& param, const int min, const int max,const int initial){
    std::function<void (int)> setParam = [&](int param_){
        param=param_;
    };
    addNumericParameter(name,setParam,min,max,initial);
}

void ProcessControl::addNumericParameter(const QString name, const std::function<void (int)> setParam, const int min, const int max, const int initial)
{
    std::function<void (double)> setParamDouble = [&](double param_){
        setParam((int)param_); //converte para int
    };
    int steps=max-min+1;
    addNumericParameter(name,setParam,(double)min,(double)max,(double)initial,steps);
}

void ProcessControl::addNumericParameter(const QString name, const std::function<void (double)> setParam, const double min, const double max, const double initial, const int steps, const int style)
{
    //Cria slider
    QSlider *slider = new QSlider(Qt::Horizontal);
    //QDial *slider = new QDial();
    //slider->setWrapping(false);
    //slider->setNotchesVisible(true);


    slider->setMinimum(0);
    slider->setMaximum(steps-1);
    if(initial>=min && initial<=max){
        setParam(initial);
        slider->setValue(int((initial-min)*(steps-1)/(max-min)));
    }



    //Cria label com valor
    QLabel *label = new QLabel(name + ":");
    QDoubleSpinBox* spinBox = new QDoubleSpinBox();
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setSingleStep((max-min)/(steps-1));
    spinBox->setValue(initial);

    //Cria widget
    QWidget* w = new QWidget();
    QGridLayout* gridLayout = new QGridLayout(w);
    gridLayout->addWidget(label,0,0);
    gridLayout->addWidget(spinBox,0,1);
    gridLayout->addWidget(slider,1,0,1,2);


    //Chama setParam quando slider mudar o valor
    QObject::connect(slider,
                     &QSlider::valueChanged,
                     [=](const int newValue){
                        double dValue= (max-min)/(steps-1)*newValue+min;  //converte (line equation)
                        spinBox->setValue(dValue);
                        setParam(dValue);
                    });

    //Chama setParam quando spinBox mudar o valor
    QObject::connect(spinBox,
                     static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                     [=](double newValue){
                        slider->setValue(int((newValue-min)*(steps-1)/(max-min)));
                        setParam(newValue);
                    });

    widget->addParameterWidget(w);

}


void ProcessControl::addLeftClickHandler(const QString name, std::function<void (QPoint)> handlerFcn,bool enabled_)
{
    ImageViewerCv *imgViewer = frameServerControl->getImgViewer();

    //Cria checkbox para habilitar ou desabilitar captura.
    QCheckBox* enabled = new QCheckBox();
    enabled->setChecked(enabled_);

    //Cria labels
    QLabel *nameLabel = new QLabel(name +": ");
    QLabel *pointLabel = new QLabel(" ");

    //Cria widget
    QWidget* w = new QWidget();
    QHBoxLayout * layout = new QHBoxLayout (w);
    layout->addWidget(enabled);
    layout->addWidget(nameLabel);
    layout->addWidget(pointLabel);

    //Função executada no clicke
    std::function<void (QPoint)> clicked = [=](QPoint p){
        if(enabled->isChecked()){
            pointLabel->setText(QString::number(p.x())+QString(",")+QString::number(p.y()));
            handlerFcn(p);
        }
    };
    QMetaObject::Connection conn= QObject::connect(imgViewer,&ImageViewerCv::leftClick,clicked);
    connections.push_back(conn);

    getWidget()->addParameterWidget(w);
}

void ProcessControl::addMultiClickHandler(const QString name, const unsigned int numOfPoints, std::function<void (std::vector<QPoint>)> handlerFcn,bool enabled_)
{
    class MultiClickWidget : public QWidget
    {
    public:
        MultiClickWidget(const QString name,ImageViewerCv *imgViewer,const unsigned int numOfPoints,std::function<void (std::vector<QPoint>)> handlerFcn,bool enabled_)
            :points(numOfPoints)
            ,processPoints(handlerFcn)
        {

            clickCount=0;
            numPoints=numOfPoints;
            //Cria labels
            QLabel *nameLabel = new QLabel(name);
            std::vector<QLabel*> pointLabels(numPoints);

            for(unsigned int i=0;i<numPoints;i++)
                pointLabels[i]=new QLabel("0");

            //Cria checkbox para habilitar ou desabilitar captura.
            QCheckBox* enabled = new QCheckBox();
            enabled->setChecked(enabled_);

            //Cria widget
            QGridLayout* gridLayout = new QGridLayout();
            gridLayout->addWidget(enabled,0,0);
            gridLayout->addWidget(nameLabel,0,1);
            for(unsigned int i=0;i<numPoints;i++)
                gridLayout->addWidget(pointLabels[i],i+1,1);
            setLayout(gridLayout);

            //Função executada no clicke
            std::function<void (QPoint)> clicked = [=](QPoint p){
                if(enabled->isChecked()){
                    // mostra ponto na tela
                    int pointIndex = clickCount % numPoints;
                    pointLabels[pointIndex]->setText(QString::number(p.x())+QString(",")+QString::number(p.y()));
                    //armazena ponto
                    points[pointIndex]=p;

                    // se alcançou numero de pontos, chama handlerFcn
                    if(pointIndex==(numPoints-1))
                        processPoints(points);

                    clickCount++;
                }
            };
            conn=QObject::connect(imgViewer,&ImageViewerCv::leftClick,clicked);
        }

        // Destrutor
        ~MultiClickWidget()
        {
            // Para de capturar cliques
            QObject::disconnect(conn);
        }

    private:
        unsigned int clickCount;
        unsigned int numPoints;
        std::vector<QPoint> points;
        std::function<void (std::vector<QPoint>)> processPoints;
        QMetaObject::Connection conn;
    };

    widget->addParameterWidget(new MultiClickWidget(name,frameServerControl->getImgViewer(),numOfPoints,handlerFcn,enabled_));

}


void ProcessControl::addSubImageAndRectHandler(const QString name, std::function<void(cv::Mat,cv::Rect)> handlerFcn,bool enabled_)
{
    //Cria widget para mostrar sub-imagem
     ImageViewerCv* referenceView = new ImageViewerCv();
     referenceView->changeTitle("Selection");

    //Quando usuário clica duas vezes na tela, define um retângulo contendo a sub-imagem
    //Define a função que trata dos pontos clicados
    std::function<void(std::vector<QPoint>)> fcn = [=](std::vector<QPoint> points){
        //Obtem imagem atual, cria um retângulo baseado nos pontos clicados e chama handlerFcn
        cv::Mat img = frameServerControl->getImgViewer()->getCvImg();

        cv::Point topLeft(points[0].x(),points[0].y());
        cv::Point bottonRight(points[1].x(),points[1].y());
        cv::Rect objRect(topLeft,bottonRight);

        cv::Mat reference(img,objRect);
        referenceView->setImage(reference);

        handlerFcn(reference,objRect);
    };
    this->addMultiClickHandler(name,2,fcn,enabled_);
    this->getWidget()->addParameterWidget(referenceView);
}

void ProcessControl::addSubImageHandler(const QString name, std::function<void(cv::Mat)> handlerFcn,bool enabled_)
{
   // ignore rect
   std::function<void(cv::Mat,cv::Rect)> f = [handlerFcn](cv::Mat img,cv::Rect r) {
       handlerFcn(img);
   };
   this->addSubImageAndRectHandler(name,f,enabled_);
}


void ProcessControl::addFileChoice(const QString name, std::function<void (const std::string)> handlerFcn)
{
    //TODO: adicionar valor inicial

    //Cria label com nome
    QLabel *label = new QLabel(name);

    //Cria edit box que mostra o full path do arquivo
    QLineEdit* fileEdit = new QLineEdit();

    //Cria botão que abre dialogo de arquivo e chama função handler
    QPushButton* openBtn = new QPushButton("Open");
    QObject::connect(openBtn,&QPushButton::clicked,[=](){
        QString detecName = QFileDialog::getOpenFileName(getWidget(), "Change Detector",
                                                        "..\\..\\ExternalLibraries\\OpenCV2.4.9\\data\\haarcascades" );
       fileEdit->setText(detecName);
       handlerFcn(detecName.toStdString());
    });

    //Cria widget
    QWidget* w = new QWidget();
    QGridLayout* gridLayout = new QGridLayout(w);
    gridLayout->addWidget(label,0,0,1,2);
    gridLayout->addWidget(fileEdit,1,1,1,2);
    gridLayout->addWidget(openBtn,2,1);
    widget->addParameterWidget(w);

}

void ProcessControl::addBoolParameter(const QString name, bool& param)
{
    //Cria checkbox para habilitar ou desabilitar parametro.
    QCheckBox* enabled = new QCheckBox(name);
    enabled->setChecked(param);
    QObject::connect(enabled,&QCheckBox::stateChanged,[&](int state){
       if(state==Qt::Checked)
           param=true;
       else
           param=false;
    });
    widget->addParameterWidget(enabled);
}

void ProcessControl::addBoolParameter(const QString name, std::function<void(bool)> setParam, const bool initial)
{
    //Cria checkbox para habilitar ou desabilitar parametro.
    QCheckBox* enabled = new QCheckBox(name);
    enabled->setChecked(initial);
    QObject::connect(enabled,&QCheckBox::stateChanged,[&](int state){
       if(state==Qt::Checked)
           setParam(true);
       else
           setParam(false);
    });
    widget->addParameterWidget(enabled);
}

QString ProcessControl::getName() const
{
    return name;
}

void ProcessControl::setName(const QString &value)
{
    name = value;
    widget->setTitle(name);
}



