#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <exception>
#include <QtCore>
#include <QThread>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>
#include <QColor>
#include <QShortcut>
#include <QMap>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QMenu>
#include <QAction>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    removed = false;
    ui->setupUi(this);
    this->setFixedSize(this->size().width(),this->size().height());
    //this->setMinimumSize(this->size().width(),this->size().height());

    createMenus();
    UpdateConfig();//Update the configuration settings
    initConditions();
    initSkills();
    initClasses();
    initEnemies();
    initActors();
    initGlobal();
    //qDebug() << QString("num%3").arg(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
//    qDebug() << "resized";
}

void MainWindow::createMenus()
{
   fileMenu = ui->menubar->addMenu(tr("&File"));
   //editMenu = ui->menubar->addMenu(tr("&Edit"));
   helpMenu = ui->menubar->addMenu(tr("&Help"));
   //recentFiles = fileMenu->addMenu("&Recent Files");
   //New Action
   QAction *newAct = new QAction(this);
   newAct->setShortcuts(QKeySequence::New);
   newAct->setStatusTip(tr("Create a new file"));
   newAct->setText("New");
   connect(newAct, SIGNAL(triggered()), this, SLOT(newf()));
   fileMenu->addAction(newAct);
   //Open Action
   QAction *openAct = new QAction(this);
   openAct->setShortcuts(QKeySequence::Open);
   openAct->setStatusTip(tr("Open an existing file"));
   openAct->setText("Open...");
   connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
   fileMenu->addAction(openAct);
   //Save Action
   QAction *saveAct = new QAction(this);
   saveAct->setShortcuts(QKeySequence::Save);
   saveAct->setStatusTip(tr("Save the current file"));
   saveAct->setText("Save");
   connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
   fileMenu->addAction(saveAct);
   //Save as Action
   QAction *saveAsAct = new QAction(this);
   saveAsAct->setShortcuts(QKeySequence::SaveAs);
   saveAsAct->setStatusTip(tr("Save to a desired file"));
   saveAsAct->setText("Save As...");
   connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
   fileMenu->addAction(saveAsAct);
   //Export Action
   QAction *exportAct = new QAction(this);
   exportAct->setShortcuts(QKeySequence::Replace);
   exportAct->setStatusTip(tr("Export game file"));
   exportAct->setText("Export");
   connect(exportAct, SIGNAL(triggered()), this, SLOT(exportf()));
   fileMenu->addAction(exportAct);
   //Help Menu
   //Stats
   QAction *statDesc = new QAction(this);
   statDesc->setText("Stats");
   connect(statDesc, SIGNAL(triggered()), this, SLOT(statDesc()));
   helpMenu->addAction(statDesc);
   //Conditions
   QAction *conditionDesc = new QAction(this);
   conditionDesc->setText("Conditions");
   connect(conditionDesc, SIGNAL(triggered()), this, SLOT(conditionDesc()));
   helpMenu->addAction(conditionDesc);
   //Skills
   QAction *skillDesc = new QAction(this);
   skillDesc->setText("Skills");
   connect(skillDesc, SIGNAL(triggered()), this, SLOT(skillDesc()));
   helpMenu->addAction(skillDesc);
   //Functions
   QAction *functionDesc = new QAction(this);
   functionDesc->setText("Functions");
   connect(functionDesc, SIGNAL(triggered()), this, SLOT(functionsDesc()));
   helpMenu->addAction(functionDesc);
   //Classes
   QAction *classDesc = new QAction(this);
   classDesc->setText("Classes");
   connect(classDesc, SIGNAL(triggered()), this, SLOT(classDesc()));
   helpMenu->addAction(classDesc);
   //Enemies
   QAction *enemyDesc = new QAction(this);
   enemyDesc->setText("Enemies");
   connect(enemyDesc, SIGNAL(triggered()), this, SLOT(enemyDesc()));
   helpMenu->addAction(enemyDesc);
   //Actors
   QAction *actorDesc = new QAction(this);
   actorDesc->setText("Actor");
   connect(actorDesc, SIGNAL(triggered()), this, SLOT(actorDesc()));
   helpMenu->addAction(actorDesc);

}

void MainWindow::newf()
{
    if(currentFile!=nullptr)
    {
        delete currentFile;
        currentFile = nullptr;
    }

    reset();
    updateConditions();
}

void MainWindow::reset()
{
    //Reset global
    global = Global();
    showGlobal();
    //Reset actors
    ui->actor_list->blockSignals(true);
    ui->actor_list->clear();
    actors.clear();
    actors.append(Actor());
    ui->actor_list->addItem("1: "+actors.back().name);
    ui->actor_list->blockSignals(false);
    ui->actor_list->setCurrentRow(0);
    //Reset enemies
    ui->enemy_list->blockSignals(true);
    ui->enemy_list->clear();
    enemies.clear();
    enemies.append(Enemy());
    ui->enemy_list->addItem("1: "+enemies.back().name);
    ui->enemy_list->blockSignals(false);
    ui->enemy_list->setCurrentRow(0);
    //Reset classes
    ui->class_list->blockSignals(true);
    ui->class_list->clear();
    classes.clear();
    classes.append(Class());
    ui->class_list->addItem("1: "+classes.back().name);
    ui->class_list->blockSignals(false);
    ui->class_list->setCurrentRow(0);
    //Reset skills
    ui->skill_list->blockSignals(true);
    ui->skill_list->clear();
    skills.clear();
    skills.append(Skill());
    ui->skill_list->addItem("1: "+skills.back().name);
    ui->skill_list->blockSignals(false);
    ui->skill_list->setCurrentRow(0);
    //Reset conditions
    ui->condition_list->blockSignals(true);
    ui->condition_list->clear();
    conditions.clear();
    conditions.append(Condition());
    ui->condition_list->addItem("1: "+conditions.back().name);
    ui->condition_list->blockSignals(false);
    ui->condition_list->setCurrentRow(0);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open"), openDir, tr("Actor (*.json)"));
    QSettings settings("Warden", "Skill Editor");
    if(fileName!="")
    {
        settings.setValue("config/openDir",fileName);
        openDir = fileName;
        delete currentFile;
        currentFile = new QFile(fileName);
    }
    else
    {
        return;
    }

    if(!currentFile->open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(currentFile->errorString());
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(currentFile->readAll());
    QJsonObject root = doc.object();

    //Verify file
    if(!root.contains("actors")||!root.contains("enemies")||!root.contains("classes")||!root.contains("skills"))
    {
        delete currentFile;
        QMessageBox msgBox;
        msgBox.setText("Invalid gamefile.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    global.read(root);
    showGlobal();

    removeActors();
    removeEnemies();
    removeClasses();
    removeSkills();
    removeConditions();

    //Read conditions
    QMap<QString,Condition*> conMap;
    QJsonArray a = root["conditions"].toArray();
    conditions.first().read(a[0].toObject());
    ui->condition_list->item(0)->setText("1: "+conditions.back().name);
    conMap.insert(conditions.first().name,&conditions.first());
    showCondition();
    for(int i=1;i<a.size();i++)
    {
        conditions.append(Condition(a[i].toObject()));
        QString header = QString::number(i+1)+": ";
        ui->condition_list->addItem(header+conditions.back().name);
        conMap.insert(conditions.back().name,&conditions.back());
    }
    updateConditions();
    //Read skills
    QMap<QString,Skill*> skillMap;
    a = root["skills"].toArray();
    skills.first().readMeta(a[0].toObject(),conMap);
    ui->skill_list->item(0)->setText("1: "+skills.back().name);
    skillMap.insert(skills.back().name,&skills.back());
    showSkill();
    for(int i=1;i<a.size();i++)
    {
        skills.append(Skill(a[i].toObject(),conMap));
        QString header = QString::number(i+1)+": ";
        ui->skill_list->addItem(header+skills.back().name);
        skillMap.insert(skills.back().name,&skills.back());
    }
    //Read Classes
    updateClassSkills();
    QMap<QString,Class*> classMap;
    a = root["classes"].toArray();
    classes.first().readMeta(a[0].toObject(),skillMap);
    ui->class_list->item(0)->setText("1: "+classes.back().name);
    classMap.insert(classes.back().name,&classes.back());
    showClass();
    for(int i=1;i<a.size();i++)
    {
        classes.append(Class(a[i].toObject(),skillMap));
        QString header = QString::number(i+1)+": ";
        ui->class_list->addItem(header+classes.back().name);
        classMap.insert(classes.back().name,&classes.back());
    }
    //Read Enemies
   // updateEnemySkills();
    a = root["enemies"].toArray();
    enemies.first().readMeta(a[0].toObject(),skillMap);
    ui->enemy_list->item(0)->setText("1: "+enemies.back().name);
    showEnemy();
    for(int i=1;i<a.size();i++)
    {
        enemies.append(Enemy(a[i].toObject(),skillMap));
        QString header = QString::number(i+1)+": ";
        ui->enemy_list->addItem(header+enemies.back().name);
    }
    //Read Actors
    a = root["actors"].toArray();
    actors.first().readMeta(a[0].toObject(),classMap);
    ui->actor_list->item(0)->setText("1: "+actors.back().name);
    showActor();
    for(int i=1;i<a.size();i++)
    {
        actors.append(Actor(a[i].toObject(),classMap));
        QString header = QString::number(i+1)+": ";
        ui->actor_list->addItem(header+actors.back().name);
    }
    currentFile->close();
}

void MainWindow::save()
{
    //Check if the user is currently working on a file
    if(currentFile==nullptr)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save"), openDir, tr("Actor (*.json)"));
        QSettings settings("Warden", "Skill Editor");
        if(fileName!="")
        {
            settings.setValue("config/openDir",fileName);
            openDir = fileName;
            delete currentFile;
            currentFile = new QFile(fileName);
        }
        else
        {
            return;
        }
    }

    if(!currentFile->open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(currentFile->errorString());
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        delete currentFile;
        currentFile = nullptr;
        return;
    }

    QJsonObject root;
    QJsonArray a;

    for(int i=0;i<conditions.length();i++)
    {
        QJsonObject obj;
        conditions[i].write(obj);
        a.append(obj);
    }
    root["conditions"] = a;

    a = QJsonArray();
    for(int i=0;i<skills.length();i++)
    {
        QJsonObject obj;
        skills[i].writeMeta(obj);
        a.append(obj);
    }
    root["skills"] = a;

    a = QJsonArray();
    for(int i=0;i<classes.length();i++)
    {
        QJsonObject obj;
        classes[i].writeMeta(obj);
        a.append(obj);
    }
    root["classes"] = a;

    a = QJsonArray();
    for(int i=0;i<enemies.length();i++)
    {
        QJsonObject obj;
        enemies[i].writeMeta(obj);
        a.append(obj);
    }
    root["enemies"] = a;

    a = QJsonArray();
    for(int i=0;i<actors.length();i++)
    {
        QJsonObject obj;
        actors[i].writeMeta(obj);
        a.append(obj);
    }
    root["actors"] = a;

    global.writeMeta(root);
    QTextStream out(currentFile);
    out << QJsonDocument(root).toJson(QJsonDocument::JsonFormat::Indented);
    currentFile->flush();
    currentFile->close();
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Save"), openDir, tr("Actor (*.json)"));
    QSettings settings("Warden", "Skill Editor");
    if(fileName!="")
    {
        settings.setValue("config/openDir",fileName);
        openDir = fileName;
        delete currentFile;
        currentFile = new QFile(fileName);
    }
    else
    {
        return;
    }

    if(!currentFile->open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(currentFile->errorString());
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        delete currentFile;
        currentFile = nullptr;
        return;
    }

    QJsonObject root;
    QJsonArray a;

    for(int i=0;i<conditions.length();i++)
    {
        QJsonObject obj;
        conditions[i].write(obj);
        a.append(obj);
    }
    root["conditions"] = a;

    a = QJsonArray();
    for(int i=0;i<skills.length();i++)
    {
        QJsonObject obj;
        skills[i].writeMeta(obj);
        a.append(obj);
    }
    root["skills"] = a;

    a = QJsonArray();
    for(int i=0;i<classes.length();i++)
    {
        QJsonObject obj;
        classes[i].writeMeta(obj);
        a.append(obj);
    }
    root["classes"] = a;

    a = QJsonArray();
    for(int i=0;i<enemies.length();i++)
    {
        QJsonObject obj;
        enemies[i].writeMeta(obj);
        a.append(obj);
    }
    root["enemies"] = a;

    a = QJsonArray();
    for(int i=0;i<actors.length();i++)
    {
        QJsonObject obj;
        actors[i].writeMeta(obj);
        a.append(obj);
    }
    root["actors"] = a;

    global.writeMeta(root);
    QTextStream out(currentFile);
    out << QJsonDocument(root).toJson(QJsonDocument::JsonFormat::Indented);
    currentFile->flush();
    currentFile->close();
}

void MainWindow::exportf()
{
    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Export"), openDir, tr("gameFile (*.json)"));
    QSettings settings("Warden", "Skill Editor");
    QFile *f;
    if(fileName!="")
    {
        settings.setValue("config/exportDir",fileName);
        f = new QFile(settings.value("config/exportDir","/Users/").toString());
    }
    else
    {
        return;
    }
    if(!f->open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(f->errorString());
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    QJsonObject root;

    global.write(root,actors,enemies);

    QTextStream out(f);
    out << QJsonDocument(root).toJson(QJsonDocument::JsonFormat::Indented);
    f->flush();
    f->close();
}

void MainWindow::readFile()
{

}

//Checks if there's a current file set, if not the user is prompted to select one
bool MainWindow::checkCurrentFile(QString message)
{
    if(currentFile==nullptr)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
        message, openDir, tr("Actor (*.json)"));
        QSettings settings("Warden", "Skill Editor");
        if(fileName!="")
        {
            settings.setValue("config/openDir",fileName);
            openDir = fileName;
            delete currentFile;
            currentFile = new QFile(fileName);
            return true;
        }
        return false;
    }
    return true;
}

void MainWindow::UpdateConfig()
{
    QSettings settings("Warden", "Skill Editor");
    openDir = settings.value("config/openDir","/Users/").toString();
    QString fileName = settings.value("currentFile","").toString();
    currentFile = fileName==""?nullptr:new QFile(fileName);
    //Check if there was a file stored in QSettings
    if(currentFile!=nullptr)
    {
        //Check if the file was moved/deleted
        /*
        if(!currentFile->exists())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error");
            msgBox.setText("Could not find "+QFileInfo(fileName).fileName());
            settings.remove("currentFile");
            delete currentFile;
            currentFile = nullptr;
        }*/
    }
}

void MainWindow::addRecent(QString dir)
{
    QSettings settings("Warden", "Skill Editor");
    int i = 0;
    while(settings.value("config/recent"+i,false)!=false&&i<5)
    {
        ++i;
    }
    settings.setValue("config/recent"+i,dir);
    QAction *openAct = new QAction(this);
    openAct->setText(dir);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open_dir()));
    //delete recentFiles->actions().at(0);
    recentFiles->actions().replace(0,openAct);
}

