#include <nmea.hpp>
#include <nmeaparser.hpp>
#include <gga_sentence.hpp> 
#include <sentencebase.hpp>
#include <QCoreApplication>

#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    nmea::Nmea nmea;


    QObject::connect(&nmea, &nmea::Nmea::ggaReceived, [](nmea::gga::GGA_Sentence s){
        qDebug() << "GGA Sentence received";
        qDebug() << "Time: " << s.getTime();
        qDebug() << "Latitude: " << s.getLatitude();
        qDebug() << "Longitude: " << s.getLongitude();
        qDebug() << "Fix quality: " << s.getGpsQualityIndicator();
        qDebug() << "Number of satellites: " << s.getNumberOfSatellitesInUse();
    });

    nmea.parse("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n");

    return a.exec();
}
