#ifndef MAIN_OBJECT_H
#define MAIN_OBJECT_H

#include "print.h"
#include "bullet.h"

class main_object : public print
{
    public:
        main_object();
        virtual ~main_object();
        void set_x_main(const int& xmain) {x_main = xmain;}
        void set_y_main(const int& ymain) {y_main = ymain;}
        int get_x_main() const {return x_main;}
        int get_y_main() const {return y_main;}
        void main_move(SDL_Renderer* screen);
    protected:

    private:
        int x_main;
        int y_main;

};

#endif // MAIN_OBJECT_H