/* HELP MENU */
void MainWindow::statDesc()
{
    QMessageBox msgBox;
    msgBox.setText(" <font size = 5 color = #55AA55>Stats determine an actors effectiveness.</font>" );
    msgBox.setInformativeText("<font size = 3 color = #ffffff><font size = 4 color=#55AA55>Attack - </font>"
                              "determines the effictiveness of all attack skillls.<br>"
                              "<font size = 4 color=#55AA55>Defence - </font>"
                              "Mitigates damage from attacks.<br>"
                              "<font size = 4 color=#55AA55>Intelligence - </font>"
                              "determines the effictiveness of most healing skills<br>"
                              "and determines sp recovery per turn.<br>"
                              "<font size = 4 color=#55AA55>Agility - </font>"
                              "determines the chance of dodging attacks and 50%<br>"
                              "of damage done with daggers, also determines<br>"
                              "the targets place in a turn.<br>"
                              "<font size = 4 color=#55AA55>Hit - </font>"
                              "determines the chance to make an attack and 50%<br>"
                              "of damage done with guns.<br>"
                              "<font size = 4 color=#55AA55>Health - </font>"
                              "the actor's health, the actor dies when this reaches 0.<br>"
                              "<font size = 4 color=#55AA55>SP - </font>"
                              "used on skills.<br>"
                              "<font size = 4 color=#55AA55>MP - </font>"
                              "the number of tiles an actor can move per turn.</font>");
    msgBox.setWindowTitle("Stats");
    msgBox.setStyleSheet("background-color:black;");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void MainWindow::conditionDesc()
{
    QMessageBox msgBox;
    msgBox.setText("<font size = 5 color = #55AA55>Conditions are attached to actors.</font>");
    msgBox.setInformativeText("<font size = 3 color = #ffffff><font size = 4 color=#55AA55>Turns - </font>"
                              "the Number of turns that this condition will last.<br>"
                              "<font size = 4 color=#55AA55>Value - </font>"
                              "the amount this condition will affect stats.<br>"
                              "<font size = 4 color=#55AA55>Max Stacks - </font>"
                              "the amount this condition can be stacked<br>(0 meaning not stackable.)<br>"
                              "<font size = 4 color=#55AA55>Repeat - </font>"
                              "determines when the condition is applied. if on, the condition is<br>"
                              "applied every turn to the actor, otherwise it's applied only when added to an actor.<br>"
                              "<font size = 4 color=#55AA55>isPercent - </font>"
                              "determines whether the value is a percentage or constant integer.<br>"
                              "<font size = 4 color=#55AA55>Reset - </font>"
                              "determines whether the effect of the condition will be reversed<br>"
                              "when the condition ends e.g. stats set back to what they were<br>"
                              "before it was added.</font>");
    msgBox.setWindowTitle("Conditions");
    msgBox.setStyleSheet("background-color:black;");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void MainWindow::skillDesc()
{
    //QMessageBox::information( 0, "Skills" , "<font size = 10  color = red > Hello World </font>" );
    QMessageBox msgBox;
    msgBox.setText(" <font size = 5 color = #55AA55>Skills are assigned to actors for use in battle.</font>" );
    msgBox.setInformativeText("<font size = 3 color = #ffffff><font size = 4 color=#55AA55>"
                              "pow - </font>The power of the skill, determines effectiveness.<br>"
                              "<font size = 4 color=#55AA55>spcost - </font>The sp cost of the skill.<br>"
                              "<font size = 4 color=#55AA55>iattackrange - </font>"
                              "The range the skill can't target from the actor.<br>"
                              "<font size = 4 color=#55AA55>oattackrange - </font>"
                              "The range the skill can target from the actor.<br>"
                              "<font size = 4 color=#55AA55>itargetrange - </font>"
                              "The range the skill can't target from the target.<br>"
                              "<font size = 4 color=#55AA55>otargetrange - </font>"
                              "The range the skill can target from the target.<br>"
                              "<font size = 4 color=#55AA55>range - </font>"
                              "The range type of an Active Skill.<br>"
                              "<font size = 4 color=#55AA55>condition chance - </font>"
                              "The chance that the condition will be applied</br>"
                              "when the skill hits the target(s).<br>"
                              "<font size = 4 color=#55AA55>Dynamics - </font>"
                              "Functions that change the attributes of Attack/Healing<br>"
                              "skills depending on the users weapon.<br>"
                              "<font size = 4 color=#55AA55>Functions - </font>"
                              "Functions are called on the target(s) of a skill.<br>"
                              "The chance field determines the chance of a function<br>"
                              "being called after the skill hits its target(s).<br>"
                              "The parameter field determines is unique to each function, refer to functions.<br>"
                              "<font size = 4 color=#55AA55>Reliant Stats - </font>"
                              "The stat(s) that determines the effectiveness of Attack/Healing skills.<br>"
                              "<font size = 4 color=#55AA55>Affected Stats - </font>"
                              "The stat(s) that a passive skill affects.</font>");
    msgBox.setWindowTitle("Skills");
    msgBox.setStyleSheet("background-color:black;");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void MainWindow::functionsDesc()
{
    QMessageBox msgBox;
    msgBox.setText("<font size = 5 color = #55AA55>Functions allow skills to perform unique actions.</font>" );
    msgBox.setInformativeText("<font size = 3 color = #ffffff><font size = 4 color=#55AA55>DrainHP - </font>"
                              "drains the targets hp by the amount evaluated from the parameter.<br>"
                              "<font size = 4 color=#55AA55>DrainSP - </font>"
                              "drains the targets hp by the amount evaluated from the parameter.<br>"
                              "<font size = 4 color=#55AA55>Revive - </font>"
                              "revives the target with health evaluated from the parameter.<br>"
                              "<font size = 4 color=#55AA55>Block - </font>"
                              "causes the target to block the next attack.<br>"
                              "<font size = 4 color=#55AA55>removeDebuffs - </font>"
                              "removes all debuffs from the target.<br>"
                              "<font size = 4 color=#55AA55>removeBuffs - </font>"
                              "removes all bufss from the target.<br>"
                              "<font size = 4 color=#55AA55>Stun - </font>"
                              "stuns the target, the parameter determines the turns the efffect will last.</font>");
    msgBox.setWindowTitle("Functions");
    msgBox.setStyleSheet("background-color:black;");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void MainWindow::classDesc()
{
    QMessageBox msgBox;
    msgBox.setText(" <font size = 5 color = #55AA55>Classes are assigned to playable actors.</font>" );
    msgBox.setInformativeText("<font size = 3 color = #ffffff>Classes contain 5 skills each and must have one of 12 types.\n"
                              "A class can contain a total of 5 maximum skills of any type.</font>");
    msgBox.setWindowTitle("Classes");
    msgBox.setStyleSheet("background-color:black;");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void MainWindow::enemyDesc()
{
    QMessageBox msgBox;
    msgBox.setText(" <font size = 5 color = #55AA55>Enemies are NPC actors that are fought in battles.</font>" );
    msgBox.setInformativeText("<font size = 3 color = #ffffff>Enemies contain a "
                              "combination of active and passive skills.<br>"
                              "Enemies do not have levels and can not gain experience.</font>");
    msgBox.setWindowTitle("Enemies");
    msgBox.setStyleSheet("background-color:black;");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void MainWindow::actorDesc()
{
    QMessageBox msgBox;
    msgBox.setText(" <font size = 5 color = #55AA55>Actors are playable characters.</font>" );
    msgBox.setInformativeText("<font size = 3 color = #ffffff>An actor contains"
                              " a minimum and maximum stat range.<br>"
                              "These stats represent the characters' base stats at level 1"
                              " and the max level respectively.</font>");
    msgBox.setWindowTitle("Actors");
    msgBox.setStyleSheet("background-color:black;");
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

QString Read(QString Filename){
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not open file.");
        msgBox.exec();
        return "";
    }
    QTextStream in(&mFile);
    QString mText = in.readAll();
    qDebug() << mText;

    mFile.close();
    return mText;
}

void Write(QString Filename){
    QFile mFile(Filename);

    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "Could not open file";
        QMessageBox msgBox;
        msgBox.setText("Could not open file.");
        msgBox.exec();
        return;
    }
    QTextStream out(&mFile);
    out << "test";
    mFile.flush();
    mFile.close();
}

void MainWindow::initConditions()
{
    counter = 1;
    if(conditions.length()==0)
    {
        conditions.append(*new Condition());
        ui->condition_list->addItem(conditions.back().name);
        cur = 0;
        ui->condition_list->setCurrentRow(cur);
    }

    QShortcut* insert_shortcut = new QShortcut(QKeySequence(Qt::Key_Insert),ui->condition_list);
    connect(insert_shortcut,SIGNAL(activated()),this,SLOT(addCondition()));

    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),ui->condition_list);
    connect(shortcut,SIGNAL(activated()),this,SLOT(deleteCondition()));
}

void MainWindow::uniqueCondition()
{
    QString text = ui->condition_name_textfield->text();
    int i = 0;
    for(int j=0;j<conditions.length();j++)
    {
        if(conditions[j].name==text&&j!=cur)
        {
            text = text.append(QString::number(i));
            conditions[cur].name = text;
            ui->condition_list->currentItem()->setText(text);
            ui->condition_name_textfield->setText(text);
            j=0;
            ++i;
        }
    }
}

void MainWindow::on_condition_add_clicked()
{
   addCondition();
}

void MainWindow::on_condition_list_currentRowChanged(int currentRow)
{
    cur = currentRow;
    showCondition();
}

void MainWindow::showCondition(){
    Condition con = conditions[cur];
    if(!removed){
        QString header = QString::number(cur+1)+": ";
        ui->condition_name_textfield->setText(con.name);
        ui->condition_list->currentItem()->setText(con.name.prepend(header));
        ui->condition_reset->setChecked(con.reset);
        ui->condition_repeat->setChecked(con.repeat);
        ui->condition_percentage->setChecked(con.isPercent);
        ui->condition_turns_spin->setValue(con.turns);
        ui->condition_max_stacks_spin->setValue(con.maxStacks);
        ui->condition_value_spin->setValue(con.v);
        ui->affected_stats_list->clear();
        for(int i=0;i<conditions[cur].affectedStats.size();i++)
        {
            ui->affected_stats_list->addItem(conditions[cur].affectedStats[i]);
        }
        ui->affected_stats_list->addItem("---");
        ui->affected_stats_list->setCurrentRow(0);
    }
}

void MainWindow::on_condition_name_textfield_editingFinished()
{
    uniqueCondition();
}


void MainWindow::on_condition_name_textfield_textChanged(const QString &arg1)
{
    if(cur>-1)
    {
        QString header = QString::number(cur+1)+": ";
        ui->condition_list->currentItem()->setText(header+arg1);
        this->conditions[cur].name = arg1;
    }
}

void MainWindow::on_condition_max_stacks_spin_valueChanged(int arg1)
{
    conditions[cur].maxStacks = arg1;
}

void MainWindow::on_condition_turns_spin_valueChanged(int arg1)
{
    conditions[cur].turns = arg1;
}

void MainWindow::on_condition_value_spin_valueChanged(int arg1)
{
    conditions[cur].v = arg1;
}

void MainWindow::on_condition_repeat_clicked(bool checked)
{
    conditions[cur].repeat = checked;
}

void MainWindow::on_condition_reset_clicked(bool checked)
{
    conditions[cur].reset = checked;
}

void MainWindow::on_condition_percentage_clicked(bool checked)
{
    conditions[cur].isPercent = checked;
}

void MainWindow::on_condition_remove_clicked()
{
    deleteCondition();
}

void MainWindow::addCondition()
{
    conditions.append(*new Condition());
    ui->condition_list->addItem(conditions.back().name);
    QString header = QString::number(conditions.length())+": ";
    QString text = conditions[conditions.length()-1].name;
    ui->condition_list->item(conditions.length()-1)->setText(header+text);
    for(int j=conditions.length()-1;j>-1;j--)
    {
        if(conditions[j].name==text&&j!=conditions.length()-1)
        {
            text = text.append(QString::number(counter));
            conditions.back().name = text;
            ui->condition_list->item(conditions.length()-1)->setText(header+text);
            j=conditions.length()-1;
            ++counter;
        }
    }
}

void MainWindow::deleteCondition()
{
    if(conditions.length()>1)
    {
        removed = true;
        for(int i=0;i<skills.length();i++)
        {
            if(skills[i].condition==&conditions[cur])
            {
                skills[i].condition = nullptr;
            }
        }
        conditions.removeAt(cur);
        if(cur==conditions.size())
        {
            cur-=1;
        }
        ui->condition_name_textfield->setText(conditions[cur].name);
        ui->condition_reset->setChecked(conditions[cur].reset);
        ui->condition_repeat->setChecked(conditions[cur].repeat);
        ui->condition_percentage->setChecked(conditions[cur].isPercent);
        ui->condition_turns_spin->setValue(conditions[cur].turns);
        ui->condition_max_stacks_spin->setValue(conditions[cur].maxStacks);
        ui->condition_value_spin->setValue(conditions[cur].v);
        for(int i=cur;i<conditions.size();i++)
        {
            QString header = QString::number(i+1)+": ";
            ui->condition_list->item(i)->setText(header+conditions[i].name);
        }
        delete ui->condition_list->takeItem(conditions.size());
        removed = false;
    }
}

void MainWindow::removeConditions()
{
    conditions[0].affectedStats.clear();
    ui->condition_list->setCurrentRow(0);
    int l = conditions.length();

    for(int i=l-1;i>0;i--)
    {
        conditions.removeAt(i);
        delete ui->condition_list->takeItem(i);
    }
}

void MainWindow::on_condition_stat_combo_currentIndexChanged(int index)
{
    int r = ui->affected_stats_list->currentRow();
    //Check if there's a value in the model
    if(conditions[cur].affectedStats.size()-1<r&&
            ui->affected_stats_list->currentItem()->text()=="---"&&
            index!=0)
    {
        conditions[cur].affectedStats.append("");
    }
    //If a null stat gets set to a value, add a new item.
    if(ui->affected_stats_list->currentItem()->text()=="---"&&index!=0)
    {
        ui->affected_stats_list->addItem("---");
    }
    //If a stat gets set to null, remove it from the list
    else if(ui->affected_stats_list->currentItem()->text()!="---"&&index==0)
    {
        deleteAffectedStat();
    }
    switch(index)
    {
        case attack:
            conditions[cur].affectedStats[r] = "attack";
            ui->affected_stats_list->currentItem()->setText("attack");
            break;
        case defence:
            conditions[cur].affectedStats[r] = "defence";
            ui->affected_stats_list->currentItem()->setText("defence");
            break;
        case intelligence:
            conditions[cur].affectedStats[r] = "intelligence";
            ui->affected_stats_list->currentItem()->setText("intelligence");
            break;
        case agility:
            conditions[cur].affectedStats[r] = "agility";
            ui->affected_stats_list->currentItem()->setText("agility");
            break;
        case hit:
            conditions[cur].affectedStats[r] = "hit";
            ui->affected_stats_list->currentItem()->setText("hit");
            break;
        case health:
            conditions[cur].affectedStats[r] = "health";
            ui->affected_stats_list->currentItem()->setText("health");
            break;
        case sp:
            conditions[cur].affectedStats[r] = "sp";
            ui->affected_stats_list->currentItem()->setText("sp");
            break;
        case mp:
            conditions[cur].affectedStats[r] = "mp";
            ui->affected_stats_list->currentItem()->setText("mp");
            break;
        case dr:
            conditions[cur].affectedStats[r] = "dr";
            ui->affected_stats_list->currentItem()->setText("dr");
            break;
    }

}

void MainWindow::deleteAffectedStat()
{
    int r = ui->affected_stats_list->currentRow();
    conditions[cur].affectedStats.removeAt(r);
    for(int i=r;i<conditions[cur].affectedStats.size();i++)
    {
        ui->affected_stats_list->item(i)->setText(conditions[cur].affectedStats[i]);
    }
    delete ui->affected_stats_list->takeItem(conditions[cur].affectedStats.size());
    ui->condition_stat_combo->setCurrentText(ui->affected_stats_list->currentItem()->text());
}

void MainWindow::on_affected_stats_list_currentTextChanged(const QString &currentText)
{
    ui->condition_stat_combo->setCurrentText(currentText);
}

/*SKILL CONTROLLER*/
void MainWindow::initSkills()
{
    skillCounter = 1;

    //Load functions
    ui->skill_functions_combo->blockSignals(true);
    QFile f("data/functions.csv");
    QList<QByteArray> data;
    if(!f.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not find functions.csv");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QCoreApplication::exit(-1);
    }

    while(!f.atEnd())
    {
        data = f.readLine().split(',');
        for(int i=0;i<data.size();i++)
        {
            ui->skill_functions_combo->addItem(data[i]);
        }
    }
    ui->skill_functions_combo->blockSignals(false);
    //Load dynamics
    ui->skill_dynamic_combo->blockSignals(true);
    QFile df("data/dynamics.csv");
    if(!df.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not find dynamics.csv");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QCoreApplication::exit(-1);
    }

    while(!df.atEnd())
    {
        data = df.readLine().split(',');
        for(int i=0;i<data.size();i++)
        {
            ui->skill_dynamic_combo->addItem(data[i]);
        }
    }
    ui->skill_dynamic_combo->blockSignals(false);
    //Load events
    ui->skill_event->blockSignals(true);
    QFile ef("data/events.csv");
    if(!ef.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not find events.csv");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QCoreApplication::exit(-1);
    }

    while(!ef.atEnd())
    {
        data = ef.readLine().split(',');
        for(int i=0;i<data.size();i++)
        {
            ui->skill_event->addItem(data[i]);
        }
    }
    ui->skill_event->blockSignals(false);
    //Load ranges
    ui->skill_range->blockSignals(true);
    ui->skill_targetrange->blockSignals(true);
    QFile rf("data/ranges.csv");
    if(!rf.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not find ranges.csv");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QCoreApplication::exit(-1);
    }

    while(!rf.atEnd())
    {
        data = rf.readLine().split(',');
        for(int i=0;i<data.size();i++)
        {
            ui->skill_range->addItem(data[i]);
            ui->skill_targetrange->addItem(data[i]);
        }
    }
    ui->skill_range->blockSignals(false);
    ui->skill_targetrange->blockSignals(false);
    //Load layers
    ui->skill_layer->blockSignals(true);
    QFile lf("data/layers.csv");
    if(!lf.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not find layers.csv");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QCoreApplication::exit(-1);
    }

    while(!lf.atEnd())
    {
        data = lf.readLine().split(',');
        for(int i=0;i<data.size();i++)
        {
            ui->skill_layer->addItem(data[i]);
        }
    }
    ui->skill_layer->blockSignals(false);
    //Add skill
    if(skills.length()==0)
    {
        addSkill();
        skillCur = 0;
        ui->skill_list->setCurrentRow(skillCur);
        //ui->skill_reliant_stats_list->addItem("---");
        ui->skill_reliant_stats_list->setCurrentRow(0);
        updateConditions();
    }

    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),ui->skill_list);
    connect(shortcut,SIGNAL(activated()),this,SLOT(deleteSkill()));

    QShortcut* insert_shortcut = new QShortcut(QKeySequence(Qt::Key_Insert),ui->skill_list);
    connect(insert_shortcut,SIGNAL(activated()),this,SLOT(addSkill()));
}

void MainWindow::on_skill_add_clicked()
{
   addSkill();
}

void MainWindow::addSkill()
{
    skills.append(Skill());
    ui->skill_list->addItem(skills.back().name);
    QString header = QString::number(skills.length())+": ";
    QString text = skills[skills.length()-1].name;
    ui->skill_list->item(skills.length()-1)->setText(header+text);
    for(int j=skills.length()-1;j>-1;j--)
    {
        if(skills[j].name==text&&j!=skills.length()-1)
        {
            text = text.append(QString::number(skillCounter));
            skills.back().name = text;
            ui->skill_list->item(skills.length()-1)->setText(header+text);
            j=skills.length()-1;
            ++skillCounter;
        }
    }
}

void MainWindow::on_skill_list_currentRowChanged(int currentRow)
{
    skillCur = currentRow;
    Skill s = skills[skillCur];
    QString header = QString::number(skillCur+1)+": ";
    if(!removed)
    {
        ui->skill_list->currentItem()->setText(s.name.prepend(header));
    }
    showSkill();
}

void MainWindow::on_skill_remove_clicked()
{
    deleteSkill();
}

void MainWindow::deleteSkill()
{
    if(skills.length()>1)
    {
        removed = true;
        //null pointers to this object in the classes
        for(int i=classes.length()-1;i>=0;i--)
        {
            //int l = classes[i].skills.length();
            for(int j=classes[i].skills.length()-1;j>=0;j--)
            {
                if(classes[i].skills[j]==&skills[skillCur])
                {
                    classes[i].skills.removeAt(j);
                }
            }
        }
        //null pointers to this object in the enemies
        for(int i=0;i<enemies.length();i++)
        {
            //Remove from active list
            for(int j=0;j<enemies[i].active.length();i++)
            {
                if(enemies[i].active[j]==&skills[skillCur])
                {
                    enemies[i].active.removeAt(j);
                }
            }
            //Remove from passive list
            for(int j=0;j<enemies[i].passive.length();i++)
            {
                if(enemies[i].passive[j]==&skills[j])
                {
                    enemies[i].passive.removeAt(j);
                }
            }
        }
        skills.removeAt(skillCur);
        if(skillCur==skills.size())
        {
            skillCur-=1;
        }
        //ui->skill_name_edit->setText(skills[skillCur].name);
        for(int i=skillCur;i<skills.size();i++)
        {
            QString header = QString::number(i+1)+": ";
            ui->skill_list->item(i)->setText(header+skills[i].name);
        }
        showSkill();
        delete ui->skill_list->takeItem(skills.size());
        removed = false;
    }
}

void MainWindow::removeSkills()
{
    skills[0].functions.clear();
    skills[0].dynamics.clear();
    skills[0].stats.clear();
    ui->skill_list->setCurrentRow(0);
    int l = skills.length();

    for(int i=l-1;i>0;i--)
    {
        actors.removeAt(i);
        delete ui->skill_list->takeItem(i);
    }
}

void MainWindow::on_skill_type_currentIndexChanged(int index)
{
    //skills[skillCur].stats.clear();
    skills[skillCur].type = ui->skill_type->currentText();
    if(skills[skillCur].type=="Passive"||skills[skillCur].type=="Event Passive")
    {
        //Check if this skill is in actives
        for(int i=0;i<enemies.length();i++)
        {
            for(int j=0;j<enemies[i].active.length();j++)
            {
                if(enemies[i].active[j]==&skills[skillCur])
                {
                    enemies[i].active.removeAt(j);
                }
            }
        }
    }
    else
    {
        //Check if this skill in in passives
        for(int i=0;i<enemies.length();i++)
        {
            for(int j=0;j<enemies[i].passive.length();j++)
            {
                if(enemies[i].passive[j]==&skills[skillCur])
                {
                    enemies[i].passive.removeAt(j);
                }
            }
        }
    }
    showSkill();
}

void MainWindow::uniqueSkill()
{
    QString text = ui->skill_name_edit->text();
    int i = 0;
    for(int j=0;j<skills.length();j++)
    {
        if(skills[j].name==text&&j!=skillCur)
        {
            text = text.append(QString::number(i));
            skills[skillCur].name = text;
            ui->skill_list->currentItem()->setText(text);
            ui->skill_name_edit->setText(text);
            j=0;
            ++i;
        }
    }
}

void MainWindow::showSkill()
{
    Skill *s = &skills[skillCur];
    ui->skill_name_edit->setText(skills[skillCur].name);
    ui->skill_description_text->setText(skills[skillCur].description);
    ui->skill_type->setCurrentText(s->type);
//    ui->skill_condition->blockSignals(true);
    if(s->condition==nullptr)
        ui->skill_condition->setCurrentIndex(0);
    else
        ui->skill_condition->setCurrentText(s->condition->name);
//    ui->skill_condition->blockSignals(false);
    ui->skill_condition->show();
    ui->skill_condition_label->show();
    if(s->type=="Attack"||s->type=="Heal")
    {
        ui->skill_stat_combo->clear();
        ui->skill_stat_combo->addItem("power");
        ui->skill_stat_combo->addItem("spcost");
        ui->skill_stat_combo->addItem("iattackRange");
        ui->skill_stat_combo->addItem("oattackRange");
        ui->skill_stat_combo->addItem("itargetRange");
        ui->skill_stat_combo->addItem("otargetRange");
        ui->skill_stat_combo->addItem("condition chance");

        //Remove the added stats from passive skills
        for(int i=0;i<s->stats.length();i++)
        {
            QString str = s->stats[i];
            if(str!="attack"&&str!="defence"&&str!="intelligence"&&str!="agility"&&str!="hit")
            {
                s->stats.removeAt(i);
                i-=1;
            }
        }

        ui->skill_reliant_stats_combo->blockSignals(true);
        ui->skill_reliant_stats_combo->clear();
        ui->skill_reliant_stats_combo->addItem("---");
        ui->skill_reliant_stats_combo->addItem("attack");
        ui->skill_reliant_stats_combo->addItem("defence");
        ui->skill_reliant_stats_combo->addItem("intelligence");
        ui->skill_reliant_stats_combo->addItem("agility");
        ui->skill_reliant_stats_combo->addItem("hit");
        ui->skill_reliant_stats_combo->blockSignals(false);

        ui->skill_stat_spin->setValue(s->pow);

        ui->skill_range->setCurrentText(s->range);
        ui->skill_range->show();
        ui->skill_targetrange->setCurrentText(s->targetRange);
        ui->skill_targetrange->show();
        ui->skill_layer->setCurrentText(s->layer);
        ui->skill_layer->show();
        ui->skill_stat_label->show();
        ui->skill_stat_combo->show();
        ui->skill_stat_spin->show();
        ui->skill_boolean->hide();
        ui->skill_event->hide();

        ui->skill_functions->setItemEnabled(0,true);

        ui->skill_functions->setItemText(0,"Dynamics");
        ui->skill_functions->setItemText(1,"Functions");
        ui->skill_functions->setItemText(2,"Reliant Stats");
        ui->skill_dynamic_list->clear();

        for(int i=0;i<s->dynamics.size();i++)
        {
            ui->skill_dynamic_list->addItem(s->dynamics[i].function);
        }
        ui->skill_dynamic_list->addItem("---");
        ui->skill_dynamic_list->setCurrentRow(0);
        ui->skill_functions->setItemEnabled(1,true);
        ui->skill_functions_list->clear();
        for(int i=0;i<s->functions.size();i++)
        {
            ui->skill_functions_list->addItem(s->functions[i].function);
        }
        ui->skill_functions_list->addItem("---");
        ui->skill_functions_list->setCurrentRow(0);
        ui->skill_reliant_stats_list->clear();
        ui->skill_functions->setItemEnabled(2,true);
        ui->skill_functions->setItemText(2,"Reliant Stats");
        for(int i=0;i<s->stats.size();i++)
        {
            ui->skill_reliant_stats_list->addItem(s->stats[i]);
        }
        ui->skill_reliant_stats_list->addItem("---");
        ui->skill_reliant_stats_list->setCurrentRow(0);
    }
    else if(s->type=="Buff"||s->type=="Debuff")
    {
        ui->skill_stat_combo->clear();
        ui->skill_stat_combo->addItem("spcost");
        ui->skill_stat_combo->addItem("iattackRange");
        ui->skill_stat_combo->addItem("oattackRange");
        ui->skill_stat_combo->addItem("itargetRange");
        ui->skill_stat_combo->addItem("otargetRange");
        ui->skill_stat_spin->setValue(s->spcost);

        ui->skill_range->setCurrentText(s->range);
        ui->skill_range->show();
        ui->skill_targetrange->setCurrentText(s->targetRange);
        ui->skill_targetrange->show();
        ui->skill_layer->setCurrentText(s->layer);
        ui->skill_layer->show();
        ui->skill_stat_label->show();
        ui->skill_stat_combo->show();
        ui->skill_stat_spin->show();
        ui->skill_boolean->hide();
        ui->skill_event->hide();

        ui->skill_functions->setItemEnabled(0,false);
        ui->skill_functions->setItemEnabled(1,true);
        ui->skill_functions->setItemEnabled(2,false);

        ui->skill_functions->setItemText(0,"");
        ui->skill_functions->setItemText(1,"Functions");
        ui->skill_functions->setItemText(2,"");
        //ui->skill_reliant_stats_list->clear();
        //ui->skill_dynamic_list->clear();

        ui->skill_functions_list->clear();
        for(int i=0;i<s->functions.size();i++)
        {
            ui->skill_functions_list->addItem(s->functions[i].function);
        }
        ui->skill_functions_list->addItem("---");
        ui->skill_functions_list->setCurrentRow(0);
    }
    else if(s->type=="Passive")
    {
        ui->skill_stat_combo->clear();
        ui->skill_stat_combo->addItem("value");
        ui->skill_stat_spin->setValue(s->pow);
        ui->skill_boolean->show();
        ui->skill_range->hide();
        ui->skill_targetrange->hide();
        ui->skill_layer->hide();
        ui->skill_event->hide();
        ui->skill_condition_label->hide();
        ui->skill_condition->hide();
        ui->skill_boolean->setText("isPercent");
        ui->skill_boolean->setToolTip("Determines wether the skill's value is a percentage or constant");
        ui->skill_boolean->setChecked(s->boolean);

        ui->skill_reliant_stats_combo->blockSignals(true);
        ui->skill_reliant_stats_combo->clear();
        ui->skill_reliant_stats_combo->addItem("---");
        ui->skill_reliant_stats_combo->addItem("attack");
        ui->skill_reliant_stats_combo->addItem("defence");
        ui->skill_reliant_stats_combo->addItem("intelligence");
        ui->skill_reliant_stats_combo->addItem("agility");
        ui->skill_reliant_stats_combo->addItem("hit");
        ui->skill_reliant_stats_combo->addItem("health");
        ui->skill_reliant_stats_combo->addItem("sp");
        ui->skill_reliant_stats_combo->addItem("mp");
        ui->skill_reliant_stats_combo->blockSignals(false);

        ui->skill_functions->setItemEnabled(2,true);
        ui->skill_functions->setItemEnabled(0,false);
        ui->skill_functions->setItemEnabled(1,false);


        ui->skill_functions->setItemText(0,"");
        ui->skill_functions->setItemText(1,"");
        ui->skill_functions->setItemText(2,"Affected Stats");

        ui->skill_reliant_stats_list->clear();
        for(int i=0;i<s->stats.size();i++)
        {
            ui->skill_reliant_stats_list->addItem(s->stats[i]);
        }
        ui->skill_reliant_stats_list->addItem("---");
        ui->skill_reliant_stats_list->setCurrentRow(0);
    }
    else if(s->type=="Event Passive")
    {
        ui->skill_stat_combo->clear();
        ui->skill_stat_combo->addItem("condition chance");
        ui->skill_stat_combo->show();
        ui->skill_range->hide();
        ui->skill_targetrange->hide();
        ui->skill_layer->hide();
        ui->skill_stat_spin->show();
        ui->skill_stat_label->show();
        ui->skill_event->show();
        ui->skill_event->setCurrentText(s->event);
        ui->skill_boolean->show();
        ui->skill_boolean->setText("isSubject");
        ui->skill_boolean->setToolTip("Determines the target, the user(subject) or other(object)");
        ui->skill_boolean->setChecked(s->boolean);
        ui->skill_functions->setItemEnabled(0,false);
        ui->skill_functions->setItemEnabled(1,true);
        ui->skill_functions->setItemEnabled(2,false);
        ui->skill_functions->setItemText(0,"");
        ui->skill_functions->setItemText(1,"Functions");
        ui->skill_functions->setItemText(2,"");

        ui->skill_functions_list->clear();
        for(int i=0;i<s->functions.size();i++)
        {
            ui->skill_functions_list->addItem(s->functions[i].function);
        }
        ui->skill_functions_list->addItem("---");
        ui->skill_functions_list->setCurrentRow(0);
    }
}

void MainWindow::on_skill_name_edit_textChanged(const QString &arg1)
{
    if(skillCur>-1)
    {
        QString header = QString::number(skillCur+1)+": ";
        ui->skill_list->currentItem()->setText(header+arg1);
        this->skills[skillCur].name = arg1;
    }
}

void MainWindow::on_skill_name_edit_editingFinished()
{
    uniqueSkill();
}

void MainWindow::on_skill_description_text_textChanged()
{
    skills[skillCur].description = ui->skill_description_text->document()->toPlainText();
}

void MainWindow::on_skill_boolean_clicked(bool checked)
{
    skills[skillCur].boolean = checked;
}

void MainWindow::on_skill_range_currentIndexChanged(const QString &arg1)
{
    skills[skillCur].range = arg1;
}

void MainWindow::on_skill_layer_currentTextChanged(const QString &arg1)
{
    skills[skillCur].layer = arg1;
}

void MainWindow::on_skill_stat_spin_valueChanged(int arg1)
{
    QString text = ui->skill_stat_combo->currentText();
    Skill* s = &skills[skillCur];
    if(text=="power")
    {
        s->pow = arg1;
    }
    else if(text=="spcost")
    {
        s->spcost = arg1;
    }
    else if(text=="iattackRange")
    {
        s->iattackRange = arg1;
    }
    else if(text=="oattackRange")
    {
        s->oattackRange = arg1;
    }
    else if(text=="itargetRange")
    {
        s->itargetRange = arg1;
    }
    else if(text=="otargetRange")
    {
        s->otargetRange = arg1;
    }
    else if(text=="value")
    {
        s->pow = arg1;
    }
    else if(text=="condition chance")
    {
        s->chance = arg1;
    }
}

void MainWindow::on_skill_stat_combo_currentIndexChanged(const QString &arg1)
{
    Skill* s = &skills[skillCur];
    if(arg1=="power")
    {
        ui->skill_stat_spin->setValue(s->pow);
        ui->skill_stat_label->setToolTip("The effectiveness of the skill");
    }
    else if(arg1=="spcost")
    {
        ui->skill_stat_spin->setValue(s->spcost);
        ui->skill_stat_label->setToolTip("The sp cost of the skill");
    }
    else if(arg1=="iattackRange")
    {
        ui->skill_stat_spin->setValue(s->iattackRange);
        ui->skill_stat_label->setToolTip("The range that the user can't hit from itself");
    }
    else if(arg1=="oattackRange")
    {
        ui->skill_stat_spin->setValue(s->oattackRange);
        ui->skill_stat_label->setToolTip("The range the user can hit from itself");
    }
    else if(arg1=="itargetRange")
    {
        ui->skill_stat_spin->setValue(s->itargetRange);
        ui->skill_stat_label->setToolTip("The range the skill can't hit from its target");
    }
    else if(arg1=="otargetRange")
    {
        ui->skill_stat_spin->setValue(s->otargetRange);
        ui->skill_stat_label->setToolTip("The range the user can hit from its target");
    }
    else if(arg1=="value")
    {
        ui->skill_stat_spin->setValue(s->pow);
        ui->skill_stat_label->setToolTip("The effectiveness of a passive skill");
    }
    else if(arg1=="condition chance")
    {
        ui->skill_stat_spin->setValue(s->chance);
        ui->skill_stat_label->setToolTip("The chance that the condition will be applied to its target");
    }
}

void MainWindow::on_Tabs_currentChanged(int index)
{
    //Skills
    if(index==4)
    {
        updateConditions();
    }
    //Classes
    else if(index==3)
    {
        updateClassSkills();
        updateClassSkillList();
    }
    //Enemies
    else if(index==2)
    {
        updateEnemySkills();
        updateEnemySkillLists();
    }
    //Actors
    else if(index==1)
    {
        updateClasses();
        updateStartingClassCombo();
    }
}

void MainWindow::on_skill_condition_currentIndexChanged(int index)
{
    if(index==-1)
    {
        return;
    }
    else if(index==0&&!conditionUpdate)
    {
        skills[skillCur].condition = nullptr;
    }
    else if(!conditionUpdate)
    {
        skills[skillCur].condition = &conditions[index-1];
    }
}

void MainWindow::updateConditions()
{
    conditionUpdate = true;
    ui->skill_condition->clear();
    ui->skill_condition->addItem("---");
    for(int i=0;i<conditions.length();i++)
    {
        ui->skill_condition->addItem(conditions[i].name);
    }
    if(skills[skillCur].condition!=nullptr)
        ui->skill_condition->setCurrentText(skills[skillCur].condition->name);
    conditionUpdate = false;
}

void MainWindow::on_skill_dynamic_combo_currentTextChanged(const QString &arg1)
{/*

    if(arg1!="---")
    {
        ui->skill_dynamic_list->addItem(arg1);
        skills[skillCur].dynamics.append(Dynamic(arg1));
    }
    else
    {
        skills[skillCur].dynamics.removeAt(dynamicCur);
        for(int i=0;i<skills[skillCur].dynamics.length();i++)
        {
            ui->skill_dynamic_list->item(i)->setText(skills[skillCur].dynamics.at(i).function);
        }
        ui->skill_dynamic_list->addItem("---");
        delete ui->skill_dynamic_list->takeItem(skills[skillCur].dynamics.size());

    }*/
}

void MainWindow::on_skill_dynamic_list_currentRowChanged(int currentRow)
{
    dynamicCur = currentRow;
    if(currentRow!=skills[skillCur].dynamics.size()&&currentRow!=-1&&ui->skill_dynamic_list->currentItem()->text()!="---")
        ui->skill_dynamic_parameter->setText(skills[skillCur].dynamics[currentRow].parameter);
    else
        ui->skill_dynamic_parameter->clear();
}

void MainWindow::on_skill_dynamic_parameter_textEdited(const QString &arg1)
{
    int r = ui->skill_dynamic_list->currentRow();
    if(r!=skills[skillCur].dynamics.size()&&r!=-1)
        skills[skillCur].dynamics[r].parameter = arg1;
}

void MainWindow::on_skill_dynamic_combo_currentIndexChanged(int index)
{
    int r = ui->skill_dynamic_list->currentRow();
    //Check if there's a value in the model
    if(skills[skillCur].dynamics.size()-1<r&&
            ui->skill_dynamic_list->currentItem()->text()=="---"&&
            index!=0)
    {
        skills[skillCur].dynamics.append(Dynamic(ui->skill_dynamic_combo->currentText()));
    }
    //If a null stat gets set to a value, add a new item
    if(ui->skill_dynamic_list->currentItem()->text()=="---"&&index!=0)
    {
        ui->skill_dynamic_list->addItem("---");
    }
    //If a stat gets set to null, remove it from the list
    else if(ui->skill_dynamic_list->currentItem()->text()!="---"&&index==0)
    {
        skills[skillCur].dynamics.removeAt(r);
        for(int i=r;i<skills[skillCur].dynamics.size();i++)
        {
            ui->skill_dynamic_list->item(i)->setText(skills[skillCur].dynamics[i].function);
        }        
        delete ui->skill_dynamic_list->takeItem(skills[skillCur].dynamics.size());
        //Set the text parameter textfield if the currently selected item isn't null
        if(ui->skill_dynamic_list->currentItem()->text()!="---")
            ui->skill_dynamic_parameter->setText(skills[skillCur].dynamics[r].parameter);
        ui->skill_dynamic_combo->setCurrentText(ui->skill_dynamic_list->currentItem()->text());
    }
    if(ui->skill_dynamic_combo->currentText()!="---")
    {
        ui->skill_dynamic_list->currentItem()->setText(ui->skill_dynamic_combo->currentText());
        skills[skillCur].dynamics[r].function = ui->skill_dynamic_combo->currentText();
    }
}

void MainWindow::on_skill_functions_list_currentRowChanged(int currentRow)
{
    if(currentRow!=skills[skillCur].functions.size()&&currentRow!=-1&&ui->skill_functions_list->currentItem()->text()!="---")
    {
        ui->skill_functions_parameter->setText(skills[skillCur].functions[currentRow].parameter);
        ui->skill_function_chance->setText(skills[skillCur].functions[currentRow].chance);
    }
    else
    {
        ui->skill_function_chance->clear();
        ui->skill_functions_parameter->clear();
    }
}

void MainWindow::on_skill_functions_parameter_textEdited(const QString &arg1)
{
    int r = ui->skill_functions_list->currentRow();
    if(r!=skills[skillCur].functions.size()&&r!=-1)
        skills[skillCur].functions[r].parameter = arg1;
}

void MainWindow::on_skill_function_chance_textEdited(const QString &arg1)
{
    int r = ui->skill_functions_list->currentRow();
    if(r!=skills[skillCur].functions.size()&&r!=-1)
        skills[skillCur].functions[r].chance = arg1;
}

void MainWindow::on_skill_functions_combo_currentIndexChanged(int index)
{
    int r = ui->skill_functions_list->currentRow();
    //Check if there's a value in the model
    if(skills[skillCur].functions.size()-1<r&&
            ui->skill_functions_list->currentItem()->text()=="---"&&
            index!=0)
    {
        skills[skillCur].functions.append(Function(""));
    }
    //If a null stat gets set to a value, add a new item.
    if(ui->skill_functions_list->currentItem()->text()=="---"&&index!=0)
    {
        ui->skill_functions_list->addItem("---");
    }
    //If a stat gets set to null, remove it from the list
    else if(ui->skill_functions_list->currentItem()->text()!="---"&&index==0)
    {
        skills[skillCur].functions.removeAt(r);
        for(int i=0;i<skills[skillCur].functions.length();i++)
        {
            ui->skill_functions_list->item(i)->setText(skills[skillCur].functions[i].function);
        }
        delete ui->skill_functions_list->takeItem(skills[skillCur].functions.size());
        if(ui->skill_functions_list->currentItem()->text()!="---")
        {
            ui->skill_functions_parameter->setText(skills[skillCur].functions[r].parameter);
            ui->skill_functions_parameter->setText(skills[skillCur].functions[r].chance);
        }
        ui->skill_functions_combo->setCurrentText(ui->skill_functions_list->currentItem()->text());
    }
    if(ui->skill_functions_combo->currentText()!="---")
    {
        ui->skill_functions_list->currentItem()->setText(ui->skill_functions_combo->currentText());
        skills[skillCur].functions[r].function = ui->skill_functions_combo->currentText();
    }
    //ui->skill_functions_list->currentItem()->setText("Text");
    //skills[ui->skill_list->currentRow()].functions[r].function = ui->skill_functions_combo->currentText();
}

void MainWindow::on_skill_dynamic_list_currentTextChanged(const QString &currentText)
{
    ui->skill_dynamic_combo->setCurrentText(currentText);
}

void MainWindow::on_skill_functions_list_currentTextChanged(const QString &currentText)
{
    ui->skill_functions_combo->setCurrentText(currentText);
}

void MainWindow::on_skill_reliant_stats_list_currentTextChanged(const QString &currentText)
{
    ui->skill_reliant_stats_combo->setCurrentText(currentText);
}

void MainWindow::on_skill_reliant_stats_combo_currentIndexChanged(int index)
{
    int r = ui->skill_reliant_stats_list->currentRow();
    //Check if there's a value in the model
    if(skills[skillCur].stats.size()-1<r&&
            ui->skill_reliant_stats_list->currentItem()->text()=="---"&&
            index!=0)
    {
        skills[skillCur].stats.append("");
    }
    //If a null stat gets set to a value, add a new item.
    if(ui->skill_reliant_stats_list->currentItem()->text()=="---"&&index!=0)
    {
        ui->skill_reliant_stats_list->addItem("---");
    }
    //If a stat gets set to null, remove it from the list
    else if(ui->skill_reliant_stats_list->currentItem()->text()!="---"&&index==0)
    {
        skills[skillCur].stats.removeAt(r);
        for(int i=0;i<skills[skillCur].stats.length();i++)
        {
            ui->skill_reliant_stats_list->item(i)->setText(skills[skillCur].stats[i]);
        }
        delete ui->skill_reliant_stats_list->takeItem(skills[skillCur].stats.size());
        ui->skill_reliant_stats_combo->setCurrentText(ui->skill_reliant_stats_list->currentItem()->text());
    }
    if(ui->skill_reliant_stats_combo->currentText()!="---")
    {
        ui->skill_reliant_stats_list->currentItem()->setText(ui->skill_reliant_stats_combo->currentText());
        skills[skillCur].stats[r] = ui->skill_reliant_stats_combo->currentText();
    }
}

/* CLASS CONTROLLER */
void MainWindow::initClasses()
{
    classCounter = 1;
    if(classes.length()==0)
    {
        addClass();
        ui->class_list->setCurrentRow(0);
        updateClassSkills();
    }
    //new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(deleteCondition());
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),ui->class_list);
    connect(shortcut,SIGNAL(activated()),this,SLOT(deleteClass()));
    QShortcut* insert_shortcut = new QShortcut(QKeySequence(Qt::Key_Insert),ui->class_list);
    connect(insert_shortcut,SIGNAL(activated()),this,SLOT(addClass()));
}

