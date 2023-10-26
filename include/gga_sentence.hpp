#ifndef GGA_SENTENCE_H
#define GGA_SENTENCE_H

#include "sentencebase.hpp"

namespace nmea{

namespace gga {
    
enum FieldsEnum{
    UTC,
    LATITUDE,
    N_or_S,
    LONGITUDE,
    E_or_W,
    GPS_QUALITY_INDICATOR,
    NUMBER_OF_SATALLITES_IN_USE,
    HORIZONTAL_DILUTION,
    ANTENNA_ALTITUDE,
    UNITS_OF_ANTENNA_ALTITUDE,
    GEOIDAL_SEPARATION,
    UNITS_OF_GEOIDAL_SEPARATION,
    AGE_OF_DIFFERENTIAL_GPS_DATA,
    DIFFERENTIAL_REFERENCE_STATION,
    __FieldsEnum__MAX
};



class GGA_Sentence : public SentenceBase
{
public:
    GGA_Sentence(std::string talkerID = "--", Fields_t fList = {}) 
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList) 
    {
        this->sentenceID = "GGA";
    }

    void setTime(Time_t time){
        fields[UTC] = timeTo_hhmmss(time);
    }

    void setTime(int hour, int minute, double second){
        setTime(Time_t(hour, minute, second));
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

    void setGpsQualityIndicator(GpsQualityIndicator gqi){
        fields[GPS_QUALITY_INDICATOR] = toStringInt(gqi, 1);
    }

    void setNumberOfSatellitesInUse(int val){
        fields[NUMBER_OF_SATALLITES_IN_USE] = toStringInt(val, 2);
    }

    void setHorizontalDilution(double val){
        fields[HORIZONTAL_DILUTION] = toStringFloat(val, 2);
    }

    void setAntennaAltitude(double val){
        fields[ANTENNA_ALTITUDE] = toStringFloat(val, 2);
        setUnitsOfAntennaAltitude();
    }

    void setUnitsOfAntennaAltitude(){
        fields[UNITS_OF_ANTENNA_ALTITUDE] = "M";
    }

    void setGeoidalSeparation(double val){
        fields[GEOIDAL_SEPARATION] = toStringFloat(val, 2);
        setUnitsOfGeoidalSeparation();
    }

    void setUnitsOfGeoidalSeparation(){
        fields[UNITS_OF_GEOIDAL_SEPARATION] = "M";
    }

    void setAgeOfDifferentialGpsData(double sec){
        fields[AGE_OF_DIFFERENTIAL_GPS_DATA] = std::to_string(sec);
    }

    void setDifferentialReferenceStation(int val){
        fields[DIFFERENTIAL_REFERENCE_STATION] = toStringInt(val, 4);
    }


    Time_t getTime(){
        return hhmmssToTime(fields[UTC]);
    }

    double getLatitude(){
        if(fields[LATITUDE].empty()){
            return 0;
        }
        return ddmmToLatDegree(std::make_pair(fields[LATITUDE], fields[N_or_S] == "S" ? 'S' : 'N'));
    }

    double getLongitude(){
        if(fields[LONGITUDE].empty()){
            return 0;
        }
        return ddmmToLongDegree(std::make_pair(fields[LONGITUDE], fields[E_or_W] == "W" ? 'W' : 'E'));
    }

    GpsQualityIndicator getGpsQualityIndicator(){
        if(fields[GPS_QUALITY_INDICATOR].empty()){
            return static_cast<GpsQualityIndicator>(0);
        }
        return static_cast<GpsQualityIndicator> (stoi(fields[GPS_QUALITY_INDICATOR]));
    }
    int getNumberOfSatellitesInUse(){
        if(fields[NUMBER_OF_SATALLITES_IN_USE].empty()){
            return 0;
        }
        return stoi(fields[NUMBER_OF_SATALLITES_IN_USE]);
    }
    double getHorizontalDilution(){
        if(fields[HORIZONTAL_DILUTION].empty()){
            return 0;
        }
        return stod(fields[HORIZONTAL_DILUTION]);
    }

    double getAntennaAltitude(){
        if(fields[ANTENNA_ALTITUDE].empty()){
            return 0;
        }
        return stod(fields[ANTENNA_ALTITUDE]);
    }

    double getGeoidalSeparation(){
        if(fields[GEOIDAL_SEPARATION].empty()){
            return 0;
        }
        return stod(fields[GEOIDAL_SEPARATION]);
    }

    double getAgeOfDifferentialGpsData(){
        if(fields[AGE_OF_DIFFERENTIAL_GPS_DATA].empty()){
            return 0;
        }
        return stod(fields[AGE_OF_DIFFERENTIAL_GPS_DATA]);
    }

    int getDifferentialReferenceStation(){
        if(fields[DIFFERENTIAL_REFERENCE_STATION].empty()){
            return 0;
        }
        return stoi(fields[DIFFERENTIAL_REFERENCE_STATION]);
    }
#if 0
    void getUnitsOfAntennaAltitude(){
//        fields[UNITS_OF_ANTENNA_ALTITUDE] = "M";
    }

    void setUnitsOfGeoidalSeparation(){
//        gields[UNITS_OF_GEOIDAL_SEPARATION] = "M";
    }
#endif
};



}
}

Q_DECLARE_METATYPE(nmea::gga::GGA_Sentence)

#endif // GGA_SENTENCE_H
