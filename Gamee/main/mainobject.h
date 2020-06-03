#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include "print.h"
#include "bullet.h"

class mainobject : public print
{
    public:
        mainobject();
        virtual ~mainobject();

        enum MOVE
        {
            move_left = 0,
            move_right = 1,
            shoot_left = 2,
            shoot_right = 3,
        };
        void input_move(SDL_Event event_, SDL_Renderer* screen, Mix_Chunk* mix_bullet, bool sound);
        void set_bullet(vector<bullet*> bullet_list)
        {
            doodle_bullet = bullet_list;
        }
        vector<bullet*> get_bullet_list() const {return doodle_bullet;}
        void shoot_bullet (SDL_Renderer* screen);
        void removebullet(const int& index);
        void update(SDL_Renderer* screen);
    protected:

    private:
        int status;
        vector<bullet*> doodle_bullet;
};

#endif // MAINOBJECT_H
