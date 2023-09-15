#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include <QAbstractTableModel>
extern "C" {
#include "src/library/library.h"
}

class mainTableModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit mainTableModel(QObject *parent = nullptr);
  ~mainTableModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  void loadBooks(Book **data);

private:
  Book *data_;
};

#endif // !MAINTABLEMODEL_H
