#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "character.h"
#include "main_character.h"
#include "ammunition.h"
#include "enemy.h"
#include <QKeyEvent>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class game; }
QT_END_NAMESPACE

class game : public QMainWindow
{
    Q_OBJECT

public:
    game(QWidget *parent = nullptr);
    ~game();

    void keyPressEvent(QKeyEvent *e);
    void update_text();

    bool aux1 = false, aux2= false, aux3=false;

    QTimer *timer_colision;
    QTimer *timer_colision_pers;
    QTimer *labels;

    QList <ammunition*> balas;
    QList <enemy*> enemigos;

    /*Establecer niveles*/
    int lvl_num = 0;
    void set_level1();
    void set_level2();

signals:
    void colision();

    void end_level1(int num);

    void main_menu();


private:
    Ui::game *ui;

    /*Escenas*/
    QGraphicsScene *menu;
    QGraphicsScene *choose_char;
    QGraphicsScene *scene1;
    QGraphicsScene *scene2;

    /* Mapa? */
    QGraphicsPixmapItem *level1;

    /* Elementos */
    main_character *pers;
    ammunition *bala;

    /*Establecer la escena de menu*/
    void set_menu();

    /*Enemigos*/
    QString dir;
    enemy *enemy1;
    enemy *enemy2;
    enemy *enemy3;

    /*personaje principal*/
    int char_num;
    bool main_exist = false;

public slots:
    void slot_menu();
    void set_lvl();
    void slot_choose_char();
    void sel_lvl1();
    void sel_lvl2();
    void sel_rick();
    void sel_morty();

    //colisiones level1
    void colision_enemy_bala();
    void colision_character_enemy();

    //eliminar enemigos
    void remove_enemy();
    void remove_enemy2();
    void remove_enemy3();

    //actualizar label de vida
    void update_label();

    void finish_level1(int num);


};

#endif // MAINWINDOW_H

