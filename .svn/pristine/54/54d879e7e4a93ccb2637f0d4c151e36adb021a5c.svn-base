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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollBar>
#include "GeneralSettings.h"
#include "AboutDialog.h"

class FrameServerControlWidget; //Forward declaration

namespace Ui {
class MainWindow;
}

/// Classe principal da aplicação FieldVision.
/** Implementa a interface de usuário para gerenciar a adição e execução de videos, detectores e rastreadores.
 *
 *
 * \ingroup FieldVision
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    /// Interface gráfica do usuário.
    Ui::MainWindow *ui;

    /// Vetor para armazenar os FrameServers adicionados.
    std::list<FrameServerControlWidget*> fsCtrls;

    /// Mostra para o usuário os frame servers criados e permite que ele escolha um.
    FrameServerControlWidget* chooseFrameServer();

    /// Função que adiciona um widget na área destinada as fontes de ví­deo (FrameServers)
    void addWidgetInSourceArea(QWidget* widget);

    /// Função que adiciona um widget na área destinada aos métodos de processamento (detectores e rastreadores)
    void addWidgetInProcessArea(QWidget* widget,const QString name);

    /// Função que adiciona um widget na área destinada à visualização dos videos (ImageViewerCv)
    void addWidgetInVideoArea(QWidget* widget);

    /// General application settings.
    GeneralSettings generalSettings;

    /// About FieldVision dialog box
    AboutDialog aboutDialog;

    int videoCount;

    void addFrameServerControl(FrameServerControlWidget *control);

private slots:

    void addUrl();

    void addLocal();

    /// Funçãoo executada a partir da escolha no menu Adicionar->Video
    void addVideoOpenCv(std::string path);

    void addVideoOpenCv();

    void addFireWireCamera();

    /// Função executada a partir da escolha no menu Adicionar->AnalogMeterDetector
    void addAnalogMeterDetector();

    void addFeatureTrackerKLT();

    void addLevelTracker();

    void addDiffractionLevelTracker();

    void addPresenceTracker();

    void addFireTracker();

    void addAnalogMeterNonFrontal();

};

#endif // MAINWINDOW_H
