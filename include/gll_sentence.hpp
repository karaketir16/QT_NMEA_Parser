#ifndef GLL_SENTENCE_H
#define GLL_SENTENCE_H

#include "sentencebase.hpp"


namespace nmea{
namespace gll {

enum FieldsEnum{
	LATITUDE,
	N_or_S,
	LONGITUDE,
	E_or_W,
	UTC,
	STATUS,
    MODE,
    __FieldsEnum__MAX
};

enum Validity {
    VALID = 'A',
    INVALID = 'V',
};
/*
NOTE 2 The mode indicator field supplements the
status field (field 6). The status field should
be set to V = invalid for all values of operating
mode except for A = Autonomous and D = Differential.
The positioning system mode indicator and status
fields should not be null fields.
*/
enum ModeEnum {
    AutonomousMode = 'A',
    DifferentialMode = 'D',
    EstimatedMode   ='E',
    ManualInputMode ='M',
    SimulatorMode = 'S',
    DataNotValid = 'N'
};

class GLL_Sentence : public SentenceBase
{
public:
    GLL_Sentence(std::string talkerID = "--", Fields_t fList = {})
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList)
    {
        this->sentenceID = "GLL";
    }

    void setTime(Time_t time){
        fields[UTC] = timeTo_hhmmss(time);
    }

    Time_t getTime() const{
        return hhmmssToTime(fields[UTC]);
    }

    void setLatitude(double lat){
        auto tmp = latDegreeTo_ddmm(lat);
        fields[LATITUDE] = tmp.first;
        fields[N_or_S] = tmp.second;
    }

    void setLongitude(double lon){
        auto tmp = longDegreeTo_ddmm(lon);
        fields[LONGITUDE] = tmp.first;
        fields[E_or_W] = tmp.second;
    }
private:
    //See Note 2
    void setStatus(bool valid){
    	if(valid){
    		fields[STATUS] = VALID;
    	} else {
    		fields[STATUS] = INVALID;
    	}
    }
public:
    void setMode(ModeEnum mode){
        switch (mode) {
        case nmea::gll::AutonomousMode:
        case nmea::gll::DifferentialMode:
            setStatus(true);
            break;
        case nmea::gll::EstimatedMode:
        case nmea::gll::ManualInputMode:
        case nmea::gll::SimulatorMode:
        case nmea::gll::DataNotValid:
            setStatus(false);
            break;
        default:
            break;
        }
        fields[MODE] = mode;
    }

    bool getStatus() const{
        if(fields[STATUS].empty()){
            return false;
        }else if(fields[STATUS][0] == VALID){
            return true;
        }
        return false;
    }

    double getLatitude() const{
        if(fields[LATITUDE].empty()){
            return 0;
        }
        return ddmmToLatDegree(std::make_pair(fields[LATITUDE], fields[N_or_S] == "S" ? 'S' : 'N'));
    }

    double getLongitude() const{
        if(fields[LONGITUDE].empty()){
            return 0;
        }
        return ddmmToLongDegree(std::make_pair(fields[LONGITUDE], fields[E_or_W] == "W" ? 'W' : 'E'));
    }

    ModeEnum getMode() const{
        if(fields[STATUS].empty()){
            return DataNotValid;
        }

        return static_cast<ModeEnum>(fields[MODE][0]);
    }

};



}
}

Q_DECLARE_METATYPE(nmea::gll::GLL_Sentence)


#endif // GLL_SENTENCE_H
