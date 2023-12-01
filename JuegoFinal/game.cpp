#include "game.h"
#include "ui_game.h"

game::game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::game)
{


    ui->setupUi(this);
    scene1= new QGraphicsScene();
    ui->graphicsView->setScene(scene1);

    QImage Bg(":/new/prefix1/sprites/FONDO1.png");
    QBrush Bgimg(Bg);
    ui->graphicsView->setBackgroundBrush(Bgimg);
    int sc_factor = 1;
    scene1->setSceneRect(0,0,(ui->graphicsView->width() - 2)/sc_factor,(ui->graphicsView->height() - 2)/sc_factor); //
    ui->graphicsView->scale(sc_factor,sc_factor);

    int char_num = 1;


    pers = new main_character(char_num);//rick 1
    scene1->addItem(pers);

    //creacion enemigos
    enemy1= new enemy(1);
    enemy2= new enemy(2);

    //añade enemigos a la listtt
    enemigos.push_back(enemy1);
    enemigos.push_back(enemy2);

    // añade enemigos a la escena
    scene1->addItem(enemy1);
    scene1->addItem(enemy2);

    timer_colision = new QTimer();
    timer_colision->start(100);

    connect(timer_colision,SIGNAL(timeout()),this,SLOT(colision_enemy_bala()));
    QObject::connect(enemy1,SIGNAL(delete_tammy()),this,SLOT(remove_enemy()));
    QObject::connect(enemy2,SIGNAL(delete_tickets()),this,SLOT(remove_enemy2()));

}


void game::keyPressEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_W && pers->pos().y() >= 0 )
    {

        //obst_collition(e);
        pers->moveup();

        dir= "up";
    }
    if (e->key()==Qt::Key_Space )
    {
        bala= new ammunition(":/new/prefix1/sprites/48.png",pers->pos().x(),pers->pos().y(),dir);
        scene1->addItem(bala);
        balas.append(bala);

    }







    if (e->key()==Qt::Key_A && pers->pos().x() >= 0 )
    {

        //obst_collition(e);
        pers->moveleft();

        dir= "left";
    }
    if (e->key()==Qt::Key_D && pers->pos().x() <= 450 )
    {

        //obst_collition(e);
        pers->moveright();

        dir= "right";
    }
    if (e->key()==Qt::Key_S && pers->pos().y() <= 450 )
    {

        //obst_collition(e);
        pers->movedown();

        dir= "down";
    }

    //obst_collition(e);
}

void game::colision_enemy_bala()
{
    for (int i = 0; i < balas.size(); i++)
    {
        for (int j = 0;j<enemigos.size();j++){
            if (balas[i]->collidesWithItem(enemigos[j],Qt::IntersectsItemBoundingRect))
            {
                enemigos.at(j)->health = enemigos[j]->getHealt() - balas.at(i)->damage;
                if ( enemigos.at(j)->getHealt() == 0){

                    // muere enemigo
                    enemigos.at(j)->dead();
                    enemigos.removeAt(j);
                    j--;

                    //elimina balas
                    scene1->removeItem(balas.at(i));
                    delete (balas[i]);
                    balas.removeAt(i);
                    i--;
                    break;
                }
                else{
                    scene1->removeItem(balas.at(i));
                    balas.removeAt(i);
                    i--;
                    break;
                }



            }
        }
    }
}

void game::remove_enemy2(){
    scene1->removeItem(enemy2);
    delete enemy2;
}



void game::remove_enemy()
{

     scene1->removeItem(enemy1);
      delete enemy1;



}


game::~game()
{
    delete ui;
}
