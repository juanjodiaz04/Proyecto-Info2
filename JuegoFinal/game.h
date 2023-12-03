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

     void set_level1();
signals:
    void colision();

    void end_level1(int num);

    void main_menu();


private:
    Ui::game *ui;

    QGraphicsScene *scene1;
    QGraphicsPixmapItem *level1;
    main_character *pers;
    ammunition *bala;


    QString dir;
    enemy *enemy1;
    enemy *enemy2;
    enemy *enemy3;
    bool main_exist = false;

public slots:

    // set levels
    //void set_level1();

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

