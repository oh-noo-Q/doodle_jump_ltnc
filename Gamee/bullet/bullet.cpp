#include "bullet.h"

bullet::bullet()
{
    //ctor
    x_val = 0;
    y_val = 0;
    is_move = false;
}

bullet::~bullet()
{
    //dtor
}

void bullet::handlemove(const int& x_border, const int& y_border)
{
    b -= y_val;
    if(b < 0) {
        is_move = false;
    }
}
