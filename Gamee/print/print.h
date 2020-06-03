#ifndef PRINT_H
#define PRINT_H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;
const int plat_amount = 30;

namespace SDLcomon
{
    bool checkcollision (const SDL_Rect& object1, const SDL_Rect& object2);
}
class print
{
    public:
        print();
        virtual ~print();
        int a, b, hp;
        void setrect(const int& x, const int& y) {rect_.x=x;rect_.y=y;}
        SDL_Rect getrect() const {return rect_;}
        SDL_Texture* getimg() const {return img_save_;}
        bool loadimg (string path, SDL_Renderer* screen);
        void render(SDL_Renderer* des, const SDL_Rect* clip= NULL);
        bool kill_monster(const int& n, SDL_Renderer* screen);
        void Free();
    protected:
        SDL_Texture* img_save_;
        SDL_Rect rect_;
};

#endif // PRINT_H
