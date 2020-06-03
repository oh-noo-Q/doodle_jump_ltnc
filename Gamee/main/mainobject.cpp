#include "mainobject.h"

mainobject::mainobject()
{
    //ctor
}

mainobject::~mainobject()
{
    //dtor
}

void mainobject::input_move(SDL_Event event_, SDL_Renderer* screen, Mix_Chunk* mix_bullet,bool sound)
{
    if(event_.type==SDL_KEYDOWN) {
            if(event_.key.keysym.sym==SDLK_RIGHT) {
                a+=5;
                loadimg("doodle//doodle_right.png",screen);
                status = move_right;
            }
            if(event_.key.keysym.sym==SDLK_LEFT) {
                a-=5;
                loadimg("doodle//doodle_left.png",screen);
                status = move_left;
            }
            if(event_.key.keysym.sym==SDLK_UP) {
                if(status==move_left) {
                    loadimg("doodle//doodle_fire_left.png",screen);
                    status = shoot_left;
                }
                else {
                    loadimg("doodle//doodle_fire_right.png",screen);
                    status = shoot_right;
                }
                bullet* p_bullet = new bullet();
                p_bullet->loadimg("bullet.png",screen);
                p_bullet->a = this->a + 35;
                p_bullet->b = this->b - 10;
                p_bullet->set_y_val(10);
                p_bullet->set_is_move(true);
                if(sound == true) Mix_PlayChannel(-1, mix_bullet, 0);

                doodle_bullet.push_back(p_bullet);
             }
       }
}
void mainobject::shoot_bullet(SDL_Renderer* screen)
{
    for(int i=0; i<doodle_bullet.size(); i++) {
        bullet* p_bullet = doodle_bullet.at(i);
        if(p_bullet != NULL) {
            if(p_bullet->get_is_move() == true) {
                p_bullet->handlemove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet->render(screen);
            }
            else {
                doodle_bullet.erase(doodle_bullet.begin() + i);
                if(p_bullet!=NULL) {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void mainobject::removebullet(const int& index)
{
    int size_ = doodle_bullet.size();
    if(size_>0 && index<size_) {
        bullet* p_bullet = doodle_bullet.at(index);
        doodle_bullet.erase(doodle_bullet.begin() + index);
        if(p_bullet != NULL) {
            delete p_bullet;
            p_bullet =  NULL;
        }
    }
}

void mainobject::update(SDL_Renderer* screen)
{
    if(status==shoot_left) this->loadimg("doodle//doodle_left.png",screen);
    if(status==shoot_right) this->loadimg("doodle//doodle_right.png",screen);
}
