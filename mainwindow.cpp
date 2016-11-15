#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startdialog.h"
#include <QMessageBox>
#include <QSqlError>
#include <QIcon>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QModelIndex>
#include "ImportTXTDialog.h"
#include <QFileInfo>
#include <QTime>
#include <QTimer>
#include <QMutex>
#include <QDebug>
#include "pathmanage.h"
#include "StandardDialog.h"
#include "ExportExcelDialog.h"
#include "ExportExcel.h"
#include "AutoFillingUnitDialog.h"
#include "AutomaticFillingUnit.h"
////////////////////////////////////////////////////
//开始
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //加载程序启动画面：
//    QSplashScreen *splash = new QSplashScreen;
//    splash->setPixmap(QPixmap("./rc/start.jpg"));
//    splash->show();
//    Qt::Alignment topRight = Qt::AlignCenter;//Qt::AlignRight|Qt::AlignTop;
//    splash->showMessage(QObject::tr("正在启动程序..."), topRight,Qt::red);
//    QWaitCondition wait;
//    QMutex *mutex = new QMutex();
//    mutex->lock();
//    wait.wait(mutex,1000);

    ///////////////////////////////////////////////////////////////////////////////
    //
    ui->InfoTextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    ui->goTolineEdit->setDisabled(true);
    ////////////////////////////////////////////////////////////////
    //创立路径
    PathManage::createRelExeDir("/WortschatzEditor/importtxt/");
    PathManage::createRelExeDir("/WortschatzEditor/importexcel/");
    PathManage::createRelExeDir("/WortschatzEditor/exportexcel/");

    ///////////////////////////////////////////////////
    //
    wordListDB = NULL;

    QDateTime currtime = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = currtime.toString("yyyy-MM-dd dddd hh:mm:ss  "); //设置显示格式
    ui->timeLabel->setText(str);//在标签上显示时间

    //定时器，每1000ms执行一次槽函数displayTime()
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(displayTime()));
    ///////////////////////////////////////////////////////////////
    //启动选择
    ui->statusBar->showMessage(tr("欢迎您使用德语背单词软件词库编辑器！ -- ansleliu"));
    QTimer::singleShot(200, this, SLOT(loadBook()));
/*
    if(model->rowCount() > 0)
    {
        ui->action_import->setEnabled(false);
    }
*/
//    splash->finish(this);//在主窗口加载完成后释放启动画面资源
//    delete splash;
//    mutex->unlock();
}

MainWindow::~MainWindow()
{
    model = NULL;

    if(wordListDB != NULL)
    {
        delete wordListDB;
        wordListDB = NULL;
    }

    wordList = NULL;

    delete ui;
}

void MainWindow::loadBook()
{
    startDialog *startwin;
    startwin = new startDialog(NULL);
    QString DaNa;
    QString TaNa;
    if(startwin->exec() == QDialog::Accepted)
    {
        DaNa = startwin->getDatabaseName();
        TaNa = startwin->getTableName();

        startwin->close();
        delete startwin;
        startwin = NULL;

        QFileInfo fd(DaNa);
        QString DatabaseName=fd.baseName();
        this->setWindowTitle(QString("德语背单词软件词库编辑器 -- 当前词库：%1").arg(DatabaseName));
        this->setWindowIcon(QIcon("./rc/edit.ico"));

        databaseName = DaNa;
        tableName = "WordsBook";

        if(wordListDB != NULL)
        {
            delete wordListDB;
            wordListDB = NULL;
        }
        wordListDB = new CreatWordListDB(databaseName,"DATABASE",QLatin1String("QSQLITE"));
        qDebug() << QString("一个新的数据库：%1 - %2建立").arg(databaseName).arg("DATABASE");

        model = wordListDB->wordListDBModel(this,tableName);
        ui->tableView->setModel(model);


        asIdSelect = "FALSE";//按id查询关
        asWortSelect = "FALSE";//按wort查询关
        descendingOrder = "FALSE";//按降序关
        ui->action_import->setEnabled(true);
        ui->action_export->setEnabled(true);
        ui->action_Standard->setEnabled(true);
        ui->action_TXT->setEnabled(true);
        ui->deleteRecordButton->setEnabled(true);
        ui->newRecordButton->setEnabled(true);
        ui->showAllButton->setEnabled(true);
        ui->descendingOrderButton->setEnabled(true);
        ui->ascendingOrderButton->setEnabled(true);
        ui->idSelectradioButton->setEnabled(true);
        ui->wortSelectradioButton->setEnabled(true);
        ui->action_translation->setEnabled(true);
        ui->action_export->setEnabled(true);

        wordid = 0;
        ui->InfoTextBrowser->setHidden(true);
        ui->ProgressBar->setHidden(true);

        ui->goTolineEdit->setDisabled(true);
    }
    else
    {
        startwin->close();
        delete startwin;
        startwin = NULL;
        on_action_quit_triggered();
    }

    if(wordListDB == NULL)
    {
        ui->action_import->setEnabled(false);
        ui->action_export->setEnabled(false);
        ui->action_Standard->setEnabled(false);
        ui->action_TXT->setEnabled(false);
        ui->deleteRecordButton->setEnabled(false);
        ui->newRecordButton->setEnabled(false);
        ui->showAllButton->setEnabled(false);
        ui->descendingOrderButton->setEnabled(false);
        ui->ascendingOrderButton->setEnabled(false);
        ui->goTolineEdit->setEnabled(false);
        ui->idSelectradioButton->setEnabled(false);
        ui->wortSelectradioButton->setEnabled(false);
        ui->action_translation->setEnabled(false);
        ui->action_export->setEnabled(false);
    }
}

