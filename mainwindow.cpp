#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , modvalue(0)
    , isDelete(false)
    , namecounter(0)
{
    ui->setupUi(this);
    QObject::connect(ui->modifyButton, SIGNAL(clicked()), this, SLOT(selectMode()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toConsole(const QString &message)
{
    QLabel *mes = new QLabel(message);
    mes->setMaximumSize(ui->scrollAreaWidgetContents->width() - 50, 30);
    mes->setMinimumSize(ui->scrollAreaWidgetContents->width() - 50, 30);
    ui->verticalLayout->insertWidget(0, mes, 0, Qt::AlignLeft);
}

void MainWindow::modifyFile(qint64 (MainWindow::*opptr)(const qint64& val))
{
    qint64 c = 0;
    qint64 modc = 0;
    char bytemodc = 0;
    QByteArray allbytesmodc;
    if(fileIn.open(QIODevice::ReadOnly) && fileOut.open(QIODevice::WriteOnly))
    {
        while(!fileIn.atEnd()) // read the entire file by byte
        {
            fileIn.read((char*)&c,1);
            modc = (this->*opptr)(c);
            while(modc != 0) // store all bytes of modified char to byte array
            {
                bytemodc |= modc;
                modc >>= 8;
                allbytesmodc.append(bytemodc);
                bytemodc = 0;
            }
            fileOut.write(allbytesmodc);
            allbytesmodc.clear();
        }
        fileIn.close();
        fileOut.close();
        if(isDelete)
        {
            fileIn.remove();
        }
        toConsole("File successfuly modified!");
    } else
    {
        toConsole("Unable to open files!");
    }
}

void MainWindow::selectMode()
{
    if(ui->timerRB->isChecked()) //timer mode
    {
        QTimer *timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(setParams()));
        QTime *time = new QTime(ui->timeEdit->time());
        int timemsecs = 0;
        timemsecs += time->hour()*3600000; // converting time to milliseconds
        timemsecs += time->minute()*60000;
        timemsecs += time->second()*1000;
        timemsecs += time->msec();
        toConsole("File will be modified every " + time->toString() + " h:m:s");
        timer->start(timemsecs);
    } else //one-time mode
    {
        setParams();
    }
}

void MainWindow::setParams()
{
    //    ../TestProject/sometext.txt;
    fileIn.setFileName(ui->inputLE->text());
    QFileInfo inputInfo (fileIn.fileName());
    if(!inputInfo.exists() && !inputInfo.isFile()) //check if path to input file exists and it's not a directory
    {
        toConsole("Input file doesn't exist!");
        return;
    }

    isDelete = ui->deleteCheck->isChecked();

    fileOut.setFileName(ui->outputLE->text());
    if(fileOut.exists() && ui->filenameModeCB->currentIndex() == 1) // if output file exists and we want to increment its index name instead of rewriting
    {
        addFilenameIndex(fileOut);
    }

    QString strvalue = ui->modValueLE->text();
    if(strvalue == "")
    {
        toConsole("Enter the modification value!");
        return;
    }
    modvalue = strvalue.toLongLong();

    setOperation();
}

void MainWindow::addFilenameIndex(QFile &file)
{
    QString filename = file.fileName();
    QString fileindex;
    fileindex.setNum(++namecounter);
    fileindex.insert(0, "(");
    fileindex.append(")");
    int chpos = filename.lastIndexOf('.');
    file.setFileName(filename.insert(chpos, fileindex));
}

void MainWindow::setOperation()
{
    switch (ui->operatorCB->currentIndex()) {
    case 0:
    {
        modifyFile(&MainWindow::opOR);
        break;
    }
    case 1:
    {
        modifyFile(&MainWindow::opAND);
        break;
    }
    case 2:
    {
        modifyFile(&MainWindow::opXOR);
        break;
    }
    case 3:
    {
        modifyFile(&MainWindow::opLeftShift);
        break;
    }
    case 4:
    {
        modifyFile(&MainWindow::opRightShift);
        break;
    default: break;
    }
    }
}

qint64 MainWindow::opOR(const qint64 &val)
{
    return val | modvalue;
}

qint64 MainWindow::opAND(const qint64 &val)
{
    return val & modvalue;
}

qint64 MainWindow::opXOR(const qint64 &val)
{
    return val ^ modvalue;
}

qint64 MainWindow::opLeftShift(const qint64 &val)
{
    return val << modvalue;
}

qint64 MainWindow::opRightShift(const qint64 &val)
{
    return val >> modvalue;
}
