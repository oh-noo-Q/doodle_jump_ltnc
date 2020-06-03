#include "print.h"
#include "mainobject.h"
#include "bullet.h"
#include "text.h"

SDL_Window* g_windows = NULL;
SDL_Renderer* g_screen = NULL;
SDL_Event g_event;
print name_game, g_background;
print plat[plat_amount], monster[4], menu_[10];
mainobject character;
Mix_Chunk *mix_jump, *mix_die, *mix_bullet;
TTF_Font *font_point= NULL, *font_author = NULL, *font_status= NULL;
text point_game, author, status_game;
float dy;
int point=0, xm, ym, max_plat, min_plat, frame=0;
string name_player;

struct player
{
    int score;
    string name;
};
player players[5];

enum status
{
    menu, playing, score, cancel, option, die, alive
};
status stt, main_, extra[4];
enum toggle
{
    on = true, off = false
};
toggle sound;

// thiet lap man hinh
bool initdata()
{
    bool success=true;
    int test= SDL_Init(SDL_INIT_VIDEO);
    if(test<0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_windows= SDL_CreateWindow("oh no problem I",400,100,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if(g_windows==NULL)     success=false;
    else {
        g_screen= SDL_CreateRenderer(g_windows,-1,SDL_RENDERER_ACCELERATED);
        if(g_screen==NULL)  success=false;
        else {
            SDL_SetRenderDrawColor(g_screen,255,255,255,255);
            int imgflag= IMG_INIT_PNG;
            if(!(IMG_Init(imgflag)&&imgflag)) success=false;
        }
        //text
        if(TTF_Init()==-1) success = false;
        font_point = TTF_OpenFont("FOnt//dlxfont_.ttf",20);
        point_game.setcolor_(text::orange);
        font_status = TTF_OpenFont("FOnt//dlxfont_.ttf",40);
        status_game.setcolor_(text::red);
        font_author = TTF_OpenFont("FOnt//dlxfont_.ttf",18);
        author.setcolor_(text::black);
        //mixer
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)==-1) return false;
        mix_jump = Mix_LoadWAV("Sound//mix_jump_1.mp3");
        mix_bullet = Mix_LoadWAV("SOund//snowball-throw2.mp3");
    }
    return success;
}

void set_up()
{
    for(int i=0;i<plat_amount;i++) {
        plat[i].a=rand()%SCREEN_WIDTH;
        plat[i].b=rand()%SCREEN_HEIGHT;
    }
    character.loadimg("doodle//doodle_left.png",g_screen);
    character.a=150;
    character.b=200;
    g_background.a = 0;
    g_background.b = 0;
    point = 0;      plat[13].b = 0;
    frame = 0;      dy = 0;
    max_plat = 12;  min_plat = 0;
    main_ = alive;
}

bool loadimage()
{
    bool ret= g_background.loadimg("bck@2x.png",g_screen);
    for(int i=0;i<plat_amount;i++) {
       if(i<12) plat[i].loadimg("doodle_platforms_0.png",g_screen);
       else if(i<13) plat[i].loadimg("doodle_platforms_1.png",g_screen);
    }
    menu_[0].loadimg("menu//play.png",g_screen);
    menu_[1].loadimg("menu//scores.png",g_screen);
    menu_[2].loadimg("menu//options.png",g_screen);
    menu_[3].loadimg("menu//cancel.png",g_screen);
    menu_[4].loadimg("menu//play-again-on.png",g_screen);
    menu_[5].loadimg("menu//menu-on@2x.png",g_screen);
    name_game.loadimg("doodle-jump@2x.png",g_screen);
    if(ret==false) return false;
    else return true;
}

void print_text(text ttf, string s, int left, int down, TTF_Font* font)
{
    ttf.settext(s);
    ttf.loadfromrender(font,g_screen);
    ttf.rendertext(g_screen,left,down);
}

void set_menu() // menu status
{
    g_background.render(g_screen);
    menu_[0].b = 240;
    for(int i=0;i<4;i++) {
        menu_[i].a = SCREEN_WIDTH/2 - 50;
        if(i>0) menu_[i].b = menu_[i-1].b +80;
    }
    for(int i=0;i<4;i++) menu_[i].render(g_screen);
    name_game.a = SCREEN_WIDTH/2 - 150;
    name_game.b = 50;
    name_game.render(g_screen);
    string bonus = "By Minh Quyet";
    print_text(author, bonus, 169, 150, font_author);
    SDL_RenderPresent(g_screen);
    while (SDL_PollEvent(&g_event)) {
        if(g_event.type==SDL_MOUSEBUTTONDOWN) {
            xm = g_event.button.x;
            ym = g_event.button.y;
            if(ym>=menu_[0].b && ym<=menu_[0].b+40 && xm>=menu_[0].a && xm<=menu_[0].a+100)
                        stt = playing;
            if(ym>=menu_[3].b && ym<=menu_[3].b+40 && xm>=menu_[3].a && xm<=menu_[3].a+100)
                        stt = cancel;
            if(ym>=menu_[1].b && ym<=menu_[1].b+40 && xm>=menu_[1].a && xm<=menu_[1].a+100)
                        stt = score;
        }
    }
}

