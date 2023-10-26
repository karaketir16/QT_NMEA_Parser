#ifndef HRP_SENTENCE_H
#define HRP_SENTENCE_H

#include "sentencebase.hpp"
#include "structs_enums.hpp"

namespace nmea{
namespace hrp {

enum FieldsEnum{
    HEADING_DEGREES,
    T_Field,
    ROLL,
    PITCH,
    P_Field,
    __FieldsEnum__MAX
};

class HRP_Sentence : public SentenceBase
{
public:
    HRP_Sentence(std::string talkerID = "--", Fields_t fList = {})
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList) 
    {
        this->sentenceID = "HRP";
        if(fList.empty()){
            fields[P_Field]= "P";
        }
    }

    void setHeading(double heading){
    	fields[HEADING_DEGREES] = toStringFloat(heading, 3);
        fields[T_Field] = "T";
    }

    double getHeading() const{
        double heading = 0;

        if(! fields[HEADING_DEGREES].empty()){
            heading = QString::fromStdString(fields[HEADING_DEGREES]).toDouble();
        }

        return heading;
    }

    void setRoll(double roll){
        fields[ROLL] = toStringFloat(roll, 3);
    }

    double getRoll() const{
        double roll = 0;

        if(! fields[ROLL].empty()){
            roll = QString::fromStdString(fields[ROLL]).toDouble();
        }

        return roll;
    }


    void setPitch(double pitch){
        fields[PITCH] = toStringFloat(pitch, 3);
    }

    double getPitch() const{
        double pitch = 0;

        if(! fields[PITCH].empty()){
            pitch = QString::fromStdString(fields[PITCH]).toDouble();
        }

        return pitch;
    }

    void setAttitude(Attitude att){
        setHeading(att.heading);
        setRoll(att.roll);
        setPitch(att.pitch);
    }

    Attitude getAttitude() const{
        Attitude att;
        att.heading = getHeading();
        att.roll = getRoll();
        att.pitch = getPitch();
        return att;
    }

};



}
}

Q_DECLARE_METATYPE(nmea::hrp::HRP_Sentence)


#endif // GLL_SENTENCE_H