void MainWindow::reloadBook()
{
    loadBook();
}

void MainWindow::on_action_change_triggered()
{
    reloadBook();
}

void MainWindow::displayTime(void)
{
    QDateTime currtime = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = currtime.toString("yyyy-MM-dd dddd hh:mm:ss "); //设置显示格式
    ui->timeLabel->setText(str);//在标签上显示时间
}

void MainWindow::saveRecord()
{
    model->submitAll();
}

void MainWindow::threadAddNewWord()
{
    model->select();
    wordid++;
    QString text = QString("导入第 %1 个词条成功").arg(wordid);
    if(wordid%200 == 0)
    {
        ui->InfoTextBrowser->clear();
    }
    ui->InfoTextBrowser->append(text);
    ui->ProgressBar->setValue(wordid);
}

void MainWindow::transformEnd()
{
    ui->InfoTextBrowser->append(QString("转换结束"));
    ui->InfoTextBrowser->clear();
    ui->InfoTextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    wordid = 0;

    ui->action_import->setEnabled(true);
    ui->action_quit->setEnabled(true);
    ui->action_Standard->setEnabled(true);
    ui->action_TXT->setEnabled(true);
    ui->deleteRecordButton->setEnabled(true);
    ui->newRecordButton->setEnabled(true);
    ui->action_translation->setEnabled(true);
    ui->action_export->setEnabled(true);
}

void MainWindow::threadStandardNewRecord()
{
    model->select();
    wordid++;
    QString text = QString("规范化第 %1 个词条成功").arg(wordid);
    if(wordid%200 == 0)
    {
        ui->InfoTextBrowser->clear();
    }
    ui->InfoTextBrowser->append(text);
    ui->ProgressBar->setValue(wordid);
}

void MainWindow::threadStandardEnd()
{
    ui->InfoTextBrowser->append(QString("规范化结束"));
    ui->InfoTextBrowser->clear();
    ui->InfoTextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    wordid = 0;
    /////////////////////////////////////////
    //
    ui->action_import->setEnabled(true);
    ui->action_quit->setEnabled(true);
    ui->action_Standard->setEnabled(true);
    ui->action_TXT->setEnabled(true);
    ui->deleteRecordButton->setEnabled(true);
    ui->newRecordButton->setEnabled(true);
    ui->action_translation->setEnabled(true);
    ui->action_export->setEnabled(true);
}

void MainWindow::threadAutoFillingNewRecord()
{
    model->select();
    wordid++;
    QString text = QString("为第 %1 个词条写入单元信息").arg(wordid);
    if(wordid%200 == 0)
    {
        ui->InfoTextBrowser->clear();
    }
    ui->InfoTextBrowser->append(text);
    ui->ProgressBar->setValue(wordid);
}

void MainWindow::threadAutoFillingEnd()
{
    ui->InfoTextBrowser->append(QString("自动写入单元信息结束"));
    ui->InfoTextBrowser->clear();
    ui->InfoTextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    wordid = 0;
    /////////////////////////////////////////
    //
    ui->action_import->setEnabled(true);
    ui->action_quit->setEnabled(true);
    ui->action_Standard->setEnabled(true);
    ui->action_TXT->setEnabled(true);
    ui->deleteRecordButton->setEnabled(true);
    ui->newRecordButton->setEnabled(true);
    ui->action_export->setEnabled(true);
    ui->action_change->setEnabled(true);
    ui->action_AutoLektion->setEnabled(true);
    ui->action_translation->setEnabled(true);
}