void set_score() // score statua
{
    g_background.render(g_screen);
    int down =  50;
    for(int j=0; j<5; j++) {
        string s = players[j].name;
        print_text(point_game, s, 50 , down, font_point);
        string s_ = to_string(players[j].score);
        print_text(point_game, s_, 300, down, font_point);
        down +=50;
    }
    menu_[5].a = 50;    menu_[5].b = SCREEN_HEIGHT - 150;
    menu_[5].render(g_screen);
    SDL_RenderPresent(g_screen);
    while (SDL_PollEvent(&g_event)) {
        if(g_event.type==SDL_MOUSEBUTTONDOWN) {
            xm = g_event.button.x;
            ym = g_event.button.y;
            if(ym>=menu_[5].b && ym<=menu_[5].b+40 && xm>=menu_[5].a && xm<=menu_[5].a+100)
                        stt = menu;
        }
    }
}

void set_die() // die status
{
    string die_ = "YOU LOSE";
    print_text(status_game, die_, 50, 100, font_status);
    menu_[4].a = SCREEN_WIDTH/2 - 50; menu_[4].b = 200;
    menu_[4].render(g_screen);
    menu_[5].a = SCREEN_WIDTH/2 - 50; menu_[5].b = 300;
    menu_[5].render(g_screen);
    SDL_RenderPresent(g_screen);
    while (SDL_PollEvent(&g_event)) {
        if(g_event.type==SDL_MOUSEBUTTONDOWN) {
            xm = g_event.button.x;
            ym = g_event.button.y;
            if(ym>=menu_[4].b && ym<=menu_[4].b+40 && xm>=menu_[4].a && xm<=menu_[4].a+100)
                        stt = playing;
            if(ym>=menu_[5].b && ym<=menu_[5].b+40 && xm>=menu_[5].a && xm<=menu_[5].a+100)
                        stt = menu;
        }
    }
}

void move_game()
{
    character.b+=dy;
    dy+=0.25;
    for(int i=min_plat;i<max_plat;i++) {
        if((character.a+60>plat[i].a) && (character.a+28<plat[i].a+68)
            && (character.b+70>plat[i].b) && (character.b+70<plat[i].b+10) && (dy>0))
            {
                 if(main_==alive) {
                    if(i==12) dy=-20;
                    if(i<12) dy=-11;
                    if(sound == on) Mix_PlayChannel(-1, mix_jump, 0);
                }
            }
    }
    if(character.b<200) {
        for(int i=min_plat;i<max_plat;i++) {
            character.b=200;
            plat[i].b-=dy;
            if(plat[i].b>SCREEN_HEIGHT) {
                plat[i].b=0;
                plat[i].a=rand()%380;
                if(point>600 && point%30==0 && monster[0].hp==0 ) {
                    monster[0].loadimg("monster//small_monster.png",g_screen);
                    monster[0].a = plat[i].a + 10;
                    monster[0].b = plat[i].b - 62;
                    monster[0].hp=1;
                }
                if(point>1000 && point%90==0 && monster[1].hp==0 ) {
                    monster[1].loadimg("monster//big_monster.png",g_screen);
                    monster[1].a = plat[i].a - 10;
                    monster[1].b = plat[i].b - 69;
                    monster[1].hp=1;
                }
            }
        }
        if(point>1200 && point%318==0 && monster[2].hp==0 ) {
            monster[2].loadimg("monster//dia_bay.png",g_screen);
            monster[2].a = rand()%350;
            monster[2].b = -40;
            monster[2].hp=1;
        }
        if(point>1000 && point%126==0 && monster[3].hp==0 ) {
            monster[3].loadimg("monster//hole_.png",g_screen);
            monster[3].a = rand()%380;
            monster[3].b = -20;
            monster[3].hp= 1;
        }
        for(int i=0; i<4; i++) {
            character.b=200;
            monster[i].b-=dy;
        }
        if(dy<-5) point+=6;
    }
    if(point>500) { max_plat=13;}
    if(character.a>SCREEN_WIDTH) character.a-=(SCREEN_WIDTH+50);
    if(character.a+60<0) character.a+=(SCREEN_WIDTH+50);
}