void MainWindow::on_class_add_clicked()
{
    addClass();
}

void MainWindow::addClass()
{
    classes.append(Class());
    ui->class_list->addItem(classes.back().name);
    QString header = QString::number(classes.length())+": ";
    QString text = classes[classes.length()-1].name;
    ui->class_list->item(classes.length()-1)->setText(header+text);
    for(int j=classes.length()-1;j>-1;j--)
    {
        if(classes[j].name==text&&j!=classes.length()-1)
        {
            text = text.append(QString::number(classCounter));
            classes.back().name = text;
            ui->class_list->item(classes.length()-1)->setText(header+text);
            j=classes.length()-1;
            ++classCounter;
        }
    }
}

void MainWindow::deleteClass()
{
    if(classes.length()>1)
    {
        int r = ui->class_list->currentRow();
        removed = true;
        for(int i=0;i<actors.length();i++)
        {
            if(actors[i].classes[classes[r].type]==&classes[r])
                actors[i].classes.remove(classes[r].type);
            for(int j=0;j<actors[i].startingClasses.length();j++)
            {
                if(actors[i].startingClasses[j]==&classes[r])
                    actors[i].startingClasses.removeAt(j);
            }
        }
        classes.removeAt(r);
        if(r==classes.size())
        {
            r-=1;
        }
        ui->class_name_text->setText(classes[r].name);
        for(int i=r;i<classes.size();i++)
        {
            QString header = QString::number(i+1)+": ";
            ui->class_list->item(i)->setText(header+classes[i].name);
        }
        delete ui->class_list->takeItem(classes.size());
        removed = false;
    }
}

