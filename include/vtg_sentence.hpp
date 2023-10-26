#ifndef VTG_SENTENCE_H
#define VTG_SENTENCE_H

#include "sentencebase.hpp"
#include "structs_enums.hpp"

namespace nmea{
namespace vtg {

enum FieldsEnum{
    COURSE_OVER_GROUND_DEGRESS_TRUE = 0, //track made good
    eT,
    COURSE_OVER_GROUND_DEGRESS_MAGNETIC,
    eM,
    GROUND_SPEED_KNOTS,
    eN,
    GROUND_SPEED_KPH,
    eK,
    MODE,
    __FieldsEnum__MAX
};

enum Validity {
    VALID = 'A',
    INVALID = 'V',
};

/*
NOTE 1 The speed over the ground should always be non-negative.
NOTE 2 The mode indicator provides status information about
the operation of the source device (such as positioning systems,
velocity sensors, etc.) generating the sentence, and the validity
of data being provided. The possible indications are as follows:
A = Autonomous mode;
D = Differential mode;
E = Estimated (dead reckoning) mode;
M = Manual input mode;
P = Precise. Satellite system used in precision mode. Precision mode is defined as: no deliberate degradation (such
as selective availability) and higher resolution code (P-code) is used to compute position fix. P is also used for
satellite system used in multi-frequency, SBAS or Precise Point Positioning (PPP) mode;
S = Simulator mode;
N = Data not valid.
The mode indicator field should not be a null field.
*/
enum ModeEnum {
    AutonomousMode = 'A',
    DifferentialMode = 'D',
    EstimatedMode   ='E',
    ManualInputMode ='M',
    SimulatorMode = 'S',
    DataNotValid = 'N'
};

class VTG_Sentence : public SentenceBase
{
public:
    VTG_Sentence(std::string talkerID = "--", Fields_t fList = {})
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList)
    {
        this->sentenceID = "VTG";
    }

    void setHeadingTrue(double heading, int precision = 3){
        fields[COURSE_OVER_GROUND_DEGRESS_TRUE] = toStringFloat(heading, precision);
        fields[eT] = "T";
    }

    void setHeadingMagnetic(double heading, int precision = 3){
        fields[COURSE_OVER_GROUND_DEGRESS_MAGNETIC] = toStringFloat(heading, precision);
        fields[eM] = "M";
    }

    void setGroundSpeed(Speed speed, int precision = 3){
        fields[GROUND_SPEED_KNOTS] = toStringFloat(speed.getSpeedKnots(), precision);
        fields[eN] = "N";

        fields[GROUND_SPEED_KPH] = toStringFloat(speed.getSpeedKiloMeterHour(), precision);
        fields[eK] = "K";
    }

    double getHeadingTrue(){
        return QString::fromStdString(fields[COURSE_OVER_GROUND_DEGRESS_TRUE]).toDouble();
    }

    double getHeadingMagnetic(){
        return QString::fromStdString(fields[COURSE_OVER_GROUND_DEGRESS_MAGNETIC]).toDouble();
    }

    Speed getGroundSpeed(){

        if(! fields[GROUND_SPEED_KNOTS].empty()){
            return QString::fromStdString(fields[GROUND_SPEED_KNOTS]).toDouble();
        }

        if(! fields[GROUND_SPEED_KPH].empty()){
            Speed s;
            s.setSpeedKiloMeterHour(QString::fromStdString(fields[GROUND_SPEED_KPH]).toDouble());
            return s;
        }

        return 0;
    }


    void setMode(ModeEnum mode){
        fields[MODE] = mode;
    }

    ModeEnum getMode(){
        return static_cast<ModeEnum>(fields[MODE][0]);
    }
};



}
}

Q_DECLARE_METATYPE(nmea::vtg::VTG_Sentence)


#endif // VTG_SENTENCE_H