void character_die(int mst, int x, int y)
{
    if(main_==die) {
        if(mst==0 || mst==1) {
            character.loadimg("doodle_die.png", g_screen);
            dy = 5;
        }
        if(mst==2) {
            if(frame==0) {
                character.a = x + 35;       character.b = y + 100;
                character.loadimg("frame//doodle_frame_0.png",g_screen);
            }
            if(frame==10) character.loadimg("frame//doodle_frame_1.png",g_screen);
            if(frame==20) character.loadimg("frame//doodle_frame_2.png",g_screen);
            character.b -= 3;
            dy = 0;     frame++;
        }
        if(mst==3) {
            if(frame==0) {
                character.a = x + 25;       character.b = y + 30;
                character.loadimg("frame//doodle_frame_0.png",g_screen);
            }
            if(frame==10) character.loadimg("frame//doodle_frame_1.png",g_screen);
            if(frame==15) character.loadimg("frame//doodle_frame_2.png",g_screen);
            if(frame==20) character.loadimg("frame//doodle_frame_3.png",g_screen);
            dy = 0;     frame++;
        }
    }
}

void close()
{
    g_background.Free();
    name_game.Free();
    character.Free();
    for(int i=0; i<plat_amount; i++) plat[i].Free();
    for(int i=0; i<10; i++) menu_[i].Free();
    point_game.Free();
    author.Free();
    status_game.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_windows);
    g_windows = NULL;
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    srand(time(0));
    stt = menu;
    sound = on;
    int i=0, died_by = 5;
    ifstream name_("name.txt");
    ifstream point_("point.txt");
    while (!name_.eof()) {
        name_ >> players[i].name;
        point_ >> players[i].score;
        i++;
    }
    if(initdata()==false) return 1;
    if(loadimage()==false) return 1;
    bool exit = false;
    while(!exit) {
        if(stt==menu) set_menu();
        if(stt==cancel) exit=true;
        if(stt==die) set_die();
        if(stt==score) set_score();
        if(stt==playing) {
            bool quit=false;
            set_up();
            while(!quit)
            {
               while(SDL_PollEvent(&g_event)!=0) {
                if(g_event.type==SDL_QUIT)  { exit=true; quit=true; }
               }
               SDL_RenderClear(g_screen);

               g_background.render(g_screen,NULL);
               if(character.b>SCREEN_HEIGHT) {
                    quit=true;      stt=die;
                    for(int i=0; i<4; i++) monster[i].Free();
               }
               character.update(g_screen);
               for(int i=0;i<max_plat;i++)  plat[i].render(g_screen);
               for(int i=0; i<4; i++) monster[i].render(g_screen);
               if( frame==0 ) move_game();
               if( main_==alive ) character.input_move(g_event, g_screen, mix_bullet, sound);
               character.render(g_screen);
               character.shoot_bullet(g_screen);
               vector<bullet*> bullet_arr = character.get_bullet_list();
               for(int i=0; i<bullet_arr.size(); i++)
               {
                  bullet* p_bullet = bullet_arr.at(i);
                   if(p_bullet != NULL)
                   {
                        SDL_Rect brect;
                        brect.x = p_bullet->a;
                        brect.y = p_bullet->b;
                        brect.w = p_bullet->getrect().w;
                        brect.h = p_bullet->getrect().h;
                        for(int j=0; j<2; j++)
                        {
                            SDL_Rect mrect;
                            mrect.x = monster[j].a;
                            mrect.y = monster[j].b;
                            mrect.w = monster[j].getrect().w;
                            mrect.h = monster[j].getrect().h;
                            //bullet collision monster
                            if(SDLcomon::checkcollision(brect,mrect)) {
                                character.removebullet(i);
                                monster[j].hp++;
                            }
                        }
                   }
               }
               SDL_Rect crect;
               crect.x = character.a;
               crect.y = character.b;
               crect.w = character.getrect().w;
               crect.h = character.getrect().h;
               for(int i=0; i<4; i++) {
                    if(i==0) monster[i].kill_monster(5,g_screen);
                    if(i==1) monster[i].kill_monster(20,g_screen);
                    if(i==2 || i==3) monster[i].kill_monster(200,g_screen);
                    SDL_Rect mrect;
                    mrect.x = monster[i].a;
                    mrect.y = monster[i].b;
                    mrect.w = monster[i].getrect().w;
                    mrect.h = monster[i].getrect().h;
                    // main collision monster
                    if(SDLcomon::checkcollision(crect,mrect) && main_==alive ) {
                        main_=die;
                        died_by = i;
                    }
               }
               character_die(died_by, monster[died_by].a, monster[died_by].b);
               if(frame>20) {
                    quit = true;   stt = die;
                    for(int i=0; i<4; i++) monster[i].Free();
               }
               // count point
               string str_val = to_string(point);
               print_text(point_game, str_val, 10, 10, font_point);

               SDL_RenderPresent(g_screen); // display
               SDL_Delay(15);
               cout << monster[2].hp << "  " << monster[3].hp << endl;
            }
        }
    }
    close();
    return 0;
}
