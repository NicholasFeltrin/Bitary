#include <QAbstractTableModel>
#include "ui_window.h"
#include "window.h"
extern "C"{
#include "src/library/library.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CustomTableModel(QObject *parent = nullptr);
    
    // Override these methods to implement your custom model
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    // Implement a function to fetch more data from the database
    void fetchMore();
    
private:
    // Add member variables to store your data
    QList<Book> data_;
};

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return data_.size();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // Assuming you have 4 columns in your view
    return 4;
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= data_.size())
        return QVariant();

    const Book& book = data_[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return book.bookID;
            case 1: return book.libraryID;
            case 2: return book.title;
            case 3: return book.borrowID;
            default: return QVariant();
        }
    }

    return QVariant();
}

#include "window.moc"