void MainWindow::threadCreatWordListEnded()
{
    ui->InfoTextBrowser->append(QString("创建单词列表结束"));
    ui->InfoTextBrowser->clear();
    ui->InfoTextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    wordid = 0;

    DownloadGoogleDialog * download;
    download = new DownloadGoogleDialog(M_WordList,this);
    if(download->exec() == QDialog::Accepted)
    {
        int startID = download->getStartID();
        int endID= download->getEndID();

        DownloadProcess * downloadPro;
        downloadPro = new DownloadProcess(startID,endID,M_WordList,this);
        wordid = startID;
        ui->ProgressBar->setRange(startID,endID);

        QEventLoop loop;
        connect(downloadPro,SIGNAL(downloadEnded()),
                &loop,SLOT(quit()));
        connect(downloadPro,SIGNAL(downloadNew()),
                this,SLOT(threadDownloadNewRecord()));
        connect(downloadPro,SIGNAL(downloadEnded()),
                this,SLOT(threadDownloadEnded()));
        connect(downloadPro,SIGNAL(NetWorkUnavailable()),
                this,SLOT(networkunable()));

        ui->ProgressBar->setHidden(false);
        ui->InfoTextBrowser->setHidden(false);

        downloadPro->start();
        loop.exec();
    }
    download->close();
    download->deleteLater();
    download = NULL;
}

void MainWindow::threadAddNewRecord()
{
    model->select();
    wordid++;
    QString text = QString("单词列表第 %1 个词条添加成功").arg(wordid);
    if(wordid%200 == 0)
    {
        ui->InfoTextBrowser->clear();
    }
    ui->InfoTextBrowser->append(text);
    ui->ProgressBar->setValue(wordid);
}

void MainWindow::threadDownloadEnded()
{
    ui->InfoTextBrowser->append(QString("下载结束"));
    ui->InfoTextBrowser->clear();
    ui->InfoTextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    wordid = 0;
    //////////////////////////////////////
    //
    ui->action_import->setEnabled(true);
    ui->action_quit->setEnabled(true);
    ui->action_Standard->setEnabled(true);
    ui->action_TXT->setEnabled(true);
    ui->deleteRecordButton->setEnabled(true);
    ui->newRecordButton->setEnabled(true);
    ui->action_translation->setEnabled(true);
    ui->action_export->setEnabled(true);
}

void MainWindow::threadDownloadNewRecord()
{
    model->select();
    wordid++;
    QString text = QString("单词列表第 %1 个词条下载成功").arg(wordid);
    if(wordid%200 == 0)
    {
        ui->InfoTextBrowser->clear();
    }
    ui->InfoTextBrowser->append(text);
    ui->ProgressBar->setValue(wordid);
}

void MainWindow::networkunable()
{
    ui->InfoTextBrowser->append(QString("网络有错误"));
    ui->InfoTextBrowser->clear();
    ui->InfoTextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    wordid = 0;
}

void MainWindow::on_newRecordButton_clicked()
{
    int rowNum = wordListDB->getRowCount();
    qDebug() << rowNum;

    AddNewRecordDialog *newRecord;
    newRecord = new AddNewRecordDialog(this,rowNum);

    if(newRecord->exec() == QDialog::Accepted)
    {
/*
        model->insertRow(rowNum); //添加一行
        model->setData(model->index(rowNum,0),rowNum+1);//主键id自增
        model->setData(model->index(rowNum,1),newRecord->getWord());
        model->setData(model->index(rowNum,2),newRecord->getMean());
        model->setData(model->index(rowNum,3),newRecord->getOther());
        model->setData(model->index(rowNum,4),newRecord->getLektion());
        model->submitAll(); //直接提交
*/
        wordListDB->addNewRecord(newRecord->getWord(),
                                 newRecord->getMean(),
                                 newRecord->getOther(),
                                 newRecord->getLektion());
        model->select();

        newRecord->close();
        delete newRecord;
        newRecord = NULL;
    }
    else
    {
        delete newRecord;
        newRecord = NULL;
    }
}

void MainWindow::on_deleteRecordButton_clicked()
{
    int rowNum = wordListDB->getRowCount();
    qDebug() << rowNum;
    //删除该行
//    model->removeRow(rowNum-1);
    int ok = QMessageBox::warning(this,tr("删除最后一行"),
                                  tr("选择所要删除的行后再进行这个操作！"
                                      "\n         "
                                      "你确定删除第 %1 行吗？").arg(rowNum),
                                  QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
       model->select(); //如果不删除，则撤销
    }
    else
    {
        wordListDB->removeRecord(rowNum); //否则提交，在数据库中删除该行
        model->select();
    }
}

