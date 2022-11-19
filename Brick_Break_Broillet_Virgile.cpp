// The goal is to make a brick break, with different level.
// The first step will be to destroy all the bricks plus the targets that will be present.
// Second, bricks will last longer and some bullets will have special effects.
// Targets will be moving and some balls will damage the player’s brick.

//update 29/04 Modification of the stationary lighter init

//update 06/05/2022 Modified init, trying to color the bricks in black (before they disappear) when the ball hits it.

//update 13/05 functional puzzle, setting menu, rules, a button reload and play.

#include "Grapic.h"
#include <iostream>
#include <cmath>
using namespace std;
using namespace grapic;

const int DIMW = 700;
const int MAX = 100;
const int FRICTION = 2;

struct Complex
{
    float x;
    float y;
};

struct BRIQUE_JEU
{
    Complex Brique;
    bool touche;
};

struct BRIQUE_JEU_IMMOBILE
{
    BRIQUE_JEU Tab[MAX][MAX];
    int nb;
    int nb_brique_tuer;
};

struct BRIQUE_JEU_OBSTACLE
{
    Complex Brique;
};

struct Balle
{
    Complex Position;
    Complex Vitesse;
    Complex Force;
    float masse;
};

Complex make_complex(float x, float y)
{
    Complex c;
    c.x = x;
    c.y = y;
    return c;
};

void remove_complex(Complex A, float H)
{
    A.x = H;
    A.y = H;
}


Complex make_complex_exp(float R, float thetha_deg)
{
    Complex C;
    float thetha_rad = (thetha_deg*M_PI)/180;
    C.x = R*(cos(thetha_rad));
    C.y = R*(sin(thetha_deg));
    return C;
}

Complex operator+(Complex a, Complex b)
{
    Complex c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c ;
};

Complex operator-(Complex a, Complex b)
{
    Complex c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c ;
};

Complex operator*(float lambda, Complex b)
{
    Complex c;
    c.x = lambda * b.x;
    c.y = lambda * b.y;
    return c ;
};

Complex operator*(Complex b, float lambda)
{
    Complex c;
    c.x = b.x * lambda;
    c.y = b.y * lambda;
    return c ;
};

Complex operator*(Complex b, Complex a)
{
    Complex c;
    c.x = a.x * b.x - a.y * b.y;
    c.y = a.x * b.y + a.y * b.x;
    return c ;
};

Complex changement(Complex P, float cx, float cy, float theta_deg)
{
    Complex C = make_complex(cx, cy);
    Complex T = make_complex(1, theta_deg);
    
    return (P-C) * T + C;
};

struct Balle Init_Balle(Complex P, Complex V, float masse)
{
    Complex F = make_complex(0, 0);
    
    Balle Ba;
    Ba.Position = P;
    Ba.Vitesse = V;
    Ba.Force = F;
    Ba.masse = masse;
    
    return Ba;
};

struct Balle Init_Balle_Mouvements(Balle &Ba, BRIQUE_JEU &J)
{
    Ba.Position.x = DIMW/2;
    Ba.Position.y = DIMW-30;
    Ba.Vitesse.x = 14;
    Ba.Vitesse.y = 14;
    Ba.masse = 0.7;
    
    return Ba;
}

void init_brique_jeu(BRIQUE_JEU &J)
{
    J.Brique = make_complex(DIMW/2-30, 10);
}

void init_brique_jeu_obstacle(BRIQUE_JEU_OBSTACLE &BJO)
{
    BJO.Brique.x = rand()%(DIMW-200);
    BJO.Brique.y = rand()%300+200;
}

void init_brique_jeu_immobile(BRIQUE_JEU_IMMOBILE &JIM)
{
    int a = 30 ; int b = 0; int w = 30; int i = 0; int g = 30;
    for (int k=0; k<2 ; k++)
    {
        for (int l=0; l<8-i; l++)
        {
            JIM.Tab[k][l].Brique = make_complex(a, DIMW-b-g);
            JIM.Tab[k][l].touche = false;
            a+=80;
        }
        b=30;
        i=1;
        a=65;
    }
}

void draw_brique_immobile(BRIQUE_JEU_IMMOBILE JIM)
{
    int i = 0;
    for (int k=0; k<2; k++)
    {
        for (int l=0; l<8-i; l++)
        {
            color(255,0,0);
            rectangleFill(JIM.Tab[k][l].Brique.x, JIM.Tab[k][l].Brique.y, JIM.Tab[k][l].Brique.x+70, JIM.Tab[k][l].Brique.y+20);
            color(0,0,255);
            rectangle(JIM.Tab[k][l].Brique.x, JIM.Tab[k][l].Brique.y, JIM.Tab[k][l].Brique.x+70, JIM.Tab[k][l].Brique.y+20);
            
        }
        i++;
    }
}

