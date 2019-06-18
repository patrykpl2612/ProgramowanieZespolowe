#include "comboboxitemdelegate.h"
#include <QComboBox>

ComboBoxItemDelegate::ComboBoxItemDelegate(int typ, QObject *parent)
    : QStyledItemDelegate(parent)
{
    this->typ = typ;
}


ComboBoxItemDelegate::~ComboBoxItemDelegate()
{
}


QWidget *ComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Create the combobox and populate it
    QComboBox *cb = new QComboBox(parent);
    //const int row = index.row();
    if(typ == 0)  //stopień
    {
        cb->addItem(QString("mgr"));
        cb->addItem(QString("dr"));
        cb->addItem(QString("dr hab."));
        cb->addItem(QString("prof. dr hab."));
    }
    else if(typ == 1)
    {
        cb->addItem(QString("nauczyciel - UMK"));
        cb->addItem(QString("inny - UMK"));
        cb->addItem(QString("doktorant"));
        cb->addItem(QString("spoza UMK"));
    }

    return cb;
}


void ComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    // get the index of the text in the combobox that matches the current value of the item
    const QString currentText = index.data(Qt::EditRole).toString();
    const int cbIndex = cb->findText(currentText);
    // if it is valid, adjust the combobox
    if (cbIndex >= 0)
       cb->setCurrentIndex(cbIndex);
}


void ComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->currentText(), Qt::EditRole);
}