void MainWindow::on_showAllButton_clicked()
{
    descendingOrder = "FALSE";

    model->setTable(tableName);//重新关联表
    model->select();   //显示整个表的内容

    ui->newRecordButton->setDisabled(false);
    ui->deleteRecordButton->setEnabled(true);
}

void MainWindow::on_ascendingOrderButton_clicked()
{
    descendingOrder = "FALSE";

    model->setSort(0, Qt::AscendingOrder); //按Wid属性即第0列升序排列
    model->select();
}

void MainWindow::on_descendingOrderButton_clicked()
{
    descendingOrder = "TRUE";
    model->setSort(0, Qt::DescendingOrder);  //按Wid属性即第0列降序排列
    model->select();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    editorDialog *dialog;
    dialog = new editorDialog(this);

    QString text;
    text = index.data().toString();
    dialog->setText(text);

    dialog->exec();
    text = dialog->getText();

    if(dialog->applyChange == "TRUE")
    {
        model->setData(index,text);
        model->submitAll();
    }
    else
    {
        model->submitAll();
    }
    dialog->close();
    delete dialog;
    dialog = NULL;
}

void MainWindow::on_idSelectradioButton_clicked()
{
    asWortSelect = "FALSE";//按wort查询关
    asIdSelect = "TRUE";//按id查询开

    ui->goTolineEdit->setDisabled(false);
    ui->deleteRecordButton->setDisabled(true);
    ui->newRecordButton->setDisabled(true);
}

void MainWindow::on_wortSelectradioButton_clicked()
{
    asIdSelect = "FALSE";//按id查询关
    asWortSelect = "TRUE";//按wort查询开

    ui->goTolineEdit->setDisabled(false);
    ui->deleteRecordButton->setDisabled(true);
    ui->newRecordButton->setDisabled(true);
}

void MainWindow::on_action_about_triggered()
{
    aboutDialog *aboutD;
    aboutD = new aboutDialog(this);
    if(aboutD->exec() == QDialog::Rejected)
    {

    }
    aboutD->close();
    delete aboutD;
    aboutD = NULL;
}

void MainWindow::on_action_import_triggered()
{
    if(model->rowCount() > 0)
    {
        QMessageBox::warning(this, tr("导入Excel文件"),
                             tr("错误提示: \n词库内容不为空,无法导入!\n请您清空词库内容或者新建词库再执行此操作!"));
    }
    else
    {
        ImportExcelDialog *import;
        import = new ImportExcelDialog(this);
        if(import->exec() == QDialog::Accepted)
        {
            QString excelpath = import->getExcelPath();
            import->close();
            delete import;
            import = NULL;
            //////////////////////////////////////////////////////////////////////////////////
            //
/*
            QSqlDatabase *excel = new QSqlDatabase(QSqlDatabase::addDatabase("QODBC","EXCEL"));
            QString excelname = QString("DRIVER={Microsoft Excel Driver (*.xls)};"
                                        "DBQ=%1;DefaultDir=./WortschatzEditor/importexcel/").arg(excelpath);

            excel->setDatabaseName(excelname);
            if(!excel->open())
            {
                QMessageBox::warning(this, tr("导入Excel文件"),
                                             tr("数据库错误: %1")
                                             .arg(excel->lastError().text()));
            }
*/
//            else
            QString excelname = QString("DRIVER={Microsoft Excel Driver (*.xls)};"
                                        "DBQ=%1;DefaultDir=./WortschatzEditor/importexcel/").arg(excelpath);
            ImportExcel *excel;
            excel = new ImportExcel(excelname,"EXCEL","QODBC");
            {
                QSqlQueryModel *querymodel;
                querymodel = new QSqlQueryModel(this);

                QString sql = QString("select count(*) from [Sheet1$]");
                QSqlQuery query(sql,*excel->getdb());
                query.next();
                int rowCount = query.value(0).toUInt();

                QSqlQuery myquery(QString("select * from [Sheet1$]"),*excel->getdb());
                querymodel->setQuery(myquery);
                //////////////////////////////////////////////////////////////
                //
                on_descendingOrderButton_clicked();
                QMutex *mutex = new QMutex();
                mutex->lock();
                QProgressDialog *progressbar;
                progressbar = new QProgressDialog(this);
                progressbar->setWindowTitle("导入进度");
                progressbar->setCancelButton(0);
                progressbar->setRange(0,rowCount);
                progressbar->setAutoClose(true);
                ////////////////////////////////////////////
                //
                ui->action_import->setEnabled(false);
                ui->action_quit->setEnabled(false);
                ui->action_Standard->setEnabled(false);
                ui->action_TXT->setEnabled(false);
                ui->deleteRecordButton->setEnabled(false);
                ui->newRecordButton->setEnabled(false);
                ui->action_translation->setEnabled(false);
                ui->action_export->setEnabled(false);
                for(int i = 0; i < rowCount; i++)
                {
                    /////////////////////////////////////////////
                    //方法一
                    QSqlRecord record = querymodel->record(i);
                  /////////////////////////////////////////////
                    //方法二
//                    uint wid = record.value(0).toUInt();
                    QString wort = record.value(1).toString();
                    QString mean = record.value(2).toString();
                    QString connection = record.value(3).toString();
                    uint lektion = record.value(4).toUInt();
//                    if(lektion == 0)
//                    {
//                        lektion = (i+1)/200 + 1;
//                    }


                    wordListDB->addNewRecord(wort,mean,connection,lektion);
                    model->select();

                    QString text = QString("正在导入第 %1 个词条，请等待……").arg(i+1);
                    progressbar->setLabelText(text);
                    progressbar->setModal(true);
                    progressbar->show();
                    progressbar->setValue(i+1);
                }
                /////////////////////////////////////////
                //
                ui->action_import->setEnabled(true);
                ui->action_quit->setEnabled(true);
                ui->action_Standard->setEnabled(true);
                ui->action_TXT->setEnabled(true);
                ui->deleteRecordButton->setEnabled(true);
                ui->newRecordButton->setEnabled(true);
                ui->action_translation->setEnabled(true);
                ui->action_export->setEnabled(true);
                /////////////////////////////////////
                //
                on_showAllButton_clicked();

                //////////////////////////////
                //
                mutex->unlock();
                delete mutex;
                mutex = NULL;

                ///////////////////////////
                //
                querymodel->clear();
                delete querymodel;
                querymodel = NULL;

                delete excel;
                excel = NULL;
            }
        }
        else
        {
            import->close();
            delete import;
            import = NULL;
        }
    }
}

