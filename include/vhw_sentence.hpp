#ifndef VHW_SENTENCE_H
#define VHW_SENTENCE_H

#include "sentencebase.hpp"
#include "structs_enums.hpp"

namespace nmea{
namespace vhw {

enum FieldsEnum{
    HEADING_DEGRESS_TRUE = 0,
    eT,
    HEADING_DEGRESS_MAGNETIC,
    eM,
    WATER_SPEED_KNOTS,
    eN,
    WATER_SPEED_KPH,
    eK,

    __FieldsEnum__MAX
};

enum Validity {
    VALID = 'A',
    INVALID = 'V',
};

class VHW_Sentence : public SentenceBase
{
public:
    VHW_Sentence(std::string talkerID = "--", Fields_t fList = {})
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList)
    {
        this->sentenceID = "VHW";
    }

    void setHeadingTrue(double heading, int precision = 3){
        fields[HEADING_DEGRESS_TRUE] = toStringFloat(heading, precision);
        fields[eT] = "T";
    }

    void setHeadingMagnetic(double heading, int precision = 3){
        fields[HEADING_DEGRESS_MAGNETIC] = toStringFloat(heading, precision);
        fields[eM] = "M";
    }

    void setWaterSpeed(Speed speed, int precision = 3){
        fields[WATER_SPEED_KNOTS] = toStringFloat(speed.getSpeedKnots(), precision);
        fields[eN] = "N";

        fields[WATER_SPEED_KPH] = toStringFloat(speed.getSpeedKiloMeterHour(), precision);
        fields[eK] = "K";
    }

    double getHeadingTrue() const{
        return QString::fromStdString(fields[HEADING_DEGRESS_TRUE]).toDouble();
    }

    double getHeadingMagnetic() const{
        return QString::fromStdString(fields[HEADING_DEGRESS_MAGNETIC]).toDouble();
    }

    Speed getWaterSpeed() const {

        if(! fields[WATER_SPEED_KNOTS].empty()){
            return QString::fromStdString(fields[WATER_SPEED_KNOTS]).toDouble();
        }

        if(! fields[WATER_SPEED_KPH].empty()){
            Speed s;
            s.setSpeedKiloMeterHour(QString::fromStdString(fields[WATER_SPEED_KPH]).toDouble());
            return s;
        }

        return 0;
    }
};



}
}

Q_DECLARE_METATYPE(nmea::vhw::VHW_Sentence)


#endif // VHW_SENTENCE_H
