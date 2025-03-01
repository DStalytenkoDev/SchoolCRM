#ifndef FUZZYSEARCHTOOLS_H
#define FUZZYSEARCHTOOLS_H


#include <QString>


namespace FuzzySearchTools
{

inline int plainComputerOfDistance(const QString& aString, const QString& bString, int shift = 0) noexcept;
inline int computeDistance(const QString& aString, const QString& bString) noexcept;

}


inline int FuzzySearchTools::plainComputerOfDistance(const QString& aString, const QString& bString, int shift) noexcept
{
    int d = 0;

    for (int index = 0; index < bString.size(); ++index)
    {
        if(aString[index + shift] != bString[index])
        {
            if(aString[index + shift].toLower() == bString[index].toLower())
                d++;
            else
                d += 2;
        }
    }

    return d;
}

inline int FuzzySearchTools::computeDistance(const QString& aString, const QString& bString) noexcept
{
    if(aString.size() == bString.size())
        return FuzzySearchTools::plainComputerOfDistance(aString, bString);

    int d = 0;
    int shift = 0;

    auto p_aString = &aString;
    auto p_bString = &bString;

    if(aString.size() < bString.size())
    {
        p_aString = &bString;
        p_bString = &aString;
    }

    while (p_bString->size() + shift < p_aString->size())
    {
        d += FuzzySearchTools::plainComputerOfDistance(*p_aString, *p_bString, shift);
        shift++;
    }

    return d;
}

#endif // FUZZYSEARCHTOOLS_H