void MainWindow::on_action_export_triggered()
{
    if(model->rowCount() < 0)
    {
        QMessageBox::warning(this, tr("导出Excel文件"),
                             tr("错误提示: \n词库内容为空,无法导出!"));
    }
    else
    {
        QFileInfo fileInfo(databaseName);
        QString filename = fileInfo.baseName();
        QString excelpath;

        ExportExcelDialog *exportDialog;
        exportDialog = new ExportExcelDialog(filename,this);

        if(exportDialog->exec() == QDialog::Accepted)
        {
            excelpath = QString("./WortschatzEditor/exportexcel/"+ exportDialog->getFilePath() + ".xls");
            exportDialog->close();
            exportDialog->deleteLater();
            exportDialog = NULL;
            QFile file(excelpath);
            if(file.exists())
            {
                QMessageBox::information(this,tr("提示"),QString("该文件已经存在，不必再次导出!"));
            }
            else
            {
                //////////////////////////////////////////////////////////////////////////////////
                //
                QString excelname = QString("DRIVER={Microsoft Excel Driver (*.xls)};Readonly=0;"
                                                "DBQ=%1;DefaultDir=./WortschatzEditor/exportexcel/").arg(excelpath);
                ExportExcel *excel;
                excel = new ExportExcel(excelname,"EXCEL","QODBC");
                {
                    int rowCount = wordListDB->getRowCount();

                    //////////////////////////////////////////////////////////////
                    //
                    QMutex *mutex = new QMutex();
                    mutex->lock();
                    QProgressDialog *progressbar;
                    progressbar = new QProgressDialog(this);
                    progressbar->setWindowTitle("导入进度");
                    progressbar->setCancelButton(0);
                    progressbar->setRange(0,rowCount);
                    progressbar->setAutoClose(true);
                    ////////////////////////////////////////////
                    //
                    ui->action_export->setEnabled(false);
                    ui->action_import->setEnabled(false);
                    ui->action_quit->setEnabled(false);
                    ui->action_Standard->setEnabled(false);
                    ui->action_TXT->setEnabled(false);
                    ui->deleteRecordButton->setEnabled(false);
                    ui->newRecordButton->setEnabled(false);
                    ui->action_translation->setEnabled(false);
                    ui->action_export->setEnabled(false);
                    for(int i = 0; i < rowCount; i++)
                    {
                        /////////////////////////////////////////////
                        //
                        QStringList record = wordListDB->getRecord(i+1);
                        QString wort = record.at(0);
                        QString mean = record.at(1);
                        QString connection = record.at(2);
                        ///////////////////////////////////////////////////////////
                        uint lektion = (i+1)/200 + 1;
                        excel->addNewRecord(i+1,wort,mean,connection,lektion);

                        QString text = QString("正在导出第 %1 个词条，请等待……").arg(i+1);
                        progressbar->setLabelText(text);
                        progressbar->setModal(true);
                        progressbar->show();
                        progressbar->setValue(i+1);
                    }
                    /////////////////////////////////////////
                    //
                    ui->action_export->setEnabled(true);
                    ui->action_import->setEnabled(true);
                    ui->action_quit->setEnabled(true);
                    ui->action_Standard->setEnabled(true);
                    ui->action_TXT->setEnabled(true);
                    ui->deleteRecordButton->setEnabled(true);
                    ui->newRecordButton->setEnabled(true);
                    ui->action_translation->setEnabled(true);
                    ui->action_export->setEnabled(true);

                    //////////////////////////////
                    //
                    mutex->unlock();
                    delete mutex;
                    mutex = NULL;

                    ///////////////////////////
                    //
                    delete excel;
                    excel = NULL;
                }
            }
        }
        else
        {
            exportDialog->close();
            exportDialog->deleteLater();
            exportDialog = NULL;
        }
    }
}