void MainWindow::uniqueClass()
{
    int r = ui->class_list->currentRow();
    QString text = ui->class_name_text->text();
    int i = 0;
    for(int j=0;j<classes.length();j++)
    {
        if(classes[j].name==text&&j!=r)
        {
            text = text.append(QString::number(i));
            classes[r].name = text;
            ui->class_list->currentItem()->setText(text);
            ui->class_name_text->setText(text);
            j=0;
            ++i;
        }
    }
}

void MainWindow::on_class_remove_clicked()
{
    deleteClass();
}

void MainWindow::on_class_list_currentRowChanged(int currentRow)
{
    showClass();
}

void MainWindow::showClass()
{
    int r = ui->class_list->currentRow();
    Class *c = &classes[r];
    ui->class_name_text->setText(c->name);
    ui->class_type_combo->setCurrentText(c->type);
    updateClassSkillList();
}

void MainWindow::updateClassSkillList()
{
    ui->class_skills_list->blockSignals(true);
    int r = ui->class_list->currentRow();
    Class *c = &classes[r];
    ui->class_skills_list->clear();
    for(int i=0;i<c->skills.length();i++)
    {
        ui->class_skills_list->addItem(c->skills[i]->name);
    }
    ui->class_skills_list->addItem("---");
    ui->class_skills_list->setCurrentRow(0);
    ui->class_skills_list->blockSignals(false);
}

