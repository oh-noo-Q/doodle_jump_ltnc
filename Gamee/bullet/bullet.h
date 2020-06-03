#ifndef BULLET_H
#define BULLET_H

#include "print.h"

class bullet : public print
{
    public:
        bullet();
        virtual ~bullet();

        void set_y_val(const int& yval) {y_val = yval;}
        int get_y_val() const {return y_val;}

        void set_is_move(const bool& ismove) {is_move = ismove;}
        bool get_is_move() const {return is_move;}

        void handlemove(const int& x_border, const int& y_border);

    protected:

    private:
        int x_val;
        int y_val;
        bool is_move;


};

#endif // BULLET_H