void MainWindow::on_action_TXT_triggered()
{
    if(model->rowCount() > 0)
    {
        QMessageBox::warning(this, tr("导入TXT文件"),
                             tr("错误提示: \n词库内容不为空,无法导入!"
                                "\n请您清空词库内容或者新建词库再执行此操作!"));
    }
    else
    {
        ui->action_import->setEnabled(false);
        ui->action_quit->setEnabled(false);
        ui->action_Standard->setEnabled(false);
        ui->action_TXT->setEnabled(false);
        ui->deleteRecordButton->setEnabled(false);
        ui->newRecordButton->setEnabled(false);
        ui->action_translation->setEnabled(false);
        ui->action_export->setEnabled(false);

        ImportTXTDialog *importtxt;
        importtxt = new ImportTXTDialog(this);
        if(importtxt->exec() == QDialog::Accepted)
        {
            QString txtPath = importtxt->getTXTPath();

            importtxt->close();
            importtxt->deleteLater();
            importtxt = NULL;

            ImportTxt *txtThread;
            txtThread = new ImportTxt(txtPath,wordListDB,this);

            QString text = QString("正在计算要导入单词的总数，请稍候……");
            ui->InfoTextBrowser->append(text);

            int wordCount = txtThread->getWordCount();
            ui->InfoTextBrowser->append(QString("要导入的单词总数为：%1")
                                        .arg(wordCount));

            ui->statusBar->showMessage(QString("要导入的单词总数为：%1")
                                       .arg(wordCount),200000);

            ui->ProgressBar->setRange(0,wordCount);

            on_descendingOrderButton_clicked();

            QEventLoop loop;
            connect(txtThread,SIGNAL(transformEnded()),
                    &loop,SLOT(quit()));
            connect(txtThread,SIGNAL(addNewWord()),
                    this,SLOT(threadAddNewWord()));
            connect(txtThread,SIGNAL(transformEnded()),
                    this,SLOT(transformEnd()));

            ui->ProgressBar->setHidden(false);
            ui->InfoTextBrowser->setHidden(false);

            txtThread->start();
            loop.exec();
        }
        else
        {
            importtxt->close();
            importtxt->deleteLater();
            importtxt = NULL;
        }
    }
}

