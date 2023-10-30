#ifndef SENTENCEBASE_HPP
#define SENTENCEBASE_HPP

#include <QString>
#include <QStringList>
#include <string>
#include <vector>
#include <utility>
#include <QTime>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <QTextStream>

#include <QMetaType>

#define NORTH 'N'
#define SOUTH 'S'
#define EAST 'E'
#define WEST 'W'

typedef std::vector<std::string> Fields_t;

namespace nmea
{

    enum GpsQualityIndicator
    {
        FIX_NOT_AVAILABLE = 0,
        GPS_FIX,
        DIFFERENTIAL_GPS_FIX,
        PPS_FIX,
        REAL_TIME_KINEMATIC,
        FLOAT_RTK,
        ESTIMATED,
        MANUAL_INPUT_MODE,
        SIMULATION_MODE,
    };

    struct Time_t
    {
        int hour;
        int minute;
        double second;
        Time_t(int hour = 0, int minute = 0, double second = 0)
            : hour(hour), minute(minute), second(second) {}

        Time_t(const QTime &t)
        {
            hour = t.hour();
            minute = t.minute();
            second = t.second();
            second += t.msec() / 1000.0;
        }

        operator QTime()
        {
            int s = second;
            second -= s;
            int ms = second * 1000;
            return QTime(hour, minute, s, ms);
        }
    };

    class SentenceBase
    {
    public:
        std::string talkerID;
        std::string sentenceID;
        Fields_t fields;

        SentenceBase(unsigned num = 0, std::string talkerID = "--", Fields_t fList = {})
        {

            if (fList.size() == 0)
            {
                fields = Fields_t(num);
            }
            else
            {
                fields = fList;
            }
            this->talkerID = talkerID;
        }

        // https://stackoverflow.com/a/14266139
        static Fields_t split(std::string sentence, std::string delimiter = ",")
        {
            Fields_t res;

            size_t pos = 0;
            std::string token;
            while ((pos = sentence.find(delimiter)) != std::string::npos)
            {
                token = sentence.substr(0, pos);
                res.push_back(token);
                sentence.erase(0, pos + delimiter.length());
            }
            res.push_back(sentence);

            return res;
        }

        std::string toStringInt(int i, int len)
        {
            std::stringstream ss;
            if (i < 0)
            {
                ss << '-';
                i = -i;
            }
            ss << std::fixed << std::setw(len) << std::setfill('0') << i;
            return ss.str();
        }

        int intLen(int x)
        {
            int a = 0;
            do
            {
                a++;
                x /= 10;
            } while (x);
            return a;
        }

        std::string toStringFloat(double val, int precision)
        {
            std::stringstream ss;
            int width = intLen(val) + precision + 1;
            if (val < 0)
            {
                ss << '-';
                val = -val;
            }

            ss << std::fixed << std::setw(width) << std::setfill('0') << std::setprecision(precision) << val;
            return ss.str();
        }

        std::string toStringFloat(double val, int precisionInt, int precisionFrac)
        {
            std::stringstream ss;
            int width = precisionInt + precisionFrac + 1; // dot
            if (val < 0)
            {
                ss << '-';
                val = -val;
            }

            ss << std::fixed << std::setw(width) << std::setfill('0') << std::setprecision(precisionFrac) << val;
            return ss.str();
        }

        std::string toStringFloatSigned(double val, int precisionInt, int precisionFrac)
        {
            std::stringstream ss;
            int width = precisionInt + precisionFrac + 1; // dot
            if (val < 0)
            {
                ss << '-';
                val = -val;
            }
            else
            {
                ss << '+';
            }

            ss << std::fixed << std::setw(width) << std::setfill('0') << std::setprecision(precisionFrac) << val;
            return ss.str();
        }

        std::string timeTo_hhmmss(Time_t time)
        {
            char tmp[50];
            sprintf(tmp, "%02d%02d%05.2lf", time.hour, time.minute, time.second);
            std::stringstream ss;
            ss << tmp;
            return ss.str();
        }

        Time_t hhmmssToTime(std::string hhmmss) const
        {
            Time_t res;
            if (hhmmss.empty())
            {
                res.hour = 0;
                res.minute = 0;
                res.second = 0;
                return res;
            }
            sscanf(hhmmss.c_str(), "%02d%02d%lf", &res.hour, &res.minute, &res.second);
            return res;
        }

        std::pair<std::string, char> latDegreeTo_ddmm(double degree)
        {
            int dd = degree;
            degree -= dd;
            char res = NORTH;
            if ((dd + degree) < 0)
            {
                dd = -dd;
                degree = -degree;
                res = SOUTH;
            }
            double minute = degree * 60;
            char tmp[50];
            sprintf(tmp, "%02d%08.5lf", dd, minute);
            std::stringstream ss;
            ss << tmp;
            return std::make_pair(ss.str(), res);
        }

        double ddmmToLatDegree(std::pair<std::string, char> in) const
        {
            double degree = 0;
            int dd = 0;
            double minute = 0;
            sscanf(in.first.c_str(), "%02d%lf", &dd, &minute);
            degree = dd + minute / 60;
            return (in.second == NORTH ? 1 : -1) * degree;
        }

        std::pair<std::string, char> longDegreeTo_ddmm(double degree)
        {
            int dd = degree;
            degree -= dd;
            char res = EAST;
            if ((dd + degree) < 0)
            {
                dd = -dd;
                degree = -degree;
                res = WEST;
            }
            double minute = degree * 60;
            char tmp[50];
            sprintf(tmp, "%03d%08.5lf", dd, minute);
            std::stringstream ss;
            ss << tmp;
            return std::make_pair(ss.str(), res);
        }

        std::string join(const Fields_t &v, const std::string delimiter)
        {
            std::stringstream ss;
            unsigned i = 0;
            for (; i + 1 < v.size(); i++)
            {
                ss << v[i] << delimiter;
            }
            ss << v[i];
            return ss.str();
        }

        double ddmmToLongDegree(std::pair<std::string, char> in) const
        {
            double degree = 0;
            int dd = 0;
            double minute = 0;
            sscanf(in.first.c_str(), "%03d%lf", &dd, &minute);
            degree = dd + minute / 60;
            return (in.second == EAST ? 1 : -1) * degree;
        }

        std::string generateSentence(bool addChecksum)
        {
            std::stringstream res;
            res << "$";
            std::stringstream checksumIncluded;
            checksumIncluded << this->talkerID;
            checksumIncluded << this->sentenceID;
            checksumIncluded << "," << join(fields, ",");
            unsigned checksum = 0;
            for (auto &c : checksumIncluded.str())
            {
                checksum ^= c;
            }
            res << checksumIncluded.str();
            if (addChecksum)
            {
                res << "*" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << checksum;
            }
            res << "\r\n";
            return res.str();
        }

        QString generateSentenceQString(bool addChecksum)
        {
            return QString::fromStdString(generateSentence(addChecksum));
        }
    };

}
#endif // SENTENCEBASE_HPP
