#ifndef ZDA_SENTENCE_H
#define ZDA_SENTENCE_H

#include "sentencebase.hpp"
#include <QDate>
#include <QString>

namespace nmea{
namespace zda {

enum FieldsEnum{
    UTC,
    DAY,
    MONTH,
    YEAR,
    LOCAL_ZONE_HOURS,
    LOCAL_ZONE_MINUTES,
    __FieldsEnum__MAX
};
/*
NOTE Local time zone is the magnitude of hours plus the
magnitude of minutes added, with the sign of local zone
hours, to local time to obtain UTC. Local zone is generally
negative for East longitudes with local exceptions near the
international date line.

Example:
At Chatham Is. (New Zealand) at 1230 (noon) local time on June 10, 1995:
$GPZDA,234500,09,06,1995,-12,45*6C<CR><LF>
In the Cook Islands at 1500 local time on June 10, 1995:
$GPZDA,013000,11,06,1995,10,30*4A<CR><LF>
 */




class ZDA_Sentence : public SentenceBase
{
public:
    ZDA_Sentence(std::string talkerID = "--", Fields_t fList = {})
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList)
    {
        this->sentenceID = "ZDA";
    }

    void setTime(Time_t time){
        fields[UTC] = timeTo_hhmmss(time);
    }


    Time_t getTime() const{
        return hhmmssToTime(fields[UTC]);
    }

    void setDate(QDate date){
        fields[DAY]     = QString::number(date.day()).toStdString();
        fields[MONTH]   = QString::number(date.month()).toStdString();
        fields[YEAR]    = QString::number(date.year()).toStdString();
    }

    QDate getDate() const{
        auto day = QString::fromStdString(fields[DAY]).toInt();
        auto month = QString::fromStdString(fields[MONTH]).toInt();
        auto year = QString::fromStdString(fields[YEAR]).toInt();

        QDate date;
        date.setDate(year, month, day);
        return date;
    }

    void setZone(int hour, int minute){
        hour = -hour; //SEE NOTE

        fields[LOCAL_ZONE_HOURS]    = hour;
        fields[LOCAL_ZONE_MINUTES]  = minute;
    }

    void getZone(int &hour, int& minute) const{
        hour = QString::fromStdString(fields[LOCAL_ZONE_HOURS]).toInt();
        minute = QString::fromStdString(fields[LOCAL_ZONE_MINUTES]).toInt();

        hour = -hour; //SEE NOTE
    }

};



}
}

Q_DECLARE_METATYPE(nmea::zda::ZDA_Sentence)


#endif // ZDA_SENTENCE_H
