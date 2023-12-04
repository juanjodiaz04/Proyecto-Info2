#include "game.h"
#include "ui_game.h"

game::game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::game)
{


    ui->setupUi(this);
    set_menu();

    /*Cambios de escena*/
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(sel_lvl1()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(sel_lvl2()));

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(slot_choose_char()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(slot_choose_char()));

    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(sel_rick()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(sel_morty()));

    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(set_lvl()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(set_lvl()));


}

void game::set_level1()
{
    /*Elementos invisibles en la escena*/
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);



    /*Elementos visibles en la escena*/
    ui->cant1->setVisible(true);
    ui->cant2->setVisible(true);
    ui->cant3->setVisible(true);
    ui->enemy1->setVisible(true);
    ui->enemy2->setVisible(true);
    ui->enemy3->setVisible(true);
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);


    /*Eliminar escenas anteriores*/
    delete menu;
    delete choose_char;

    scene1 = new QGraphicsScene();
    ui->graphicsView->setScene(scene1);

    QImage Bg(":/new/prefix1/sprites/FONDO1.png");
    QBrush Bgimg(Bg);
    ui->graphicsView->setBackgroundBrush(Bgimg);
    int sc_factor = 1;
    scene1->setSceneRect(0,0,(ui->graphicsView->width() - 2)/sc_factor,(ui->graphicsView->height() - 2)/sc_factor); //
    ui->graphicsView->scale(sc_factor,sc_factor);

    update_text();

    pers = new main_character(char_num); //rick 1
    scene1->addItem(pers);
    main_exist = true;

    //creacion enemigos
    enemy1= new enemy(1,*pers);
    enemy2= new enemy(2,*pers);
    enemy3= new enemy(3,*pers);


    //añade enemigos a la listtt
    enemigos.push_back(enemy1);
    enemigos.push_back(enemy2);
    enemigos.push_back(enemy3);

    // añade enemigos a la escena
    scene1->addItem(enemy1);
    scene1->addItem(enemy2);
    scene1->addItem(enemy3);

    timer_colision = new QTimer();
    timer_colision->start(100);

    timer_colision_pers= new QTimer();
    timer_colision_pers->start(1000);


    connect(timer_colision,SIGNAL(timeout()),this,SLOT(colision_enemy_bala()));
    connect(timer_colision_pers,SIGNAL(timeout()),this,SLOT(colision_character_enemy()));

    //final de levels
    connect(this,SIGNAL(end_level1(int)),this,SLOT(finish_level1(int)));

    // remover enemigos por vida
    QObject::connect(enemy1,SIGNAL(delete_tammy()),this,SLOT(remove_enemy()));
    QObject::connect(enemy2,SIGNAL(delete_tickets()),this,SLOT(remove_enemy2()));
    QObject::connect(enemy3,SIGNAL(delete_story_master()),this,SLOT(remove_enemy3()));
}

void game::set_level2()
{

}

