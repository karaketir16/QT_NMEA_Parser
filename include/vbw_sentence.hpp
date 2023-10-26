#ifndef VBW_SENTENCE_H
#define VBW_SENTENCE_H

#include "sentencebase.hpp"
#include "structs_enums.hpp"
#include <QPair>
#include <QString>


namespace nmea{
namespace vbw {

enum FieldsEnum{
    LONGITUDINAL_WATER_SPEED = 0,
    TRANSVERSE_WATER_SPEED,
    STATUS_WATER_SPEED,
    LONGITUDINAL_GROUND_SPEED,
    TRANSVERSE_GROUND_SPEED,
    STATUS_GROUND_SPEED,
#ifdef NMEA_ABOVE_3
    STERN_TRANSVERSE_WATER_SPEED,
    STATUS_STERN_TRANSVERSE_WATER_SPEED,
    STERN_TRANSVERSE_GROUND_SPEED,
    STATUS_STERN_TRANSVERSE_GROUND_SPEED,
#endif
    __FieldsEnum__MAX
};

enum Validity {
    VALID = 'A',
    INVALID = 'V',
};

class VBW_Sentence : public SentenceBase
{
public:
    VBW_Sentence(std::string talkerID = "--", Fields_t fList = {}) 
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList)
    {
        this->sentenceID = "VBW";
    }

    void setWaterSpeed(double longitudinal, double transverse, Validity validity = VALID, int precision = 3){
        fields[LONGITUDINAL_WATER_SPEED] = toStringFloat(longitudinal, precision);
        fields[TRANSVERSE_WATER_SPEED] = toStringFloat(transverse, precision);
        fields[STATUS_WATER_SPEED] = validity;
    }

    void setGroundSpeed(double longitudinal, double transverse, Validity validity = VALID, int precision = 3){
        fields[LONGITUDINAL_GROUND_SPEED] = toStringFloat(longitudinal, precision);
        fields[TRANSVERSE_GROUND_SPEED] = toStringFloat(transverse, precision);
        fields[STATUS_GROUND_SPEED] = validity;
    }

    QPair<Speed, Speed> getWaterSpeed(bool *valid){
        *valid = (fields[STATUS_WATER_SPEED] == "A");

        return {
            QString::fromStdString( fields[LONGITUDINAL_WATER_SPEED] ).toDouble(),
            QString::fromStdString( fields[TRANSVERSE_WATER_SPEED] ).toDouble()
        };
    }

    QPair<Speed, Speed> getGroundSpeed(bool *valid){
        *valid = (fields[STATUS_GROUND_SPEED] == "A");

        return {
            QString::fromStdString( fields[LONGITUDINAL_GROUND_SPEED] ).toDouble(),
            QString::fromStdString( fields[TRANSVERSE_GROUND_SPEED] ).toDouble()
        };
    }
};



}
}

Q_DECLARE_METATYPE(nmea::vbw::VBW_Sentence)


#endif // VBW_SENTENCE_H