//Removes all classes after the first element, and nulls pointers, used for loading new files
void MainWindow::removeClasses()
{
   classes[0].skills.clear();
   ui->class_list->setCurrentRow(0);
   int l = classes.length();

   for(int i=l-1;i>0;i--)
   {
       classes.removeAt(i);
       delete ui->class_list->takeItem(i);
   }
}

void MainWindow::on_class_type_combo_currentIndexChanged(const QString &arg1)
{
    int r = ui->class_list->currentRow();
    classes[r].type = arg1;
    for(int i=0;i<actors.length();i++)
    {
        if(actors[i].classes.contains(arg1))
        {
            if(actors[i].classes[arg1]==&classes[r])
                actors[i].classes.remove(arg1);
        }
        for(int j=0;j<actors[i].startingClasses.length();j++)
        {
            if(actors[i].startingClasses[j]->type==arg1)
            {
                actors[i].startingClasses.removeAt(j);
            }
        }
    }
}
//Refreshes Class skill combobox
void MainWindow::updateClassSkills()
{
    int r = ui->class_list->currentRow();
    ClassSkillUpdate = true;
    ui->class_skills_combo->clear();
    ui->class_skills_combo->addItem("---");
    for(int i=0;i<skills.length();i++)
    {
        ui->class_skills_combo->addItem(skills[i].name);
    }
    if(classes[r].skills.length()>0)
        ui->class_skills_combo->setCurrentText(classes[r].skills[ui->class_skills_list->currentRow()]->name);
    //ui->skill_condition->setCurrentText(skills[skillCur].condition->name);
    ClassSkillUpdate = false;
}

