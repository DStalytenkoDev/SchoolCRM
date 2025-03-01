#include "MarkTypeSelector.h"

MarkTypeSelector::MarkTypeSelector(QWidget* parent) : QComboBox(parent)
{
    this->addItems({
        "AlphaTest",
        "BetaTest",
        "GamaTest",
        "PracticeLesson",
        "LaboratoryLesson",
        "IrregularMark"
    });

    this->setType(dbapi::StudentMark::IrregularMark);

    connect(this, &MarkTypeSelector::currentIndexChanged, this, &MarkTypeSelector::indexChanging);
}

void MarkTypeSelector::setType(dbapi::StudentMark::Type type)
{
    this->setCurrentIndex(3);
}

dbapi::StudentMark::Type MarkTypeSelector::getType()
{
    return static_cast<dbapi::StudentMark::Type>(this->currentIndex() % 5);
}

void MarkTypeSelector::indexChanging()
{
    emit this->typeChanged(this->getType());
}
