#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "condition.h"
#include "skill.h"
#include "class.h"
#include "enemy.h"
#include "actor.h"
#include "global.h"

#include <QFile>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum stats{
        attack = 1,
        defence = 2,
        intelligence = 3,
        agility = 4,
        hit = 5,
        health = 6,
        sp = 7,
        mp = 8,
        dr = 9
    };

    explicit MainWindow(QWidget *parent = 0);
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *recentFiles;
    ~MainWindow();

private slots:
    void newf();
    void open();
    void save();
    void saveAs();
    void exportf();
    void readFile();
    void reset();
    bool checkCurrentFile(QString);
    bool eventFilter(QObject*,QEvent*);
    void statDesc();
    void conditionDesc();
    void skillDesc();
    void functionsDesc();
    void classDesc();
    void enemyDesc();
    void actorDesc();
    void UpdateConfig();
    void initConditions();
    void showCondition();
    void addCondition();
    void deleteCondition();
    void removeConditions();
    void on_condition_add_clicked();
    void deleteAffectedStat();
    void initSkills();
    void addSkill();
    void deleteSkill();
    void removeSkills();
    void showSkill();
    void uniqueSkill();
    void updateConditions();
    void initClasses();
    void addClass();
    void removeClasses(); //Used when deserializing
    void deleteClass();
    void uniqueClass();
    void showClass();
    void updateClassSkills();
    void updateClassSkillList();
    void initEnemies();
    void addEnemy();
    void deleteEnemy();
    void uniqueEnemy();
    void removeEnemies();
    void showEnemy();
    void updateEnemySkills();
    void updateEnemySkillLists();
    void initActors();
    void addActor();
    void deleteActor();
    void removeActors();
    void showActor();
    void updateClasses();
    void updateStartingClassCombo();
    void uniqueActor();
    void initGlobal();
    void showGlobal();
    void setSpinValue(int);

    void on_condition_list_currentRowChanged(int currentRow);

    void on_condition_name_textfield_editingFinished();


    void on_condition_name_textfield_textChanged(const QString &arg1);

    void on_condition_max_stacks_spin_valueChanged(int arg1);

    void on_condition_turns_spin_valueChanged(int arg1);

    void on_condition_value_spin_valueChanged(int arg1);

    void on_condition_repeat_clicked(bool checked);

    void on_condition_reset_clicked(bool checked);

    void on_condition_percentage_clicked(bool checked);

    void on_condition_remove_clicked();

    void on_condition_stat_combo_currentIndexChanged(int index);

    void on_affected_stats_list_currentTextChanged(const QString &currentText);

    void on_skill_add_clicked();

    void on_skill_list_currentRowChanged(int currentRow);

    void on_skill_remove_clicked();

    void on_skill_type_currentIndexChanged(int index);

    void on_skill_name_edit_textChanged(const QString &arg1);

    void on_skill_name_edit_editingFinished();

    void on_skill_description_text_textChanged();

    void on_skill_boolean_clicked(bool checked);

    void on_skill_range_currentIndexChanged(const QString &arg1);

    void on_skill_layer_currentTextChanged(const QString &arg1);

    void on_skill_stat_spin_valueChanged(int arg1);

    void on_skill_stat_combo_currentIndexChanged(const QString &arg1);

    void on_Tabs_currentChanged(int index);

    void on_skill_condition_currentIndexChanged(int index);

    void on_skill_dynamic_combo_currentTextChanged(const QString &arg1);

    void on_skill_dynamic_list_currentRowChanged(int currentRow);

    void on_skill_dynamic_parameter_textEdited(const QString &arg1);

    void on_skill_dynamic_combo_currentIndexChanged(int index);

    void on_skill_functions_list_currentRowChanged(int currentRow);

    void on_skill_functions_parameter_textEdited(const QString &arg1);

    void on_skill_function_chance_textEdited(const QString &arg1);

    void on_skill_functions_combo_currentIndexChanged(int index);

    void on_skill_dynamic_list_currentTextChanged(const QString &currentText);

    void on_skill_functions_list_currentTextChanged(const QString &currentText);

    void on_skill_reliant_stats_list_currentTextChanged(const QString &currentText);

    void on_skill_reliant_stats_combo_currentIndexChanged(int index);

    void on_class_add_clicked();

    void on_class_remove_clicked();

    void on_class_list_currentRowChanged(int currentRow);

    void on_class_name_text_editingFinished();

    void on_class_name_text_textChanged(const QString &arg1);

    void on_class_type_combo_currentIndexChanged(const QString &arg1);

    void on_class_skills_combo_currentIndexChanged(int index);

    void on_class_skills_list_currentTextChanged(const QString &currentText);

    void on_enemy_stats_combo_currentIndexChanged(const QString &arg1);

    void on_enemy_stats_spin_valueChanged(int arg1);

    void on_enemy_name_text_textEdited(const QString &arg1);

    void on_enemy_description_text_textChanged();

    void on_enemy_exp_spin_valueChanged(int arg1);

    void on_enemy_add_clicked();

    void on_enemy_remove_clicked();

    void on_enemy_list_currentRowChanged(int currentRow);



    void on_enemy_active_combo_currentIndexChanged(int index);

    void on_enemy_active_list_currentTextChanged(const QString &currentText);

    void on_enemy_passive_combo_currentIndexChanged(int index);

    void on_enemy_passive_list_currentTextChanged(const QString &currentText);

    void on_actor_add_clicked();

    void on_actor_remove_clicked();

    void on_actor_min_stat_combo_currentIndexChanged(const QString &arg1);

    void on_actor_min_stat_spin_valueChanged(int arg1);

    void on_actor_max_stat_combo_currentIndexChanged(const QString &arg1);

    void on_actor_max_stat_spin_valueChanged(int arg1);

    void on_actor_lvl_spin_valueChanged(int arg1);

    void on_actor_name_text_textEdited(const QString &arg1);

    void on_actor_name_text_editingFinished();

    void on_actor_description_text_textChanged();

    void on_actor_class_combo_currentIndexChanged(int index);

    void on_global_variable_combo_currentIndexChanged(const QString &arg1);

    void on_global_variable_spin_valueChanged(int arg1);

    void on_global_class_description_list_currentRowChanged(int currentRow);

    void on_global_class_description_text_textChanged();

    void on_actor_weapon_combo_currentTextChanged(const QString &arg1);

    void on_actor_uniform_combo_currentTextChanged(const QString &arg1);

    void on_actor_id_combo_currentTextChanged(const QString &arg1);

    void on_actor_list_currentRowChanged(int currentRow);

    void on_actor_class_list_currentTextChanged(const QString &currentText);

    void on_actor_starting_class_combo_currentIndexChanged(int index);

    void on_actor_starting_class_list_currentRowChanged(int currentRow);

    void on_enemy_misc_color_button_clicked();

    void on_enemy_misc_key_line_textEdited(const QString &arg1);

    void on_actor_misc_key_line_textEdited(const QString &arg1);

    void on_skill_targetrange_currentIndexChanged(const QString &arg1);

    void on_skill_event_currentIndexChanged(const QString &arg1);

    void resizeEvent(QResizeEvent *e);
private:
    QString savDir;
    QString openDir;
    QFile* currentFile;
    int cur;
    int counter;
    int skillCounter;
    int enemyCounter;
    int actorCounter;
    int skillCur;
    int dynamicCur;
    int classCounter;
    bool removed;//used for checking if an affectedStat was removed
    bool conditionUpdate;//Used for checking if the condition combobox is being updated
    bool ClassSkillUpdate;
    QList<Condition> conditions;
    QList<Skill> skills;
    QList<Class> classes;
    QList<Enemy> enemies;
    QList<Actor> actors;
    Global global;
    Ui::MainWindow *ui;
    void createMenus();
    void uniqueCondition();
    void addRecent(QString recent);

};

#endif // MAINWINDOW_H