void draw_brique_obstacle(BRIQUE_JEU_OBSTACLE BJO)
{
    color(0,0,0);
    rectangleFill(BJO.Brique.x, BJO.Brique.y, BJO.Brique.x+140, BJO.Brique.y+20);
}

void update_brique_immobile(BRIQUE_JEU_IMMOBILE &JIM, Balle &Ba, Menu M, BRIQUE_JEU &J, BRIQUE_JEU_OBSTACLE &BJO)
{
    JIM.nb_brique_tuer = 0;
    int i=0;
    for (int k =0; k<2; k++)
    {
        for (int l=0; l<8-i; l++)
        {
            if (Ba.Position.y > JIM.Tab[k][l].Brique.y)
            {
                if (JIM.Tab[k][l].Brique.x <= Ba.Position.x)
                {
                    if (JIM.Tab[k][l].Brique.x+70 >= Ba.Position.x)
                        {
                            JIM.Tab[k][l].touche=true;
                        }
                }
            }
            else
            {
                JIM.Tab[k][l].touche = JIM.Tab[k][l].touche;
            }
            
            if (JIM.Tab[k][l].touche == true)
            {
                color(0,0,0);
                rectangleFill(JIM.Tab[k][l].Brique.x, JIM.Tab[k][l].Brique.y, JIM.Tab[k][l].Brique.x+70, JIM.Tab[k][l].Brique.y+20);
                JIM.Tab[k][l].Brique.x = -2000;
                JIM.Tab[k][l].Brique.y = -2000;
                JIM.nb_brique_tuer++;
            }
            if (JIM.nb_brique_tuer==16) {
                Ba.masse=100000;
                Ba.Vitesse.y=0;
                Ba.Vitesse.x=0;
                fontSize(36);
                color(0,255,0);
                print((DIMW/2)-75, DIMW/2, "YOU WIN");
                menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
                switch (menu_select(M)) {
                    case 1:
                        menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
                        init_brique_jeu(J);
                        Init_Balle_Mouvements(Ba, J);
                        init_brique_jeu_immobile(JIM);
                        break;
                    case 2:
                        winClear();
                        fontSize(30);
                        print(60, DIMW-70, "CECI SONT LES REGLES DU CASSE-BRIQUE :");
                        fontSize(27);
                        print(58, DIMW-130, "Le but du jeu est de casser la totalite des briques.");
                        print(50, DIMW-170, "Il faut faire rebondir la balle sur la raquette du bas,");
                        fontSize(25);
                        print(30, DIMW-210, "Si la balle rebondit a droite de la raquette, elle part a droite.");
                        print(10, DIMW-250, "Si la balle rebondit a gauche de la raquette, elle part a gauche.");
                        print(40, DIMW-290, "Si la balle tombe hors de la raquette, la partie est perdue.");
                        print(70, DIMW-330, "Apres avoir casse 8 briques, une raquette apparait.");
                        print(70, DIMW-370, "Si vous la touchez, la balle rebondit, ou se teleporte.");
                        
                        fontSize(35);
                        print((DIMW/2)-120, DIMW-440, "Bonne chance !");
                        
                        
                        menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
                        break;
                    default:
                        break;
                }
            }
            else if (JIM.nb_brique_tuer==8)
            {
                draw_brique_obstacle(BJO);
            }
        }
    }
}


void Ajout_Force(Balle &Ba, Complex Force)
{
    Ba.Force = Ba.Force + Force;
}

float distance1(float x, float y)
{
    return sqrt(x*x + y*y);
}

void draw_Balle(Balle Ba)
{
    color(0,0,0);
    circleFill(Ba.Position.x, Ba.Position.y, 5);
}

void draw_brique_principale(BRIQUE_JEU J)
{
    color(33, 85, 205);
    rectangleFill(J.Brique.x, J.Brique.y, J.Brique.x+25, J.Brique.y+10);
    color(10,161,221);
    rectangleFill(J.Brique.x+26, J.Brique.y, J.Brique.x+45, J.Brique.y+10);
    color(121,218,232);
    rectangleFill(J.Brique.x+46, J.Brique.y, J.Brique.x+70, J.Brique.y+10);
}

