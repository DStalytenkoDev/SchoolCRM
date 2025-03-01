#include "ChooseAndFindConcept.h"
#include <QCompleter>


ChooseAndFindConcept::ChooseAndFindConcept(QWidget *parent) : QComboBox(parent)
{
    this->setEditable(true);
    this->setInsertPolicy(NoInsert);

    connect(this, &ChooseAndFindConcept::editTextChanged, this, &ChooseAndFindConcept::search);

    this->proxyModel = new QSortFilterProxyModel(this);
    this->proxyModel->setSourceModel(this->model());
    this->proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->proxyModel->setFilterKeyColumn(this->modelColumn());

    auto completer = new QCompleter(this);
    completer->setModel(this->proxyModel);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    this->setCompleter(completer);
}

void ChooseAndFindConcept::search(const QString& text)
{
    this->proxyModel->setFilterFixedString(text);
}
