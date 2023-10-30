#ifndef HDT_SENTENCE_H
#define HDT_SENTENCE_H

#include "sentencebase.hpp"


namespace nmea{
namespace hdt {

enum FieldsEnum{
	HEADING_DEGREES,
	T_Field,
    __FieldsEnum__MAX
};

class HDT_Sentence : public SentenceBase
{
public:
    HDT_Sentence(std::string talkerID = "--", Fields_t fList = {})
                : SentenceBase(__FieldsEnum__MAX, talkerID, fList) 
    {
        this->sentenceID = "HDT";
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

};



}
}

Q_DECLARE_METATYPE(nmea::hdt::HDT_Sentence)


#endif // HDT_SENTENCE_H