void MainWindow::on_class_name_text_editingFinished()
{
    uniqueClass();
}

void MainWindow::on_class_name_text_textChanged(const QString &arg1)
{
    int r = ui->class_list->currentRow();
    if(r>-1)
    {
        QString header = QString::number(r+1)+": ";
        ui->class_list->currentItem()->setText(header+arg1);
        this->classes[r].name = arg1;
    }
}

void MainWindow::on_class_skills_combo_currentIndexChanged(int index)
{
    if(!ClassSkillUpdate)
    {
        int r = ui->class_list->currentRow();
        int sr = ui->class_skills_list->currentRow();
        //Check if there's a value in the model
        if(classes[r].skills.size()-1<sr&&
                ui->class_skills_list->currentItem()->text()=="---"&&
                index!=0&&classes[r].skills.size()<5)
        {
            classes[r].skills.append(&skills[index-1]);
        }
        //If a null stat gets set to a value, add a new item.
        if(ui->class_skills_list->currentItem()->text()=="---"&&index!=0&&classes[r].skills.size()<5)
        {
            ui->class_skills_list->addItem("---");
            //classes[r].skills[sr] = &skills[index-1];
        }
        //If a stat gets set to null, remove it from the list
        else if(ui->class_skills_list->currentItem()->text()!="---"&&index==0)
        {
            classes[r].skills.removeAt(sr);
            for(int i=0;i<classes[r].skills.length();i++)
            {
                ui->class_skills_list->item(i)->setText(classes[r].skills[i]->name);
            }
            delete ui->class_skills_list->takeItem(classes[r].skills.size());
            ui->class_skills_combo->setCurrentText(ui->class_skills_list->currentItem()->text());
            if(classes[r].skills.size()==4)
            {
                ui->class_skills_list->addItem("---");
            }
        }
        //ui->class_skills_list->currentItem()->setText(ui->class_skills_combo->currentText());
        if(ui->class_skills_combo->currentText()!="---")
        {
            ui->class_skills_list->currentItem()->setText(ui->class_skills_combo->currentText());
           // classes[r].skills[sr] = &skills[index];
        }
    }
}

void MainWindow::on_class_skills_list_currentTextChanged(const QString &currentText)
{
    ui->class_skills_combo->setCurrentText(currentText);
}

/* ENEMY CONTROLLER */
void MainWindow::initEnemies()
{
    enemyCounter = 1;
    if(enemies.length()==0)
    {
        addEnemy();
        ui->enemy_list->setCurrentRow(0);
        updateEnemySkills();
    }
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),ui->enemy_list);
    connect(shortcut,SIGNAL(activated()),this,SLOT(deleteEnemy()));

    QShortcut* insert_shortcut = new QShortcut(QKeySequence(Qt::Key_Insert),ui->enemy_list);
    connect(insert_shortcut,SIGNAL(activated()),this,SLOT(addEnemy()));
}

void MainWindow::addEnemy()
{
    enemies.append(Enemy());
    ui->enemy_list->addItem(enemies.back().name);
    QString header = QString::number(enemies.length())+": ";
    QString text = enemies[enemies.length()-1].name;
    ui->enemy_list->item(enemies.length()-1)->setText(header+text);
    for(int j=enemies.length()-1;j>-1;j--)
    {
        if(enemies[j].name==text&&j!=enemies.length()-1)
        {
            text = text.append(QString::number(enemyCounter));
            enemies.back().name = text;
            ui->enemy_list->item(enemies.length()-1)->setText(header+text);
            j=enemies.length()-1;
            ++enemyCounter;
        }
    }
}

void MainWindow::deleteEnemy()
{
    if(enemies.length()>1)
    {
        int r = ui->enemy_list->currentRow();
        removed = true;
        enemies.removeAt(r);
        if(r==enemies.size())
        {
            r-=1;
        }
        for(int i=r;i<enemies.size();i++)
        {
            QString header = QString::number(i+1)+": ";
            ui->enemy_list->item(i)->setText(header+enemies[i].name);
        }
        showEnemy();
        delete ui->enemy_list->takeItem(enemies.size());
        removed = false;
    }
}

void MainWindow::removeEnemies()
{
    enemies[0].active.clear();
    enemies[0].passive.clear();
    ui->enemy_list->setCurrentRow(0);
    int l = enemies.length();

    for(int i=l-1;i>0;i--)
    {
        enemies.removeAt(i);
        delete ui->enemy_list->takeItem(i);
    }
}

void MainWindow::showEnemy()
{
    int r = ui->enemy_list->currentRow();
    Enemy *e = &enemies[r];
    ui->enemy_name_text->setText(e->name);
    ui->enemy_description_text->setText(e->description);
    ui->enemy_exp_spin->setValue(e->exp);
    ui->enemy_stats_combo->setCurrentText("Attack");
    ui->enemy_stats_spin->setValue(e->attack);
    ui->enemy_misc_color_label->setText("#"+QString::number(e->tint,16));
    ui->enemy_misc_key_line->setText(e->key);
    qDebug() << QString::number(e->tint);
    updateEnemySkillLists();
}

void MainWindow::uniqueEnemy()
{
    int r = ui->enemy_list->currentRow();
    QString text = ui->enemy_name_text->text();
    int i = 0;
    for(int j=0;j<enemies.length();j++)
    {
        if(enemies[j].name==text&&j!=r)
        {
            text = text.append(QString::number(i));
            enemies[r].name = text;
            ui->enemy_list->currentItem()->setText(QString::number(ui->enemy_list->currentRow()+1)+": "+text);
            ui->enemy_name_text->setText(text);
            j=0;
            ++i;
        }
    }
}

void MainWindow::updateEnemySkills()
{
    ClassSkillUpdate = true;
    ui->enemy_active_combo->clear();
    ui->enemy_passive_combo->clear();
    ui->enemy_active_combo->addItem("---");
    ui->enemy_passive_combo->addItem("---");
    for(int i=0;i<skills.length();i++)
    {
        if(skills[i].type!="Passive"&&skills[i].type!="Event Passive")
        {
            ui->enemy_active_combo->addItem(skills[i].name);
        }
        else
        {
            ui->enemy_passive_combo->addItem(skills[i].name);
        }
    }
    ClassSkillUpdate = false;
}

void MainWindow::updateEnemySkillLists()
{
    int r = ui->enemy_list->currentRow();
    Enemy *e = &enemies[r];
    //Active list update
    ui->enemy_active_list->blockSignals(true);
    ui->enemy_active_list->clear();
    for(int i=0;i<e->active.length();i++)
    {
        ui->enemy_active_list->addItem(e->active[i]->name);
    }
    ui->enemy_active_list->addItem("---");
    ui->enemy_active_list->setCurrentRow(0);
    ui->enemy_active_list->blockSignals(false);
    //Passive list update
    ui->enemy_passive_list->blockSignals(true);
    ui->enemy_passive_list->clear();
    for(int i=0;i<e->passive.length();i++)
    {
        ui->enemy_passive_list->addItem(e->passive[i]->name);
    }
    ui->enemy_passive_list->addItem("---");
    ui->enemy_passive_list->setCurrentRow(0);
    ui->enemy_passive_list->blockSignals(false);
}

void MainWindow::on_enemy_stats_combo_currentIndexChanged(const QString &arg1)
{
    Enemy* e = &enemies[ui->enemy_list->currentRow()];
    if(arg1=="Attack")
    {
        ui->enemy_stats_spin->setValue(e->attack);
    }
    else if(arg1=="Defence")
    {
        ui->enemy_stats_spin->setValue(e->defence);
    }
    else if(arg1=="Intelligence")
    {
        ui->enemy_stats_spin->setValue(e->intelligence);
    }
    else if(arg1=="Agility")
    {
        ui->enemy_stats_spin->setValue(e->agility);
    }
    else if(arg1=="Hit")
    {
        ui->enemy_stats_spin->setValue(e->hit);
    }
    else if(arg1=="HP")
    {
        ui->enemy_stats_spin->setValue(e->health);
    }
    else if(arg1=="SP")
    {
        ui->enemy_stats_spin->setValue(e->sp);
    }
    else if(arg1=="MP")
    {
        ui->enemy_stats_spin->setValue(e->mp);
    }
}

void MainWindow::on_enemy_stats_spin_valueChanged(int arg1)
{
    QString text = ui->enemy_stats_combo->currentText();
    Enemy* e = &enemies[ui->enemy_list->currentRow()];
    if(text=="Attack")
    {
        e->attack = arg1;
    }
    else if(text=="Defence")
    {
        e->defence = arg1;
    }
    else if(text=="Intelligence")
    {
        e->intelligence = arg1;
    }
    else if(text=="Agility")
    {
        e->agility = arg1;
    }
    else if(text=="Hit")
    {
        e->hit = arg1;
    }
    else if(text=="HP")
    {
        e->health = arg1;
    }
    else if(text=="SP")
    {
        e->sp = arg1;
    }
    else if(text=="MP")
    {
        e->mp = arg1;
    }
}

void MainWindow::on_enemy_name_text_textEdited(const QString &arg1)
{
    int r = ui->enemy_list->currentRow();
    QString header = QString::number(r+1)+": ";
    ui->enemy_list->currentItem()->setText(header+arg1);
    uniqueEnemy();
    enemies[r].name = arg1;
}

void MainWindow::on_enemy_description_text_textChanged()
{
    int r = ui->enemy_list->currentRow();
    enemies[r].description = ui->enemy_description_text->document()->toPlainText();
}

void MainWindow::on_enemy_exp_spin_valueChanged(int arg1)
{
    int r = ui->enemy_list->currentRow();
    enemies[r].exp = arg1;
}