void game::keyPressEvent(QKeyEvent *e)
{
    if (main_exist){
        if (e->key()==Qt::Key_W && pers->pos().y() >= 0 )
        {


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
    }


}

void game::update_text()
{

    ui->label->setStyleSheet("font-weight: bold; font-style: italic; text-decoration: underline; color: white; background-color: black;");
    ui->label_2->setStyleSheet("font-weight: bold; font-style: italic; text-decoration: underline; color: white; background-color: black;");
    ui->enemy1->setStyleSheet("font-weight: bold; font-style: italic; text-decoration: underline; color: white; background-color: black;");
    ui->enemy2->setStyleSheet("font-weight: bold; font-style: italic; text-decoration: underline; color: white; background-color: black;");
    ui->enemy3->setStyleSheet("font-weight: bold; font-style: italic; text-decoration: underline; color: white; background-color: black;");
    ui->cant1->setStyleSheet("font-weight: bold; font-style: italic; text-decoration: underline; color: white; background-color: black;");
    ui->cant2->setStyleSheet("font-weight: bold; font-style: italic; text-decoration: underline; color: white; background-color: black;");
    ui->cant3->setStyleSheet("font-weight: bold; font-style: italic; text-decoration: underline; color: white; background-color: black;");


    ui->label->setText("HEALTH: ");
    ui->enemy1->setText("ENEMIGO 1: ");
    ui->enemy2->setText("ENEMIGO 2: ");
    ui->enemy3->setText("ENEMIGO 3: ");

    labels= new QTimer();
    labels->start(100);

    connect(labels,SIGNAL(timeout()),this,SLOT(update_label()));
}


void game::colision_enemy_bala()
{

    if (enemigos.size()== 0){
        emit end_level1(0);
    }
    else{
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
}

void game::colision_character_enemy()
{
    for (int j = 0 ;j<enemigos.size();j++){
        if(pers->collidesWithItem(enemigos[j],Qt::IntersectsItemBoundingRect)){
            pers->health -= enemigos.at(j)->damage;
            pers->colision();
            if (pers->health<=0){
                timer_colision_pers->stop();
                delete pers;
                main_exist = false;
                emit end_level1(1);


                break;
            }
        }
    }
}

void game::remove_enemy2(){
    scene1->removeItem(enemy2);
    delete enemy2;
}

void game::remove_enemy3()
{
    scene1->removeItem(enemy3);
    delete enemy3;
}

void game::update_label()
{
    ui->label_2->clear();
    ui->cant1->clear();
    ui->cant2->clear();
    ui->cant3->clear();

    QString enemig1 = QString::number(enemy1->health);
    QString enemig2 = QString::number(enemy2->health);
    QString enemig3 = QString::number(enemy3->health);
    QString personaje = QString::number(pers->health);

    if(pers->health != 0){
        ui->label_2->setText(personaje);
    }
    else{
        pers->health = 0;
        ui->label_2->setText("0");
    }

    if(enemy1->health != 0 && aux1== false){

        ui->cant1->setText(enemig1);

    }
    else{

        ui->cant1->setText("0");
        aux1= true;

    }
    if(enemy2->health != 0 && aux2 == false){

        ui->cant2->setText(enemig2);
    }
    else{

        ui->cant2->setText("0");
        aux2= true;

    }
    if(enemy3->health != 0 && aux3 == false){

        ui->cant3->setText(enemig3);

    }
    else{

        ui->cant3->setText("0");
        aux3= true;
    }



}

void game::finish_level1(int num)
{

    if (num == 0){

        ui->label->setVisible(false);
        ui->label_2->setVisible(false);
        ui->enemy1->setVisible(false);
        ui->enemy2->setVisible(false);
        ui->enemy3->setVisible(false);
        ui->cant2->setVisible(false);
        ui->cant3->setVisible(false);
        ui->cant1->setVisible(false);

        delete timer_colision;
        delete pers;

        delete scene1;
        set_menu(); //Finalización del nivel
    }
    else if (num== 1){
        delete timer_colision;
        delete enemy1;
        delete enemy2;
        delete enemy3;

        ui->label->setVisible(false);
        ui->label_2->setVisible(false);
        ui->enemy1->setVisible(false);
        ui->enemy2->setVisible(false);
        ui->enemy3->setVisible(false);
        ui->cant2->setVisible(false);
        ui->cant3->setVisible(false);
        ui->cant1->setVisible(false);



        delete scene1;
        set_menu(); //Finalización del nivel
    }

}

void game::set_menu()
{
    /*Elementos invisibles en la escena*/
    ui->cant1->setVisible(false);
    ui->cant2->setVisible(false);
    ui->cant3->setVisible(false);
    ui->enemy1->setVisible(false);
    ui->enemy2->setVisible(false);
    ui->enemy3->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_4->setVisible(false);


    /*Elementos visibles en la escena*/
    ui->pushButton->setVisible(true);
    ui->pushButton_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label_4->setVisible(true);


    menu = new QGraphicsScene();
    ui->graphicsView->setScene(menu);
    QImage Bg(":/new/prefix1/sprites/menu_BG2.jpg");
    QBrush Bgimg(Bg);
    ui->graphicsView->setBackgroundBrush(Bgimg);
    int sc_factor = 1;
    menu->setSceneRect(0,0,(ui->graphicsView->width() - 2)/sc_factor,(ui->graphicsView->height() - 2)/sc_factor);
    ui->graphicsView->scale(sc_factor,sc_factor);
}



void game::slot_menu()
{
    set_menu();
}

void game::set_lvl()
{
    if (lvl_num == 1)
    {
        set_level1();
    }
    else if (lvl_num == 2)
    {
        set_level2();
    }
}

void game::slot_choose_char()
{
    /*Elementos invisibles en la escena*/
    ui->cant1->setVisible(false);
    ui->cant2->setVisible(false);
    ui->cant3->setVisible(false);
    ui->enemy1->setVisible(false);
    ui->enemy2->setVisible(false);
    ui->enemy3->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);

    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);


    /*Elementos visibles en la escena*/
    ui->pushButton_3->setVisible(true);
    ui->pushButton_4->setVisible(true);


    choose_char = new QGraphicsScene();
    ui->graphicsView->setScene(choose_char);
    QImage Bg(":/new/prefix1/sprites/menu_BG2.jpg");
    QBrush Bgimg(Bg);
    ui->graphicsView->setBackgroundBrush(Bgimg);
    int sc_factor = 1;
    choose_char->setSceneRect(0,0,(ui->graphicsView->width() - 2)/sc_factor,(ui->graphicsView->height() - 2)/sc_factor);
    ui->graphicsView->scale(sc_factor,sc_factor);
}

void game::sel_lvl1()
{
    lvl_num = 1;
}

void game::sel_lvl2()
{
    lvl_num = 2;
}

void game::sel_rick()
{
    char_num = 1;
}

void game::sel_morty()
{
    char_num = 0;
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
