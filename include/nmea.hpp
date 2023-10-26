#ifndef NMEA_HPP
#define NMEA_HPP

#include "nmeaparser.hpp"
#include "gga_sentence.hpp"
#include "mwv_sentence.hpp"
#include "hdt_sentence.hpp"
#include "vbw_sentence.hpp"
#include "vhw_sentence.hpp"
#include "vtg_sentence.hpp"
#include "dpt_sentence.hpp"
#include "gll_sentence.hpp"
#include "hrp_sentence.hpp"

#include <iostream>

#include <stdint.h>
#include <QObject>
#include <QMetaType>
#include <QDebug>



namespace nmea{

enum class SentenceID {
    GGA,
    HDT,
    MWV,
    VBW,
    DPT,
    GLL,
    VHW,
    VTG,
    HRP,
    NONE
};

class Nmea : public QObject {
    Q_OBJECT
public:
    SentenceID nmeaMAP(std::string s){

#define __nmap_macro(x) \
    if(s == #x) return SentenceID :: x

        __nmap_macro(GGA);
        __nmap_macro(HDT);
        __nmap_macro(MWV);
        __nmap_macro(VBW);
        __nmap_macro(DPT);
        __nmap_macro(GLL);
        __nmap_macro(VHW);
        __nmap_macro(VTG);
        __nmap_macro(HRP);

        return SentenceID::NONE;
    }



    NmeaParser<Nmea> parser;
    
    Nmea(QObject *parent = nullptr)  : QObject(parent), parser(this, &Nmea::_onParse){
        connect(this, &Nmea::parseSentenceSignal, this, &Nmea::parseSentence, Qt::QueuedConnection);

        qRegisterMetaType<nmea::gga::GGA_Sentence>("nmea::gga::GGA_Sentence");
        qRegisterMetaType<nmea::hdt::HDT_Sentence>("nmea::hdt::HDT_Sentence");
        qRegisterMetaType<nmea::mwv::MWV_Sentence>("nmea::mwv::MWV_Sentence");
        qRegisterMetaType<nmea::vbw::VBW_Sentence>("nmea::vbw::VBW_Sentence");
        qRegisterMetaType<nmea::dpt::DPT_Sentence>("nmea::dpt::DPT_Sentence");
        qRegisterMetaType<nmea::gll::GLL_Sentence>("nmea::gll::GLL_Sentence");
        qRegisterMetaType<nmea::vhw::VHW_Sentence>("nmea::vhw::VHW_Sentence");
        qRegisterMetaType<nmea::vtg::VTG_Sentence>("nmea::vtg::VTG_Sentence");
        qRegisterMetaType<nmea::hrp::HRP_Sentence>("nmea::hrp::HRP_Sentence");
        
    }

    void parse(std::string str){
        for(unsigned i = 0; i < str.length(); i++){
            parser.parse(str[i]);
        }
    }

    void parse(char* str, int len){
        for(int i = 0; i < len; i++){
            parser.parse(str[i]);
        }
    }

    void parse(char ch){
        parser.parse(ch);
    }

    void _onParse(std::string s){
        emit parseSentenceSignal(s);
    }

signals:
    void parseSentenceSignal(std::string sentence);

#define _signalMacro(_lower, _upper)\
    void _lower##Received(nmea:: _lower :: _upper##_Sentence)

    _signalMacro(gga, GGA);
    _signalMacro(hdt, HDT);
    _signalMacro(mwv, MWV);
    _signalMacro(vbw, VBW);
    _signalMacro(dpt, DPT);
    _signalMacro(gll, GLL);
    _signalMacro(vhw, VHW);
    _signalMacro(vtg, VTG);
    _signalMacro(hrp, HRP);


public slots:

    void parseSentence(std::string sentence){
        auto truncatedS = sentence.substr(1, sentence.length() - 6);

        auto res = nmea::SentenceBase::split(truncatedS);

        auto len = res[0].length();

        auto talkerID_tmp = res[0].substr(0, (len - 3));
        auto sentenceID_tmp = res[0].substr((len - 3), 3);

        res.erase(res.begin());

        switch (nmeaMAP(sentenceID_tmp)) {
#define _swCaseMacro(_lower, _upper)\
        case nmea::SentenceID::_upper:\
            emit _lower##Received(nmea:: _lower :: _upper##_Sentence(talkerID_tmp, res));\
            break

        _swCaseMacro(gga, GGA);
        _swCaseMacro(hdt, HDT);
        _swCaseMacro(mwv, MWV);
        _swCaseMacro(vbw, VBW);
        _swCaseMacro(dpt, DPT);
        _swCaseMacro(gll, GLL);
        _swCaseMacro(vhw, VHW);
        _swCaseMacro(vtg, VTG);
        _swCaseMacro(hrp, HRP);


        default:
            break;
        }
    }

};


}




#endif // NMEA_HPP
