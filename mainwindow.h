#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QFileInfo>
#include <QComboBox>
#include <QRadioButton>
#include <QScrollArea>
#include <QTimer>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void toConsole(const QString& message);

private:
    Ui::MainWindow *ui;
    QFile fileIn;
    QFile fileOut;
    qint64 modvalue;
    bool isDelete;
    int namecounter;
    void modifyFile(qint64 (MainWindow::*opptr)(const qint64& val));
    void addFilenameIndex(QFile &file);
    void setOperation();
    qint64 opOR(const qint64& val);
    qint64 opAND(const qint64& val);
    qint64 opXOR(const qint64& val);
    qint64 opLeftShift(const qint64& val);
    qint64 opRightShift(const qint64& val);

public slots:
    void selectMode();
    void setParams();
};
#endif // MAINWINDOW_H
