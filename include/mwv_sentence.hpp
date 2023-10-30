#ifndef MWV_SENTENCE_H
#define MWV_SENTENCE_H

#include <QString>
#include <QMetaType>
#include <QPair>
#include "sentencebase.hpp"
#include <structs_enums.hpp>
#include <QDebug>

namespace nmea{

namespace mwv {
    
enum FieldsEnum{
    eWindAngle,
    eReference,
    eWindSpeed,
    eWindSpeedUnits,
    eStatus,
    __FieldsEnum__MAX
};

enum ReferenceEnum{
    eRelative   = 'R',
    eTrue       = 'T'
};

enum WindSpeedUnitsEnum{
    eKph   = 'K',
    eMps   = 'M',
    eKnots   = 'N'
};

class MWV_Sentence : public SentenceBase
{
public:
    MWV_Sentence(std::string talkerID = "--", Fields_t fList = {})
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList) 
    {
        this->sentenceID = "MWV";
    }

    void setWindAngle(double angle){
        fields[eWindAngle] = QString::number(angle).toStdString();;
    }

    void setReference(ReferenceEnum ref){
        fields[eReference] = static_cast<char>(ref);
    }
    void setWindSpeed(double speed, WindSpeedUnitsEnum unit){
        fields[eWindSpeed] = QString::number(speed).toStdString();
        fields[eWindSpeedUnits] = static_cast<char>(unit);
    }

    void setStatus(bool valid){
        fields[eStatus] = (valid ? 'A' : 'V');
    }


    double getWindAngle() const{
        double angle = 0;

        if(! fields[eWindAngle].empty()){
            angle = QString::fromStdString(fields[eWindAngle]).toDouble();
        }

        return angle;
    }

    ReferenceEnum getReference() const{
        ReferenceEnum ref = ReferenceEnum::eRelative;

        if(! fields[eReference].empty()){
            ref = static_cast<ReferenceEnum>(fields[eReference][0]);
        }
        return ref;
    }

    QPair<double, WindSpeedUnitsEnum> getWindSpeed() const{
        double speed = 0;
        WindSpeedUnitsEnum unit = WindSpeedUnitsEnum::eMps;

        if(! fields[eWindSpeed].empty()){
            speed = QString::fromStdString(fields[eWindSpeed]).toDouble();
        }
        if(! fields[eWindSpeedUnits].empty()){
            unit = static_cast<WindSpeedUnitsEnum>(fields[eWindSpeedUnits][0]);
        }

        return qMakePair(speed, unit);
    }

    bool getStatus() const{
        if(fields[eStatus].empty()){
            return false;
        }else if(fields[eStatus][0] == 'A'){
            return true;
        }
        return false;
    }

    Speed getSpeed() const{
        Speed s;
        auto su = getWindSpeed();
        switch (su.second) {
        case nmea::mwv::eKph:
            s.setSpeedKiloMeterHour(su.first);
            break;
        case nmea::mwv::eMps:
            s.setSpeedMeterSecond(su.first);
            break;
        case nmea::mwv::eKnots:
            s.setSpeedKnots(su.first);
            break;
        }
        return s;
    }

    void setSpeed(Speed s){
        setWindSpeed(s.getSpeedKnots(), nmea::mwv::eKnots);
    }

};



}
}

Q_DECLARE_METATYPE(nmea::mwv::MWV_Sentence)

#endif // MWV_SENTENCE_H
