#ifndef CHOOSEANDFINDCONCEPT_H
#define CHOOSEANDFINDCONCEPT_H


#include <QComboBox>
#include <QSortFilterProxyModel>

class ChooseAndFindConcept : public QComboBox
{
    Q_OBJECT

public:
    ChooseAndFindConcept(QWidget* parent = nullptr);

private:
    void search(const QString& text);
    QSortFilterProxyModel* proxyModel;
};

#endif // CHOOSEANDFINDCONCEPT_H
