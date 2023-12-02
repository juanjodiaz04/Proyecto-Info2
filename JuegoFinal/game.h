#ifndef GAME_H
#define GAME_H

#include <QMainWindow>

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
    void setLevel1();
    void keyPressEvent(QKeyEvent *e);


    QTimer *timer_colision;
    QTimer *timer_colision_pers;

    QList <ammunition*> balas;
    QList <enemy*> enemigos;
signals:
    void colision();


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

public slots:
    void colision_enemy_bala();
    void colision_character_enemy();
    void remove_enemy();
    void remove_enemy2();
    void remove_enemy3();


};

#endif // MAINWINDOW_H