void MainWindow::on_action_translation_triggered()
{
    if(model->rowCount() > 0)
    {
        QMessageBox::warning(this, tr("转化德语助手生词本XML文件"),
                             tr("错误提示: \n词库内容不为空,无法导入!"
                                "\n请您清空词库内容或者新建词库再执行此操作!"));
    }
    else
    {
        ui->action_import->setEnabled(false);
        ui->action_quit->setEnabled(false);
        ui->action_Standard->setEnabled(false);
        ui->action_TXT->setEnabled(false);
        ui->deleteRecordButton->setEnabled(false);
        ui->newRecordButton->setEnabled(false);
        ui->action_translation->setEnabled(false);
        ui->action_export->setEnabled(false);

        TranslatDehelperDialog *importxml;
        importxml = new TranslatDehelperDialog(this);
        if(importxml->exec() == QDialog::Accepted)
        {
            QString xmlPath = importxml->getXMLPath();

            importxml->close();
            importxml->deleteLater();
            importxml = NULL;

            TranslatDehelperBackup *xmlThread;
            xmlThread = new TranslatDehelperBackup(xmlPath,wordListDB,this);

            QString text = QString("正在计算要导入单词的总数，请稍候……");
            ui->InfoTextBrowser->append(text);

            int wordCount = xmlThread->getWordCount();
            ui->InfoTextBrowser->append(QString("要导入的单词总数为：%1")
                                        .arg(wordCount));

            ui->statusBar->showMessage(QString("要导入的单词总数为：%1")
                                       .arg(wordCount),200000);

            ui->ProgressBar->setRange(0,wordCount);

            on_descendingOrderButton_clicked();

            QEventLoop loop;
            connect(xmlThread,SIGNAL(transformEnded()),
                    &loop,SLOT(quit()));
            connect(xmlThread,SIGNAL(addNewWord()),
                    this,SLOT(threadAddNewWord()));
            connect(xmlThread,SIGNAL(transformEnded()),
                    this,SLOT(transformEnd()));

            ui->ProgressBar->setHidden(false);
            ui->InfoTextBrowser->setHidden(false);

            xmlThread->start();
            loop.exec();
        }
        else
        {
            importxml->close();
            importxml->deleteLater();
            importxml = NULL;
        }
    }
}