void MainWindow::on_enemy_add_clicked()
{
    addEnemy();
}

void MainWindow::on_enemy_remove_clicked()
{
    deleteEnemy();
}

void MainWindow::on_enemy_list_currentRowChanged(int currentRow)
{
    showEnemy();
}


void MainWindow::on_enemy_active_combo_currentIndexChanged(int index)
{
    if(!ClassSkillUpdate)
    {
        int r = ui->enemy_list->currentRow();
        int sr = ui->enemy_active_list->currentRow();
        //Check if there's a value in the model
        if(enemies[r].active.size()-1<sr&&
                ui->enemy_active_list->currentItem()->text()=="---"&&
                index!=0)
        {
            for(int i=0;i<skills.length();i++)
            {
                if(skills[i].name==ui->enemy_active_combo->currentText())
                {
                    enemies[r].active.append(&skills[i]);
                }
            }
        }
        //If a null stat gets set to a value, add a new item.
        if(ui->enemy_active_list->currentItem()->text()=="---"&&index!=0)
        {
            ui->enemy_active_list->addItem("---");
        }
        //If a stat gets set to null, remove it from the list
        else if(ui->enemy_active_list->currentItem()->text()!="---"&&index==0)
        {
            enemies[r].active.removeAt(sr);
            for(int i=r;i<enemies[r].active.size();i++)
            {
                ui->enemy_active_list->item(i)->setText(enemies[r].active[i]->name);
            }
            delete ui->enemy_active_list->takeItem(enemies[r].active.size());
            ui->enemy_active_combo->setCurrentText(ui->enemy_active_list->currentItem()->text());
        }
        //ui->enemy_active_list->currentItem()->setText(ui->enemy_active_combo->currentText());
        if(ui->enemy_active_combo->currentText()!="---")
        {
            ui->enemy_active_list->currentItem()->setText(ui->enemy_active_combo->currentText());
           // classes[r].skills[sr] = &skills[index];
        }
    }
}

void MainWindow::on_enemy_active_list_currentTextChanged(const QString &currentText)
{
    ui->enemy_active_combo->setCurrentText(currentText);
}

void MainWindow::on_enemy_passive_combo_currentIndexChanged(int index)
{
    if(!ClassSkillUpdate)
    {
        int r = ui->enemy_list->currentRow();
        int sr = ui->enemy_passive_list->currentRow();
        //Check if there's a value in the model
        if(enemies[r].passive.size()-1<sr&&
                ui->enemy_passive_list->currentItem()->text()=="---"&&
                index!=0)
        {
            for(int i=0;i<skills.length();i++)
            {
                if(skills[i].name==ui->enemy_passive_combo->currentText())
                {
                    enemies[r].passive.append(&skills[i]);
                }
            }
        }
        //If a null stat gets set to a value, add a new item.
        if(ui->enemy_passive_list->currentItem()->text()=="---"&&index!=0)
        {
            ui->enemy_passive_list->addItem("---");
        }
        //If a stat gets set to null, remove it from the list
        else if(ui->enemy_passive_list->currentItem()->text()!="---"&&index==0)
        {
            enemies[r].passive.removeAt(sr);
            for(int i=r;i<enemies[r].passive.size();i++)
            {
                ui->enemy_passive_list->item(i)->setText(enemies[r].passive[i]->name);
            }
            delete ui->enemy_passive_list->takeItem(enemies[r].passive.size());
            ui->enemy_passive_combo->setCurrentText(ui->enemy_passive_list->currentItem()->text());
        }
        //ui->enemy_active_list->currentItem()->setText(ui->enemy_active_combo->currentText());
        if(ui->enemy_passive_combo->currentText()!="---")
        {
            ui->enemy_passive_list->currentItem()->setText(ui->enemy_passive_combo->currentText());
           // classes[r].skills[sr] = &skills[index];
        }
    }
}

void MainWindow::on_enemy_passive_list_currentTextChanged(const QString &currentText)
{
    ui->enemy_passive_combo->setCurrentText(currentText);
}

/*ACTOR CONTROLLER*/
void MainWindow::initActors()
{
    actorCounter = 1;

    QFile f("data/weapons.csv");
    QList<QByteArray> data;
    if(!f.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not find weapons.csv");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QCoreApplication::exit(-1);
    }

    ui->actor_weapon_combo->blockSignals(true);
    while(!f.atEnd())
    {
        data = f.readLine().split(',');
        for(int i=0;i<data.size();i++)
        {
            ui->actor_weapon_combo->addItem(data[i]);
        }
    }
    ui->actor_weapon_combo->blockSignals(false);

    QFile uf("data/uniforms.csv");
    if(!uf.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not find uniforms.csv");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QCoreApplication::exit(-1);
    }

    ui->actor_uniform_combo->blockSignals(true);
    while(!uf.atEnd())
    {
        data = uf.readLine().split(',');
        for(int i=0;i<data.size();i++)
        {
            ui->actor_uniform_combo->addItem(data[i]);
        }
    }
    ui->actor_uniform_combo->blockSignals(false);

    QFile idf("data/ids.csv");
    if(!idf.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not find ids.csv");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QCoreApplication::exit(-1);
    }

    ui->actor_id_combo->blockSignals(true);
    while(!idf.atEnd())
    {
        data = idf.readLine().split(',');
        for(int i=0;i<data.size();i++)
        {
            ui->actor_id_combo->addItem(data[i]);
        }
    }
    ui->actor_id_combo->blockSignals(false);

    if(actors.length()==0)
    {
        addActor();
        ui->actor_starting_class_combo->blockSignals(true);
        ui->actor_class_list->setCurrentRow(0);
        ui->actor_list->setCurrentRow(0);
        ui->actor_starting_class_combo->blockSignals(false);
        ui->actor_starting_class_list->setCurrentRow(0);
    }

    showActor();
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete),ui->actor_list);
    connect(shortcut,SIGNAL(activated()),this,SLOT(deleteActor()));

    QShortcut* insert_shortcut = new QShortcut(QKeySequence(Qt::Key_Insert),ui->actor_list);
    connect(insert_shortcut,SIGNAL(activated()),this,SLOT(addActor()));
}

void MainWindow::addActor()
{
    actors.append(Actor());
    ui->actor_list->addItem(actors.back().name);
    QString header = QString::number(actors.length())+": ";
    QString text = actors[actors.length()-1].name;
    ui->actor_list->item(actors.length()-1)->setText(header+text);
    for(int j=actors.length()-1;j>-1;j--)
    {
        if(actors[j].name==text&&j!=actors.length()-1)
        {
            text = text.append(QString::number(actorCounter));
            actors.back().name = text;
            ui->actor_list->item(actors.length()-1)->setText(header+text);
            j=actors.length()-1;
            ++actorCounter;
        }
    }
}

void MainWindow::deleteActor()
{
    if(actors.length()>1)
    {
        int r = ui->actor_list->currentRow();
        removed = true;
        actors.removeAt(r);
        if(r==actors.size())
        {
            r-=1;
        }
        for(int i=r;i<actors.size();i++)
        {
            QString header = QString::number(i+1)+": ";
            ui->actor_list->item(i)->setText(header+actors[i].name);
        }
        showActor();
        delete ui->actor_list->takeItem(actors.size());
        removed = false;
    }
}

void MainWindow::removeActors()
{
    actors[0].classes.clear();
    actors[0].startingClasses.clear();
    ui->actor_list->setCurrentRow(0);
    int l = actors.length();

    for(int i=l-1;i>0;i--)
    {
        actors.removeAt(i);
        delete ui->actor_list->takeItem(i);
    }
}

void MainWindow::showActor()
{
    int r = ui->actor_list->currentRow();
    Actor *a = &actors[r];
    ui->actor_name_text->setText(a->name);
    ui->actor_description_text->setText(a->description);
    ui->actor_lvl_spin->setValue(a->lvl);
    ui->actor_min_stat_combo->setCurrentText("Attack");
    ui->actor_min_stat_spin->setValue(a->minAttack);
    ui->actor_max_stat_combo->setCurrentText("Attack");
    ui->actor_max_stat_spin->setValue(a->maxAttack);
    ui->actor_uniform_combo->setCurrentText(a->uniform);
    ui->actor_weapon_combo->setCurrentText(a->weapon);
    ui->actor_id_combo->setCurrentText(a->id);
    ui->actor_misc_key_line->setText(a->key);
    updateClasses();
    ui->actor_starting_class_combo->blockSignals(true);
    updateStartingClassCombo();
    ui->actor_starting_class_combo->blockSignals(false);
}

void MainWindow::updateClasses()
{
    int r = ui->actor_list->currentRow()==-1 ? 0 : ui->actor_list->currentRow();
    Actor *a = &actors[r];
    //Update classes
    ui->actor_class_combo->blockSignals(true);
    ui->actor_class_combo->clear();
    ui->actor_class_combo->addItem("---");

    for(int i=0;i<classes.length();i++)
    {
        //Add classes with matching types to class combo
        if(ui->actor_class_list->currentItem()->text()==classes[i].type)
            ui->actor_class_combo->addItem(classes[i].name);
    }
    QString text = ui->actor_class_list->currentItem()->text();
    if(a->classes.contains(text))
            ui->actor_class_combo->setCurrentText(a->classes[text]->name);
    ui->actor_class_combo->blockSignals(false);
    //Update starting classes
    ui->actor_starting_class_combo->blockSignals(true);
    ui->actor_starting_class_list->blockSignals(true);
    ui->actor_starting_class_list->clear();
    for(int i=0;i<a->startingClasses.length();i++)
    {
        ui->actor_starting_class_list->addItem(a->startingClasses[i]->type);
    }
    ui->actor_starting_class_list->addItem("---");
    ui->actor_starting_class_list->setCurrentRow(0);
    ui->actor_starting_class_list->blockSignals(false);
    ui->actor_starting_class_combo->blockSignals(false);
}

void MainWindow::updateStartingClassCombo()
{
    int r = ui->actor_list->currentRow();
    Actor *a = &actors[r];
    ui->actor_starting_class_combo->blockSignals(true);
    ui->actor_starting_class_combo->clear();
    ui->actor_starting_class_combo->addItem("---");
    foreach(const QString k, a->classes.keys())
    {
//        for(int i=0;i<a->startingClasses.length();i++)
//        {
            ui->actor_starting_class_combo->addItem(k);
//        }
    }
    ui->actor_starting_class_combo->setCurrentText(ui->actor_starting_class_list->currentItem()->text());
    ui->actor_starting_class_combo->blockSignals(false);
}

void MainWindow::uniqueActor()
{
    int r = ui->actor_list->currentRow();
    QString text = ui->actor_name_text->text();
    int i = 0;
    for(int j=0;j<actors.length();j++)
    {
        if(actors[j].name==text&&j!=r)
        {
            text = text.append(QString::number(i));
            actors[r].name = text;
            ui->actor_list->currentItem()->setText(QString::number(ui->actor_list->currentRow()+1)+": "+text);
            ui->actor_name_text->setText(text);
            j=0;
            ++i;
        }
    }
}

void MainWindow::on_actor_add_clicked()
{
    addActor();
}

void MainWindow::on_actor_remove_clicked()
{
    deleteActor();
}

void MainWindow::on_actor_min_stat_combo_currentIndexChanged(const QString &arg1)
{
    Actor* a = &actors[ui->actor_list->currentRow()];
    if(arg1=="Attack")
    {
        ui->actor_min_stat_spin->setValue(a->minAttack);
    }
    else if(arg1=="Defence")
    {
        ui->actor_min_stat_spin->setValue(a->minDefence);
    }
    else if(arg1=="Intelligence")
    {
        ui->actor_min_stat_spin->setValue(a->minIntelligence);
    }
    else if(arg1=="Agility")
    {
        ui->actor_min_stat_spin->setValue(a->minAgility);
    }
    else if(arg1=="Hit")
    {
        ui->actor_min_stat_spin->setValue(a->minHit);
    }
    else if(arg1=="HP")
    {
        ui->actor_min_stat_spin->setValue(a->minHealth);
    }
    else if(arg1=="SP")
    {
        ui->actor_min_stat_spin->setValue(a->minSp);
    }
    else if(arg1=="MP")
    {
        ui->actor_min_stat_spin->setValue(a->minMp);
    }
}