void Colision(Balle &Ba, BRIQUE_JEU &J, Menu M, BRIQUE_JEU_IMMOBILE &BJIM)
{
    if (Ba.Position.x >= DIMW-5)
    {
        Ba.Position.x = DIMW - (Ba.Position.x - DIMW);
        Ba.Vitesse.x = -Ba.Vitesse.x;
    }
    else if (Ba.Position.x < 0)
    {
        Ba.Position.x = DIMW + (Ba.Position.x - DIMW);
        Ba.Vitesse.x = -Ba.Vitesse.x;
    }
    if (Ba.Position.y >= DIMW-5 )
    {
        Ba.Position.y = DIMW - (Ba.Position.y - DIMW);
        Ba.Vitesse.y = -Ba.Vitesse.y;
    }
    else if (Ba.Position.y > DIMW)
    {
        color(255,255,255);
        circleFill(Ba.Position.x, Ba.Position.y, 5);
    }
    if (Ba.Position.y < 11)
    {
        fontSize(36);
        color(255,0,0);
        print((DIMW/2)-100, DIMW/2, "GAME OVER");
        fontSize(17);
        color(0,0,0);
        print(150, (DIMW/2)-255, "Tips? : Si la balle touche la brique obstacle en haut");
        print(130, (DIMW/2)-275, "On a plus de chance de la rattraper en retournant au centre!");
        color(255,255,255);
        rectangleFill(0, 0, 90, 20);
        menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
        switch (menu_select(M)) {
            case 1:
                menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
                init_brique_jeu(J);
                Init_Balle_Mouvements(Ba, J);
                init_brique_jeu_immobile(BJIM);
                break;
            case 2:
                winClear();
                fontSize(30);
                print(60, DIMW-70, "CECI SONT LES REGLES DU CASSE-BRIQUE :");
                fontSize(27);
                print(58, DIMW-130, "Le but du jeu est de casser la totalite des briques.");
                print(50, DIMW-170, "Il faut faire rebondir la balle sur la raquette du bas,");
                fontSize(25);
                print(30, DIMW-210, "Si la balle rebondit a droite de la raquette, elle part a droite.");
                print(10, DIMW-250, "Si la balle rebondit a gauche de la raquette, elle part a gauche.");
                print(40, DIMW-290, "Si la balle tombe hors de la raquette, la partie est perdue.");
                print(70, DIMW-330, "Apres avoir casse 8 briques, une raquette apparait.");
                print(70, DIMW-370, "Si vous la touchez, la balle rebondit, ou se teleporte.");
                
                fontSize(35);
                print((DIMW/2)-120, DIMW-440, "Bonne chance !");
                
                
                menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
                
                break;
            default:
                break;
        }
    }
    else if (Ba.Position.y < J.Brique.y+10){
        if (J.Brique.x < Ba.Position.x) {
            if (J.Brique.x+70 > Ba.Position.x)
            {
                if (Ba.Position.x >= J.Brique.x && Ba.Position.x <= J.Brique.x+25)
                {
                    if (Ba.Vitesse.y < 0 && Ba.Vitesse.x > 0)
                    {
                        Ba.Vitesse.x = -Ba.Vitesse.x+0.25;
                        Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                    }
                    else if (Ba.Vitesse.y > 0 && Ba.Vitesse.x > 0){
                        Ba.Vitesse.x = Ba.Vitesse.x+0.25;
                        Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                    }
                    else if (Ba.Vitesse.y > 0 && Ba.Vitesse.x < 0)
                    {
                        Ba.Vitesse.x = -Ba.Vitesse.x+0.25;
                        Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                    }
                    else if (Ba.Vitesse.y < 0 && Ba.Vitesse.x < 0)
                    {
                        Ba.Vitesse.x = Ba.Vitesse.x+0.25;
                        Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                    }
                }
                else if (Ba.Position.x >= J.Brique.x+46 && Ba.Position.x <= J.Brique.x+70)
                {
                    if (Ba.Vitesse.y < 0 && Ba.Vitesse.x > 0)
                    {
                        Ba.Vitesse.x = Ba.Vitesse.x+0.25;
                        Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                    }
                    else if (Ba.Vitesse.y > 0 && Ba.Vitesse.x > 0)
                    {
                        Ba.Vitesse.x = -Ba.Vitesse.x+0.25;
                        Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                    }
                    else if (Ba.Vitesse.y > 0 && Ba.Vitesse.x < 0)
                    {
                        Ba.Vitesse.x = Ba.Vitesse.x+0.25;
                        Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                    }
                    else if (Ba.Vitesse.y < 0 && Ba.Vitesse.x < 0)
                    {
                        Ba.Vitesse.x = -Ba.Vitesse.x+0.25;
                        Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                    }
                }
                else
                {
                    Ba.Vitesse.x = Ba.Vitesse.x+0.25;
                    Ba.Vitesse.y = -Ba.Vitesse.y+0.25;
                }
            }
        }
    }
}

void Colision2(Balle &Ba, BRIQUE_JEU_IMMOBILE &BJM)
{
    int k = 0;
        for (int i=0; i<2; i++)
        {
            for (int j=0; j<8-k; j++)
            {
                if (Ba.Position.y > BJM.Tab[i][j].Brique.y)
                {
                    if (BJM.Tab[i][j].Brique.x <= Ba.Position.x)
                    {
                        if (BJM.Tab[i][j].Brique.x+70 >= Ba.Position.x)
                        {
                            Ba.Vitesse.y = -Ba.Vitesse.y;
                        }
                    }
                }
            }
            k++;
        }
    
}

