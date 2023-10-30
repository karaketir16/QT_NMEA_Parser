#ifndef DPT_SENTENCE_H
#define DPT_SENTENCE_H

#include "sentencebase.hpp"


namespace nmea{
namespace dpt {

enum FieldsEnum{
    WATER_DEPTH_RELATIVE,
    OFFSET_FROM_TRANSDUCER,
    MAX_RANGE_SCALE_IN_USE,
    __FieldsEnum__MAX
};

class DPT_Sentence : public SentenceBase
{
public:
    DPT_Sentence(std::string talkerID = "--", Fields_t fList = {})
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList) 
    {
        this->sentenceID = "DPT";
    }

    void setDepth(double depth){
        fields[WATER_DEPTH_RELATIVE] = toStringFloat(depth, 3);
    }
    void setOffset(double offset){
        fields[OFFSET_FROM_TRANSDUCER] = toStringFloat(offset, 3);
    }

    void setMax(double mx){
        fields[MAX_RANGE_SCALE_IN_USE] = toStringFloat(mx, 3);
    }

    double getDepth() const{
        double val = 0;

        if(! fields[WATER_DEPTH_RELATIVE].empty()){
            val = QString::fromStdString(fields[WATER_DEPTH_RELATIVE]).toDouble();
        }

        return val;
    }
    double getOffset() const{
        double val = 0;

        if(! fields[OFFSET_FROM_TRANSDUCER].empty()){
            val = QString::fromStdString(fields[OFFSET_FROM_TRANSDUCER]).toDouble();
        }

        return val;
    }

    double getMax() const{
        double val = 0;

        if(! fields[MAX_RANGE_SCALE_IN_USE].empty()){
            val = QString::fromStdString(fields[MAX_RANGE_SCALE_IN_USE]).toDouble();
        }

        return val;
    }

};


}
}

Q_DECLARE_METATYPE(nmea::dpt::DPT_Sentence)

#endif // DPT_SENTENCE_H