void MainWindow::on_actor_min_stat_spin_valueChanged(int arg1)
{
    QString text = ui->actor_min_stat_combo->currentText();
    Actor* a = &actors[ui->actor_list->currentRow()];
    if(text=="Attack")
    {
        a->minAttack = arg1;
    }
    else if(text=="Defence")
    {
        a->minDefence = arg1;
    }
    else if(text=="Intelligence")
    {
        a->minIntelligence = arg1;
    }
    else if(text=="Agility")
    {
        a->minAgility = arg1;
    }
    else if(text=="Hit")
    {
        a->minHit = arg1;
    }
    else if(text=="HP")
    {
        a->minHealth = arg1;
    }
    else if(text=="SP")
    {
        a->minSp = arg1;
    }
    else if(text=="MP")
    {
        a->minMp = arg1;
    }
}

void MainWindow::on_actor_max_stat_combo_currentIndexChanged(const QString &arg1)
{
    Actor* a = &actors[ui->actor_list->currentRow()];
    if(arg1=="Attack")
    {
        ui->actor_max_stat_spin->setValue(a->maxAttack);
    }
    else if(arg1=="Defence")
    {
        ui->actor_max_stat_spin->setValue(a->maxDefence);
    }
    else if(arg1=="Intelligence")
    {
        ui->actor_max_stat_spin->setValue(a->maxIntelligence);
    }
    else if(arg1=="Agility")
    {
        ui->actor_max_stat_spin->setValue(a->maxAgility);
    }
    else if(arg1=="Hit")
    {
        ui->actor_max_stat_spin->setValue(a->maxHit);
    }
    else if(arg1=="HP")
    {
        ui->actor_max_stat_spin->setValue(a->maxHealth);
    }
    else if(arg1=="SP")
    {
        ui->actor_max_stat_spin->setValue(a->maxSp);
    }
    else if(arg1=="MP")
    {
        ui->actor_max_stat_spin->setValue(a->maxMp);
    }
}

void MainWindow::on_actor_max_stat_spin_valueChanged(int arg1)
{
    QString text = ui->actor_max_stat_combo->currentText();
    Actor* a = &actors[ui->actor_list->currentRow()];
    if(text=="Attack")
    {
        a->maxAttack = arg1;
    }
    else if(text=="Defence")
    {
        a->maxDefence = arg1;
    }
    else if(text=="Intelligence")
    {
        a->maxIntelligence = arg1;
    }
    else if(text=="Agility")
    {
        a->maxAgility = arg1;
    }
    else if(text=="Hit")
    {
        a->maxHit = arg1;
    }
    else if(text=="HP")
    {
        a->maxHealth = arg1;
    }
    else if(text=="SP")
    {
        a->maxSp = arg1;
    }
    else if(text=="MP")
    {
        a->maxMp = arg1;
    }
}

void MainWindow::on_actor_lvl_spin_valueChanged(int arg1)
{
    actors[ui->actor_list->currentRow()].lvl = arg1;
}

void MainWindow::on_actor_name_text_textEdited(const QString &arg1)
{
    int r = ui->actor_list->currentRow();
    QString header = QString::number(r+1)+": ";
    ui->actor_list->currentItem()->setText(header+arg1);
    actors[r].name = arg1;
}

void MainWindow::on_actor_name_text_editingFinished()
{
    uniqueActor();
}

void MainWindow::on_actor_description_text_textChanged()
{
    int r = ui->actor_list->currentRow();
    actors[r].description = ui->actor_description_text->document()->toPlainText();
}

void MainWindow::on_actor_class_combo_currentIndexChanged(int index)
{
    if(!ClassSkillUpdate)
    {
        int r = ui->actor_list->currentRow()==-1 ? 0 : ui->actor_list->currentRow();
        int sr = ui->actor_class_list->currentRow();

        QString text = ui->actor_class_list->currentItem()->text();
        //Check if there's a value in the model
        /*if(actors[r].classes.size()-1<sr&&
                ui->actor_class_list->currentItem()->text()=="---"&&
                index!=0)
        {
            for(int i=0;i<classes.length();i++)
            {
                if(classes[i].name==ui->actor_class_combo->currentText())
                {
                    actors[r].classes.insert(ui->actor_class_list->currentItem()->text(),&classes[i]);
                }
            }
        }*/
        //If a null stat gets set to a value, add a new item.
        if(index!=0)
        {
            if(actors[r].classes.contains(text))
                actors[r].classes.remove(text);
            for(int i=0;i<classes.length();i++)
            {
                if(classes[i].name==ui->actor_class_combo->currentText())
                {
                    actors[r].classes.insert(text,&classes[i]);
                    updateStartingClassCombo();
                }
            }
        }

        //If a stat gets set to null, remove it from the list
        else if(index==0)
        {
            actors[r].classes.remove(text);
            updateStartingClassCombo();
        }
    }
}

void MainWindow::on_actor_class_list_currentTextChanged(const QString &currentText)
{
    int r = ui->actor_list->currentRow()==-1 ? 0 : ui->actor_list->currentRow();

    updateClasses();
    ui->actor_class_combo->blockSignals(true);
    if(actors[r].classes.contains(currentText))
        ui->actor_class_combo->setCurrentText(actors[r].classes[currentText]->name);

    ui->actor_class_combo->blockSignals(false);
    /*for(int i=0;i<classes.length();i++)
    {
        if(&classes[i]==actors[r].classes[currentText])
        {
            ui->actor_class_combo->setCurrentText(classes[i].name);
        }
    }*/
}

bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    if(obj == ui->actor_class_combo)
    {
        return true;
    }
    return false;
}

void MainWindow::on_actor_starting_class_combo_currentIndexChanged(int index)
{
    if(!ClassSkillUpdate)
    {
        int r = ui->actor_list->currentRow();
        int sr = ui->actor_starting_class_list->currentRow();
        //Check if there's a value in the model
        if(actors[r].startingClasses.size()-1<sr&&
                ui->actor_starting_class_list->currentItem()->text()=="---"&&
                index!=0&&
                actors[r].startingClasses.size()<3)
        {
            actors[r].startingClasses.append(actors[r].classes[ui->actor_starting_class_combo->currentText()]);
        }
        //If a null stat gets set to a value, add a new item.
        if(ui->actor_starting_class_list->currentItem()->text()=="---"&&index!=0&&actors[r].startingClasses.size()<3)
        {
            ui->actor_starting_class_list->currentItem()->setText(ui->actor_starting_class_combo->currentText());

            ui->actor_starting_class_list->addItem("---");
            updateStartingClassCombo();
        }
        //If a stat gets set to null, remove it from the list
        else if(ui->actor_starting_class_list->currentItem()->text()!="---"&&index==0)
        {
            actors[r].startingClasses.removeAt(sr);
            for(int i=r;i<actors[r].startingClasses.size();i++)
            {
                ui->actor_starting_class_list->item(i)->setText(actors[r].startingClasses[i]->type);
            }
            delete ui->actor_starting_class_list->takeItem(actors[r].startingClasses.size());
            if(actors[r].startingClasses.size()==2)
            {
                ui->actor_starting_class_list->addItem("---");
            }
            updateStartingClassCombo();
            ui->actor_starting_class_combo->setCurrentText(ui->actor_starting_class_list->currentItem()->text());
        }
        //ui->enemy_active_list->currentItem()->setText(ui->enemy_active_combo->currentText());
        if(ui->actor_starting_class_combo->currentText()!="---")
        {
            ui->actor_starting_class_list->currentItem()->setText(ui->actor_starting_class_combo->currentText());
           // classes[r].skills[sr] = &skills[index];
        }
    }
}
/*GLOBAL CONTROLLER*/
void MainWindow::initGlobal()
{
    global = Global();
    for(int i=0;i<ui->global_class_description_list->count();i++)
    {
        global.classDescriptions.insert(ui->global_class_description_list->item(i)->text(),"");
    }
    showGlobal();
}

void MainWindow::showGlobal()
{
    ui->global_variable_combo->setCurrentText("Damage Variance");
    ui->global_variable_spin->setValue(global.variance);
    ui->global_variable_spin->setMaximum(25);
    ui->global_class_description_list->setCurrentRow(0);
    ui->global_class_description_text->setText(global.classDescriptions.value(ui->global_class_description_list->currentItem()->text(),""));
}

void MainWindow::setSpinValue(int v)
{
    if(ui->global_variable_spin->maximum()<v)
        ui->global_variable_spin->setMaximum(v);
    ui->global_variable_spin->setValue(v);
}

void MainWindow::on_global_variable_combo_currentIndexChanged(const QString &arg1)
{
    if(arg1=="Damage Variance")
    {
        setSpinValue(global.variance);
        ui->global_variable_spin->setMaximum(25);

    }
    else if(arg1=="Starting Money")
    {
        setSpinValue(global.startingMoney);
        ui->global_variable_spin->setMaximum(1000);

    }
    else if(arg1=="Max Experience")
    {
        setSpinValue(global.maxXp);
        ui->global_variable_spin->setMaximum(5000);

    }
    else if(arg1=="Max Level")
    {
        setSpinValue(global.maxLvl);
        ui->global_variable_spin->setMaximum(99);

    }
    else if(arg1=="Max Class Experience")
    {
        setSpinValue(global.maxClassXp);
        ui->global_variable_spin->setMaximum(500);

    }
}

void MainWindow::on_global_variable_spin_valueChanged(int arg1)
{
    QString text = ui->global_variable_combo->currentText();
    if(text=="Damage Variance")
    {
        global.variance = arg1;
    }
    else if(text=="Starting Money")
    {
        global.startingMoney = arg1;
    }
    else if(text=="Max Experience")
    {
        global.maxXp = arg1;
    }
    else if(text=="Max Level")
    {
        global.maxLvl = arg1;
    }
    else if(text=="Max Class Experience")
    {
        global.maxClassXp = arg1;
    }
}

void MainWindow::on_global_class_description_list_currentRowChanged(int currentRow)
{
    ui->global_class_description_text->setText(global.classDescriptions.value(ui->global_class_description_list->currentItem()->text(),""));
}


void MainWindow::on_global_class_description_text_textChanged()
{
    global.classDescriptions[ui->global_class_description_list->currentItem()->text()] = ui->global_class_description_text->document()->toPlainText();
}

void MainWindow::on_actor_weapon_combo_currentTextChanged(const QString &arg1)
{
    Actor* a = &actors[ui->actor_list->currentRow()];
    a->weapon = arg1!="---" ? arg1+"-"+QString::number(a->lvl) : "---";
}

void MainWindow::on_actor_uniform_combo_currentTextChanged(const QString &arg1)
{
    Actor* a = &actors[ui->actor_list->currentRow()];
    a->uniform = arg1!="---" ? arg1+"-"+QString::number(a->lvl) : "---";
}

void MainWindow::on_actor_id_combo_currentTextChanged(const QString &arg1)
{
    Actor* a = &actors[ui->actor_list->currentRow()];
    a->id = arg1!="---" ? arg1+"-"+QString::number(a->lvl) : "---";
}

void MainWindow::on_actor_list_currentRowChanged(int currentRow)
{
    showActor();
}


void MainWindow::on_actor_starting_class_list_currentRowChanged(int currentRow)
{
    int r = ui->actor_list->currentRow();
    ui->actor_starting_class_combo->blockSignals(true);
    if(actors[r].startingClasses.size()!=-1&&currentRow!=actors[r].startingClasses.size())
        ui->actor_starting_class_combo->setCurrentText(actors[r].startingClasses[currentRow]->type);
    else
        ui->actor_starting_class_combo->setCurrentIndex(0);
    ui->actor_starting_class_combo->blockSignals(false);
}

void MainWindow::on_enemy_misc_color_button_clicked()
{
    QColor color = QColorDialog::getColor(QColor(),ui->enemy_misc_tool,"Choose a Tint value");
    if(color.isValid()){
        enemies[ui->enemy_list->currentRow()].tint = (color.red()<<16) + (color.green()<<8) + color.blue();
        ui->enemy_misc_color_label->setText(color.name());
    }
}

void MainWindow::on_enemy_misc_key_line_textEdited(const QString &arg1)
{
    enemies[ui->enemy_list->currentRow()].key = arg1;
}

void MainWindow::on_actor_misc_key_line_textEdited(const QString &arg1)
{
    actors[ui->actor_list->currentRow()].key = arg1;
}

void MainWindow::on_skill_targetrange_currentIndexChanged(const QString &arg1)
{
    skills[skillCur].targetRange = arg1;
}

void MainWindow::on_skill_event_currentIndexChanged(const QString &arg1)
{
    skills[skillCur].event = arg1;
}
