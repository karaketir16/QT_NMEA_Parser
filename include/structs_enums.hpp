#ifndef STRUCTS_ENUMS_H
#define STRUCTS_ENUMS_H

#include <QMap>
#include <QPair>
#include <stdint.h>
#include <QVariant>
#include <QList>
#include <QTime>
#include <QMutex>
#include <QString>
#include <QTextStream>

#pragma pack(push, 1)


struct Position {
    double      lat;
    double     lon;
    QString toString(){
        QString s;
        QTextStream ss(&s);
        ss << " Lat: " << lat;
        ss << " Lon: " << lon;
        return s;
    }
};
Q_DECLARE_METATYPE(Position)

struct RegisterPosition{
    RegisterPosition(){
        qRegisterMetaType<Position>();
    }
};
static RegisterPosition registerPosition;


struct Attitude {
    double      heading;
    double      roll;
    double      pitch;
    QString toString(){
        QString s;
        QTextStream ss(&s);
        ss << " Heading: " << heading;
        ss << " roll: " << roll;
        ss << " pitch: " << pitch;
        return s;
    }
};
Q_DECLARE_METATYPE(Attitude)

struct Speed {
    void setSpeedMeterSecond(double speed){
        setSpeedKiloMeterHour(speed * 3.6);
    }
    void setSpeedKiloMeterHour(double speed){
        speedInKnot = speed / 1.852;
    }
    void setSpeedKnots(double speed){
        speedInKnot = speed;
    }

    double getSpeedKnots(){
        return speedInKnot;
    }

    double getSpeedMeterSecond(){
        return getSpeedKiloMeterHour() / 3.6;
    }
    double getSpeedKiloMeterHour(){
        return  speedInKnot * 1.852;
    }

    operator double(){
        return speedInKnot;
    }

    Speed(double knots = 0){
        speedInKnot = knots;
    }

private:
    double speedInKnot = 0;

};

Q_DECLARE_METATYPE(Speed)

struct WindData
{
    double angle;
    Speed speed;
    QString toString(){
        QString s;
        QTextStream ss(&s);
        ss << " Speed(knots): " << speed.getSpeedKnots();
        ss << " Angle: " << angle;
        return s;
    }
};

Q_DECLARE_METATYPE(WindData)


typedef Speed WaterSpeed;
typedef Speed GroundSpeed;

typedef double Heading;
typedef double Depth;

Q_DECLARE_METATYPE(double)

#pragma pack(pop)


#endif // STRUCTS_ENUMS_H