void Colision3(Balle &Ba, BRIQUE_JEU_OBSTACLE &BJO)
{
    if (Ba.Position.y > BJO.Brique.y && Ba.Position.y < BJO.Brique.y+10) {
        if (BJO.Brique.x <= Ba.Position.x) {
            if (BJO.Brique.x+140 >= Ba.Position.x) {
                    Ba.Vitesse.x = Ba.Vitesse.x+1;
                    Ba.Vitesse.y = -Ba.Vitesse.y+1;
            }
        }
    }else if (Ba.Position.y < BJO.Brique.y+20 && Ba.Position.y > BJO.Brique.y+10){
        if (BJO.Brique.x <= Ba.Position.x) {
            if (BJO.Brique.x+140 >= Ba.Position.x) {
                Ba.Position.x = rand()%150+300;
                Ba.Position.y = Ba.Position.y;
            }
        }
    }
}

void Balle_Update(Balle &Ba, BRIQUE_JEU &J, BRIQUE_JEU_IMMOBILE &BJIM, Menu M, BRIQUE_JEU_OBSTACLE &BJO)
{
    float dt = 0.01; float m = Ba.masse;
    Ba.Force.x = 0; Ba.Force.y = 0;
    Ajout_Force(Ba, make_complex(0, -9.81));
    
    Ba.Vitesse = Ba.Vitesse + Ba.Force*(dt/m);
    Ba.Position = Ba.Position + Ba.Vitesse*dt;
    
    Colision(Ba, J, M, BJIM);
    Colision2(Ba, BJIM);
    
    if (BJIM.nb_brique_tuer >= 8) {
        Colision3(Ba, BJO);
    }
}

void brique_principale_update(BRIQUE_JEU &J)
{
    if(isKeyPressed(SDLK_LEFT))
    {
        color(255,255,255);
        rectangleFill(J.Brique.x, J.Brique.y, (J.Brique.x+70)+20, J.Brique.y+10);
        J.Brique.x = J.Brique.x-20;
        draw_brique_principale(J);
        
    }
    else if (isKeyPressed(SDLK_RIGHT))
    {
        color(255,255,255);
        rectangleFill(J.Brique.x-20, J.Brique.y, J.Brique.x+70, J.Brique.y+10);
        J.Brique.x = J.Brique.x+20;
        draw_brique_principale(J);
    }
}

int main(int , char** )
{
    srand(time(NULL));
    BRIQUE_JEU_OBSTACLE BJO;
    BRIQUE_JEU J;
    BRIQUE_JEU_IMMOBILE BJIM;
    Balle B;
    Menu M;
    bool stop=false;
    winInit("Casse-Brique", DIMW, DIMW);
    backgroundColor( 255, 255, 255);
    
    
    menu_add(M, "Jouer");
    menu_add(M, "Recharger");
    menu_add(M, "Regles");
    
    
    while (!stop)
    {
        menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
        winClear();
        switch (menu_select(M)) {
            case 0:
                break;
            case 1:
                menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
                init_brique_jeu_obstacle(BJO);
                init_brique_jeu(J);
                Init_Balle_Mouvements(B, J);
                init_brique_jeu_immobile(BJIM);
                break;
            case 2:
                winClear();
                fontSize(30);
                print(60, DIMW-70, "CECI SONT LES REGLES DU CASSE-BRIQUE :");
                fontSize(27);
                print(58, DIMW-130, "Le but du jeu est de casser la totalite des briques.");
                print(50, DIMW-170, "Il faut faire rebondir la balle sur la raquette du bas,");
                fontSize(25);
                print(30, DIMW-210, "Si la balle rebondit a droite de la raquette, elle part a droite.");
                print(10, DIMW-250, "Si la balle rebondit a gauche de la raquette, elle part a gauche.");
                print(40, DIMW-290, "Si la balle tombe hors de la raquette, la partie est perdue.");
                print(70, DIMW-330, "Apres avoir casse 8 briques, une raquette apparait.");
                print(70, DIMW-370, "Si vous la touchez, la balle rebondit, ou se teleporte.");
                
                fontSize(35);
                print((DIMW/2)-120, DIMW-440, "Bonne chance !");
                
                
                menu_draw(M, DIMW/2-70,150, DIMW/2+70, 250);
                break;
            default:
                break;
        }
        draw_Balle(B);
        draw_brique_principale(J);
        draw_brique_immobile(BJIM);
        brique_principale_update(J);
        update_brique_immobile(BJIM, B, M, J, BJO);
        Balle_Update(B, J, BJIM, M, BJO);
        stop = winDisplay();
    }
}
