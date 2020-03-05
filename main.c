#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
// #include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

// Makefile: gcc -o frogger main.c -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec

typedef struct posicao{
    int x;
    int y;
    int v;
    int h;
    int w;}posicao;


const int SCREEN_W = 942;
const int SCREEN_H = 530;
const int BOUNCER_SIZE = 32;
const int FPS = 100;
const int MOVE_VERTICAL = 5;
const int MOVE_HORIZONTAL = 5;

enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

int main()
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *car[10], *frog=NULL, *fundo=NULL, *img1=NULL, *img2=NULL, *img3=NULL, *img4=NULL, *img5=NULL;
    
    bool quit = false, begin_game=false, sair=false, colisao=false, redraw, win=false;
    int conta;
    float tempo = 0;
    int p=1;
    bool key[4] = { false, false, false, false };

    srand((unsigned)time(NULL));
    int r=100, w=130;
    

    
    // ROTINAS DE INICIALIZACAO
    
	al_init();
	al_install_keyboard();
	// al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
    al_init_image_addon();
	
    //INICIALIZACAO DO TEXTO
	ALLEGRO_FONT *fonte = al_load_font("src/Apple LiSung Light.ttf", 32, 1);
    
    
    //CRIA FILA DE EVENTOS
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	timer = al_create_timer(1.0 / FPS);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	
    //CRIA DISPLAY SCREEN_W x SCREEN_H na cor 0 0 0
	display = al_create_display(SCREEN_W, SCREEN_H);
    al_set_window_title(display, "Frogger(Game of Thrones)");
    al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(al_map_rgb(0, 0, 0));
    al_register_event_source(event_queue, display);
    
    
    fundo=al_load_bitmap("src/background.jpg");
    img1=al_load_bitmap("src/tyrion_lannister.jpg");
    img2=al_load_bitmap("src/khal_drogo.jpg");
    img3=al_load_bitmap("src/jon_snow.jpg");
    img4=al_load_bitmap("src/jaime__lannister.png");
    img5=al_load_bitmap("src/daenerys_targaryen.jpg");
    int wimg1=al_get_bitmap_width(img1), himg=al_get_bitmap_height(img1), wimg2=al_get_bitmap_width(img2), wimg3=al_get_bitmap_width(img3),wimg4=al_get_bitmap_width(img4), wimg5=al_get_bitmap_width(img5);
    al_draw_bitmap(fundo, 0, 0, 0);
    al_draw_bitmap(img1, 30, 30, 0);
    al_draw_bitmap(img2, 30, 60+himg, 0);
    al_draw_bitmap(img3, 30, 90+himg+himg, 0);
    al_draw_bitmap(img4, SCREEN_W/2 + 30, 30, 0);
    al_draw_bitmap(img5, SCREEN_W/2+30, 90+himg*2, 0);
    al_draw_text(fonte, al_map_rgb(rand()%200 + 55,rand()%200 + 55, rand()%200 + 55), 300, 0, 0, "Escolha seu personagem:");
    al_draw_text(fonte, al_map_rgb(255, 255, 0), 50+wimg1, 30+himg/2, 0, "Tyrion Lannister (t)");
    al_draw_text(fonte, al_map_rgb(255, 0, 255), 50+wimg2, 60+himg+himg/2, 0, "Khal Drogo (k)");
    al_draw_text(fonte, al_map_rgb(0, 0, 255), 50+wimg3, 90+himg+himg+himg/2, 0, "Jon Snow (j)");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 50+(SCREEN_W/2)+wimg4,30+himg/2, 0, "Jaime Lannister (l)");
    al_draw_text(fonte, al_map_rgb(255, 0, 0), 40+wimg5+SCREEN_W/2, 90+himg*2+himg/2, 0, "Danny Targaryen (d)");
    
    al_flip_display();
    int personagem;
    while (!begin_game && !quit) {
        ALLEGRO_EVENT e;
        al_wait_for_event(event_queue, &e);
        if(e.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(e.keyboard.keycode){
                    case ALLEGRO_KEY_T:
                    personagem=1;
                    begin_game=true;
                    break;
                    case ALLEGRO_KEY_K:
                    personagem=2;
                    begin_game=true;
                    break;
                    case ALLEGRO_KEY_J:
                    personagem=3;
                    begin_game=true;
                    break;
                    case ALLEGRO_KEY_L:
                    personagem=4;
                    begin_game=true;
                    break;
                    case ALLEGRO_KEY_D:
                    personagem=5;
                    begin_game=true;
                    break;
                    case ALLEGRO_KEY_ESCAPE:
                    quit=true;
                    break;
            }
        
        }
        if (e.type==ALLEGRO_EVENT_DISPLAY_CLOSE) {
            quit=true;
            begin_game=true;
            break;
        }
    
    }
    al_start_timer(timer);
    //CRIA O FROGGER TAMANHO BOUNCER SIZE
    if (personagem==1)
        frog = al_load_bitmap("src/tyrion_lannister.png");
        
    else if (personagem==2)
        frog = al_load_bitmap("src/khal_drogo.png");
    
    else if(personagem==3)
        frog = al_load_bitmap("src/jon_snow.png");
    else if(personagem==4)
        frog = al_load_bitmap("src/jaime_lannister.png");
    else
        frog = al_load_bitmap("src/daenerys_targaryen.png");

    // al_destroy_bitmap(fundo);
    al_destroy_bitmap(img1);
    al_destroy_bitmap(img2);
    al_destroy_bitmap(img3);
    al_destroy_bitmap(img4);
    al_destroy_bitmap(img5);
    
    
    fundo=al_load_bitmap("src/FireAndBloog.jpg");
    al_draw_bitmap(fundo, 0, 0, 0);
	al_set_target_bitmap(frog);
    int sizef = al_get_bitmap_width(frog);
	posicao frogger;
	frogger.x = SCREEN_W/2;
	frogger.y = (SCREEN_H - BOUNCER_SIZE)+2;
    frogger.w=al_get_bitmap_width(frog);
    frogger.h=al_get_bitmap_height(frog);

	al_draw_bitmap(frog, frogger.x, frogger.y, 0);
    
    //CRIA OS 10 OBSTACULOS ALEATORIOS
	posicao carro[10];
	int i;
	int CAR_H[10];
	for (i=0;i<10;i++)
	{
		CAR_H[i] = 24;
		car[i] = al_load_bitmap("src/white_walker.png");
		carro[i].x = 0;
		carro[i].y = 50*i;
		carro[i].v = (rand()%6)+p;
        carro[i].w=al_get_bitmap_width(car[i]);
        carro[i].h=al_get_bitmap_height(car[i]);

		al_set_target_bitmap(car[i]);
		al_set_target_bitmap(al_get_backbuffer(display));
		al_draw_bitmap(car[i], carro[i].x, carro[i].y, 0);
	}
	al_flip_display();
   
	while(!quit)
	{
        ALLEGRO_EVENT e;
        if (colisao) {
            al_stop_timer(timer);
            al_set_target_bitmap(al_get_backbuffer(display));
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(fonte, al_map_rgb(255, 0, 0), r, w, 0, "Voce foi pego pelos outros!!! D:");
            al_draw_text(fonte, al_map_rgb(255, 0, 0), r, w+100, 0, "Deseja continuar? (s)/(n):");
            al_flip_display();
            al_wait_for_event(event_queue, &e);
            if(e.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(e.keyboard.keycode){
                        case ALLEGRO_KEY_S:
                        quit=false;
                        colisao=false;
                        key[KEY_UP] = false;
                        key[KEY_DOWN] = false;
                        key[KEY_RIGHT] = false;
                        key[KEY_LEFT] = false;
                        p=1;
                        al_set_target_bitmap(al_get_backbuffer(display));
                        al_clear_to_color(al_map_rgb(0, 0, 0));
                        al_draw_textf(fonte, al_map_rgb(0, 255, 0), 100, 200, 0, "Level %d",p);
                        al_flip_display();
                        al_rest(3.0);
                        break;
                        case ALLEGRO_KEY_N:
                        quit=true;
                        break;
                        case ALLEGRO_KEY_ESCAPE:
                        quit=true;
                        break;
                }
            }
            if (e.type==ALLEGRO_EVENT_DISPLAY_CLOSE) {
                quit=true;
                break;
            }
           al_start_timer(timer);
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
             al_start_timer(timer);
            frogger.x = SCREEN_W/2;
            frogger.y = (SCREEN_H - BOUNCER_SIZE)+2;
        }
        else {
            al_wait_for_event(event_queue, &e);
            if(e.type == ALLEGRO_EVENT_TIMER) {
                // Atualiza obstáculos
                tempo++;
                for (i=0;i<10;i++)
                {
                    carro[i].x += carro[i].v;
                    if( (carro[i].x+CAR_H[i]) > SCREEN_W)
                    {
                        carro[i].x = SCREEN_W - CAR_H[i];
                        carro[i].v = ((rand()%6)+p)*(-1);
                    }
                    if(carro[i].x < 0)
                    {
                        carro[i].x=0;
                        carro[i].v = (rand()%6)+p;
                    }
                }

                // Atualiza Frogger
                if(key[KEY_UP]) {
                    frogger.y -= MOVE_VERTICAL;
                }

                if(key[KEY_DOWN] && frogger.y <= SCREEN_H - BOUNCER_SIZE - MOVE_VERTICAL) {
                   frogger.y += MOVE_VERTICAL;
                }

                if(key[KEY_LEFT] && frogger.x >= MOVE_HORIZONTAL) {
                   frogger.x -= MOVE_HORIZONTAL;
                }

                if(key[KEY_RIGHT] && frogger.x <= SCREEN_W - BOUNCER_SIZE - MOVE_HORIZONTAL) {
                   frogger.x += MOVE_HORIZONTAL;
                }

                redraw = true;
             }
            else if(e.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                quit=true;
                break;
            }
            else if(e.type == ALLEGRO_EVENT_KEY_DOWN) {
               switch(e.keyboard.keycode) {
                  case ALLEGRO_KEY_UP:
                  case ALLEGRO_KEY_W:
                    key[KEY_UP] = true;
                    break;
                    
                 case ALLEGRO_KEY_DOWN:
                 case ALLEGRO_KEY_S:
                    key[KEY_DOWN] = true;
                    break;

                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A: 
                   key[KEY_LEFT] = true;
                   break;

                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                   key[KEY_RIGHT] = true;
                      break;
             }
            }
            else if(e.type == ALLEGRO_EVENT_KEY_UP) {
                switch(e.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                    case ALLEGRO_KEY_W:
                      key[KEY_UP] = false;
                      break;

                    case ALLEGRO_KEY_DOWN:
                    case ALLEGRO_KEY_S:
                        key[KEY_DOWN] = false;
                        break;

                    case ALLEGRO_KEY_LEFT:
                    case ALLEGRO_KEY_A: 
                        key[KEY_LEFT] = false;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                    case ALLEGRO_KEY_D:
                        key[KEY_RIGHT] = false;
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        quit = true;
                        break;
                }
            }
            
            if (redraw && al_is_event_queue_empty(event_queue))
            {
                redraw = false;
                al_set_target_bitmap(al_get_backbuffer(display));
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(fundo, 0, 0, 0);
                al_draw_bitmap(frog, frogger.x, frogger.y, 0);
                for (i=0;i<10;i++)
                    al_draw_bitmap(car[i], carro[i].x, carro[i].y, 0);
                al_flip_display();
            }
        
            
            //COLISAO
            for (i=0;i<10;i++)
            {
                if ( (frogger.y + frogger.h >= carro[i].y && frogger.y <= carro[i].y + carro[i].h) && (frogger.x <= carro[i].x + CAR_H[i] && frogger.x+BOUNCER_SIZE >= carro[i].x))
                {
                    win = false;
                    colisao =true;
                }
            }
            
            
            //VENCEU
            if (frogger.y<0)
            {
                win = true;
            }
            
            if (win)
            {
                win=false;
                tempo = tempo/FPS;
                al_stop_timer(timer);
                al_set_target_bitmap(al_get_backbuffer(display));
                al_clear_to_color(al_map_rgb(0, 0, 0));
                p++;
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), 100, 150, 0, "Voce sobreviveu! %.02f segundos :D",tempo);
                al_draw_textf(fonte, al_map_rgb(0, 255, 0), 100, 200, 0, "Level %d",p);
                al_flip_display();
                al_rest(3.0);

                key[KEY_UP] = false;
                key[KEY_DOWN] = false;
                key[KEY_RIGHT] = false;
                key[KEY_LEFT] = false;
                frogger.x = SCREEN_W/2;
                frogger.y = (SCREEN_H - BOUNCER_SIZE)+2;
                al_set_target_bitmap(al_get_backbuffer(display));
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(fundo, 0, 0, 0);
                al_draw_bitmap(frog, frogger.x, frogger.y, 0);
                for (i=0;i<10;i++)
                    al_draw_bitmap(car[i], carro[i].x, carro[i].y, 0);
                al_flip_display();
                al_start_timer(timer);
            }
       }
	}
    
    p=0;
    
    
	tempo = tempo/FPS;

    al_destroy_bitmap(fundo);
    for (i=0; i<10; i++) {
        al_destroy_bitmap(car[i]);
    }
    al_destroy_bitmap(frog);
    // al_destroy_bitmap(img1);
    // al_destroy_bitmap(img2);
    // al_destroy_bitmap(img3);
    // al_destroy_bitmap(img4);
    // al_destroy_bitmap(img5);
    al_destroy_font(fonte);
    al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);
    return 0;
}
