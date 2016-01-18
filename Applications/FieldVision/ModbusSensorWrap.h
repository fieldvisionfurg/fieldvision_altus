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
#ifndef MODBUSSENSORWRAP_H
#define MODBUSSENSORWRAP_H

#include "VisionCore.h"
#include "VisionSensors.h"
#include "modbus.h"


/// Prove comunicação Modbus para sensores visuais (trackers).
/** Esta classe faz um wrap de um sensor (tracker) e envia os resultados via
 * protocolo Modbus.
 *
 *
 * \ingroup FieldVision
 */
template<class TImg,class TObj>
class ModbusSensorWrap : public VisionCore::Tracker<TImg,TObj>
{
public:
    ModbusSensorWrap(VisionCore::Tracker<TImg,TObj>* sensor);

    virtual ~ModbusSensorWrap();

    /// Conecta a um dispositivo modbus
    void connectModbus(std::string ip,int port, int address);

    // Tracker interface
    void update(const VisionCore::Frame<TImg>& frame);
    const TObj &getLastTrack();
    bool lostTrack() const;

private:

    void send(const double& data);

    void send(const std::vector<VisionSense::AnalogMeter>& data);

    void send(const std::vector<cv::Point2f>& points);

    void send(std::vector<std::vector<cv::Point>> obj);

    void send(bool presence);


    VisionCore::Tracker<TImg,TObj>* sensor;

    bool ownSensor;

    /// Conexão modbus.
    modbus_t *mb;

    /// Registradores modbus.
    uint16_t tab_reg[1000];

    /// Endereço de memoria no receptor que será escrito.
    int modbusAddress;

};

template<class TImg,class TObj>
ModbusSensorWrap<TImg,TObj>::ModbusSensorWrap(VisionCore::Tracker<TImg,TObj>* sensor)
    :sensor(sensor)
    ,ownSensor(true)
    ,mb(NULL)
{

}

template<class TImg,class TObj>
ModbusSensorWrap<TImg,TObj>::~ModbusSensorWrap()
{
    if(mb!=NULL){
        modbus_close(mb);
    }
    if(sensor && ownSensor)
        delete sensor;
}

template<class TImg,class TObj>
void ModbusSensorWrap<TImg,TObj>::update(const VisionCore::Frame<TImg>& frame)
{
    if(sensor!=NULL){
        sensor->update(frame);
        if(mb!=NULL){
            this->send(sensor->getLastTrack());
        }
    }
}

template<class TImg,class TObj>
const TObj& ModbusSensorWrap<TImg,TObj>::getLastTrack()
{
    return sensor->getLastTrack();
}

template<class TImg,class TObj>
bool ModbusSensorWrap<TImg,TObj>::lostTrack() const
{
    return sensor->lostTrack();
}

template<class TImg,class TObj>
void ModbusSensorWrap<TImg,TObj>::connectModbus(std::string ip, int port, int address)
{
    if(mb!=NULL){
        modbus_close(mb);
    }

    mb=modbus_new_tcp(ip.c_str(), port);
    modbusAddress=address;
    modbus_connect(mb);
}

template<class TImg,class TObj>
void ModbusSensorWrap<TImg,TObj>::send(const double &data)
{
     //Evia via modbus
     tab_reg[0]=(uint16_t)(data*1000);
     modbus_write_registers(mb, modbusAddress, 1, tab_reg);
}


template<class TImg,class TObj>
void ModbusSensorWrap<TImg,TObj>::send(const std::vector<VisionSense::AnalogMeter>& data)
{
    //Evia via modbus
    int count=0;
    for(const VisionSense::AnalogMeter& m : data){
        tab_reg[count*4]=m.center.x;
        tab_reg[count*4+1]=m.center.y;
        tab_reg[count*4+2]=m.radius;
        tab_reg[count*4+3]=m.pointerAngle;
        count++;
    }
    modbus_write_registers(mb, modbusAddress, count*4, tab_reg);
}

template<class TImg,class TObj>
void ModbusSensorWrap<TImg,TObj>::send(const std::vector<cv::Point2f>& points)
{
    //Evia via modbus
    int count=0;
    for(const cv::Point2f& point : points){
        tab_reg[2*count]=(uint16_t)(point.x);
        tab_reg[2*count+1]=(uint16_t)(point.y);
        count++;
    }
    modbus_write_registers(mb, modbusAddress, count*2, tab_reg);
}

//Fire
template<class TImg,class TObj>
void ModbusSensorWrap<TImg,TObj>::send(std::vector<std::vector<cv::Point>> obj)
{
    //Evia via modbus
    if(obj.size()>0) //is there fire?
        tab_reg[0]=1;
    else
        tab_reg[0]=0;
    modbus_write_registers(mb, modbusAddress, 1, tab_reg);
}

template<class TImg,class TObj>
void ModbusSensorWrap<TImg,TObj>::send(bool presence)
{
    //Evia via modbus
    tab_reg[0]=presence ? 0 : 1;
    modbus_write_registers(mb, modbusAddress, 1, tab_reg);
}



#endif // MODBUSSENSORWRAP_H
