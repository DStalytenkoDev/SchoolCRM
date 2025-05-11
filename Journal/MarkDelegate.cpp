#include "MarkDelegate.h"
#include <QLineEdit>

MarkDelegate::MarkDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QWidget *MarkDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return new QLineEdit(parent);
}

void MarkDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
}

void MarkDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QString text = static_cast<QLineEdit*>(editor)->text();

    emit ((MarkDelegate*)this)->indexEdited(index, text);
}

void MarkDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
