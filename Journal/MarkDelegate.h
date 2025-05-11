#ifndef MARKDELEGATE_H
#define MARKDELEGATE_H


#include <QStyledItemDelegate>


class MarkDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    MarkDelegate(QObject* parent = nullptr);

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

signals:
    void indexEdited(QModelIndex index, QString text);

};

#endif // MARKDELEGATE_H
