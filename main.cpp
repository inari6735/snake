#include<iostream>
#include<fstream>

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>

#include"classes.h"
#include"Location.h"

int set_dir(const Snake& snake, ALLEGRO_EVENT pressed_key);

void place_point(Point &point);

int main(){

//*####### INITIALIZING ALLEGRO ########
//    timer, queue, display, font, redraw, event

    al_init();
    al_install_keyboard();

    int FPS = 60;
    ALLEGRO_TIMER* timer = al_create_timer(1.0/FPS);
    ALLEGRO_EVENT_QUEUE* queue= al_create_event_queue();
    ALLEGRO_DISPLAY* display= al_create_display(960, 600);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    


//*###### GAME SET UP ###########

    srand(time(NULL));

    Board board;
    board.w = 10;//test values
    board.h = 7;
    board.a = 60;

    float speed=1.5f;

    Location sn_loc;

    Point point;
    place_point(point);

//### LOADING SETTINGS ###
/*
    std::ifstream settings;
    settings.open("settings.txt");
    settings>>sn_loc.x>>sn_loc.y>>speed;
*/

    sn_loc.x=4;
    sn_loc.y=3;
    Snake snake(sn_loc, speed, FPS);

    bool redraw= true;
    ALLEGRO_EVENT event;

    board.disp_h=al_get_display_height(display);
    board.disp_w=al_get_display_width(display);

    snake.draw(&board);
    al_start_timer(timer);
    int wait=1;
    while(wait){
        al_wait_for_event(queue, &event);
        if(event.type == ALLEGRO_KEY_SPACE) wait=0;
    }
    
    

    int move_timer=0;

//*###### MAIN GAME LOOP ########
    while(1){

        if(event.type==ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        else if(event.type == ALLEGRO_TIMER_EVENT){
            snake.step();
            snake.draw(&board);
        }
        
        else{
           switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_W:
                snake.set_dir(parse_dir(0));
                break;
            case ALLEGRO_KEY_RIGHT:
            case ALLEGRO_KEY_D:
                snake.set_dir(parse_dir(1));
                break;
            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_S:
                snake.set_dir(parse_dir(2));
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_A:
                snake.set_dir(parse_dir(3));
                break;                     
            }
        }
        
        if(snake.game_over(board))break;
        
        if(snake.get_head_loc()== point.loc){
            snake.grow();
            place_point(point);
        }


        al_wait_for_event(queue, &event);
    }

//*###### CLEANING UP ########
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);


    return 0;
}

void place_point(Point &point, Board brd){
    point.loc.x= rand()%brd.w;
    point.loc.y= rand()%brd.h;
}