void MainWindow::on_action_QT_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_action_quit_triggered()
{
    if (QMessageBox::information(this, "提示信息", "您确定要退出吗?",
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QApplication::closeAllWindows();
    }
    else
    {

    }
}

bool MainWindow::addNewRow(const uint &wid, const QString &wort, const QString &mean,
                           const QString &connection, const uint lektion)
{
    wordListDB->getdb()->transaction();//开始事务操作

    int rowNum = wordListDB->getRowCount();
    qDebug() << rowNum;
    model->insertRow(rowNum);//添加一行
    model->setData(model->index(rowNum,0),wid);
    model->setData(model->index(rowNum,1),wort);
    model->setData(model->index(rowNum,2),mean);
    model->setData(model->index(rowNum,3),connection);
    model->setData(model->index(rowNum,4),lektion);

    model->submitAll();//提交所有被修改的数据到数据库中

    wordListDB->getdb()->commit();//提交成功，事务将真正修改数据库数据
    return true;
}

bool MainWindow::addNewRecord(const QSqlRecord &record)
{
    wordListDB->getdb()->transaction();//开始事务操作
    int rowNum = wordListDB->getRowCount();
    qDebug() << rowNum;

    model->insertRecord(rowNum,record);

    model->submitAll();// 提交所有被修改的数据到数据库中
    wordListDB->getdb()->commit();//提交成功，事务将真正修改数据库数据
    return true;
}

void MainWindow::on_goTolineEdit_textChanged(const QString &keyWort)
{
    ui->newRecordButton->setDisabled(true);
    //根据姓名进行筛选
    if(asWortSelect == "TRUE" && asIdSelect == "FALSE")
    {
        model->setFilter(QString("Wort like '%%1%'").arg(keyWort));
        //显示结果
        model->select();
    }
    else if(asWortSelect == "FALSE" && asIdSelect == "TRUE")
    {
        model->setFilter(QString("Wid = '%1'").arg(keyWort.toInt()));
        //显示结果
        model->select();
    }
}

void MainWindow::on_action_GoogleTTS_triggered()
{
    ui->action_import->setEnabled(false);
    ui->action_quit->setEnabled(false);
    ui->action_Standard->setEnabled(false);
    ui->action_TXT->setEnabled(false);
    ui->deleteRecordButton->setEnabled(false);
    ui->newRecordButton->setEnabled(false);
    ui->action_translation->setEnabled(false);
    ui->action_export->setEnabled(false);

    wordList = new WordListCreate(this,wordListDB);
    M_WordList = wordList->getWordList();
    ui->InfoTextBrowser->clear();
    QString text = QString("正在计算要创建的列表的单词量，请稍候……");
    ui->InfoTextBrowser->append(text);

    int wordCount = wordListDB->getRowCount();
    ui->InfoTextBrowser->append(QString("要创建的列表的单词量为：%1")
                                .arg(wordCount));

    ui->statusBar->showMessage(QString("要创建的列表的单词量为：%1")
                               .arg(wordCount),200000);

    ui->ProgressBar->setRange(0,wordCount);

    model = wordList->getModel();
    model->setTable("WordList");
    QEventLoop loop;
    connect(wordList,SIGNAL(creatWordListEnded()),
            &loop,SLOT(quit()));
    connect(wordList,SIGNAL(addNewRecord()),
            this,SLOT(threadAddNewRecord()));
    connect(wordList,SIGNAL(creatWordListEnded()),
            this,SLOT(threadCreatWordListEnded()));

    ui->ProgressBar->setHidden(false);
    ui->InfoTextBrowser->setHidden(false);

    wordList->start();
    loop.exec();
}

void MainWindow::on_action_Standard_triggered()
{
    StandardDialog *standard;
    standard = new StandardDialog(this);
    if(standard->exec() == QDialog::Accepted)
    {
        ui->action_import->setEnabled(false);
        ui->action_quit->setEnabled(false);
        ui->action_Standard->setEnabled(false);
        ui->action_TXT->setEnabled(false);
        ui->deleteRecordButton->setEnabled(false);
        ui->newRecordButton->setEnabled(false);
        ui->action_translation->setEnabled(false);
        ui->action_export->setEnabled(false);

        QStringList standardItems = standard->getStandardItems();
        standard->close();
        standard->deleteLater();
        standard = NULL;

        StandardDB * standardThread;
        standardThread = new StandardDB(wordListDB,this);

        standardThread->setStandardItems(standardItems);

        ui->InfoTextBrowser->clear();
        QString text = QString("正在计算要规范的词库的单词量的总数，请稍候……");
        ui->InfoTextBrowser->append(text);

        int wordCount = wordListDB->getRowCount();
        ui->InfoTextBrowser->append(QString("要规范的词库单词总数为：%1")
                                    .arg(wordCount));

        ui->statusBar->showMessage(QString("要规范的词库单词总数为：%1")
                                   .arg(wordCount),200000);

        ui->ProgressBar->setRange(0,wordCount);

        QEventLoop loop;
        connect(standardThread,SIGNAL(standardEnded()),
                &loop,SLOT(quit()));
        connect(standardThread,SIGNAL(standardNewRecord()),
                this,SLOT(threadStandardNewRecord()));
        connect(standardThread,SIGNAL(standardEnded()),
                this,SLOT(threadStandardEnd()));

        ui->ProgressBar->setHidden(false);
        ui->InfoTextBrowser->setHidden(false);

        standardThread->start();
        loop.exec();
    }
    else
    {
        standard->close();
        standard->deleteLater();
        standard = NULL;
    }
}

void MainWindow::on_action_AutoLektion_triggered()
{
    AutoFillingUnitDialog *autofilling;
    autofilling = new AutoFillingUnitDialog(this);
    if(autofilling->exec() == QDialog::Accepted)
    {
        ui->action_AutoLektion->setEnabled(false);
        ui->action_import->setEnabled(false);
        ui->action_quit->setEnabled(false);
        ui->action_Standard->setEnabled(false);
        ui->action_TXT->setEnabled(false);
        ui->deleteRecordButton->setEnabled(false);
        ui->newRecordButton->setEnabled(false);
        ui->action_export->setEnabled(false);
        ui->action_change->setEnabled(false);
        ui->action_translation->setEnabled(false);
        ui->action_export->setEnabled(false);

        int wordNumInUnit = autofilling->returnWordNumUnit();
        autofilling->close();
        autofilling->deleteLater();
        autofilling = NULL;

        AutomaticFillingUnit * autoFillingThread;
        autoFillingThread = new AutomaticFillingUnit(wordListDB,wordNumInUnit,this);

        ui->InfoTextBrowser->clear();
        QString text = QString("正在计算要自动写入单元信息的词库的单词量的总数，请稍候……");
        ui->InfoTextBrowser->append(text);

        int wordCount = wordListDB->getRowCount();
        ui->InfoTextBrowser->append(QString("要自动写入单元信息的词库单词总数为：%1")
                                    .arg(wordCount));

        ui->statusBar->showMessage(QString("要自动写入单元信息的词库单词总数为：%1")
                                   .arg(wordCount),200000);

        ui->ProgressBar->setRange(0,wordCount);

        QEventLoop loop;
        connect(autoFillingThread,SIGNAL(fillingEnded()),
                &loop,SLOT(quit()));
        connect(autoFillingThread,SIGNAL(fillingNewWordUnit()),
                this,SLOT(threadAutoFillingNewRecord()));
        connect(autoFillingThread,SIGNAL(fillingEnded()),
                this,SLOT(threadAutoFillingEnd()));

        ui->ProgressBar->setHidden(false);
        ui->InfoTextBrowser->setHidden(false);

        autoFillingThread->start();
        loop.exec();
    }
    else
    {
        autofilling->close();
        autofilling->deleteLater();
        autofilling = NULL;
    }
}

void MainWindow::on_action_givemoney_triggered()
{

}

