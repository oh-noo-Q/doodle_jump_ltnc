#include "print.h"

print::print()
{
    //ctor
    img_save_=NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
    hp = 0;
}

print::~print()
{
    //dtor
    Free();
}

bool print::loadimg(string path, SDL_Renderer* screen)
{
    SDL_Texture* new_texture= NULL;
    SDL_Surface* load_surface= IMG_Load(path.c_str());
    if(load_surface!=NULL) {
        new_texture= SDL_CreateTextureFromSurface(screen,load_surface);
        if(new_texture!=NULL) {
            rect_.w= load_surface->w;
            rect_.h= load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    }
    img_save_= new_texture;
    if(img_save_!=NULL) return true;
    else return false;
}

void print::render(SDL_Renderer* des, const SDL_Rect* clip)
{
    rect_.x = a;
    rect_.y = b;
    SDL_Rect renderquad={rect_.x,rect_.y,rect_.w,rect_.h};
    SDL_RenderCopy(des,img_save_,clip,&renderquad);
}

void print::Free()
{
    if(img_save_!=NULL) {
        SDL_DestroyTexture(img_save_);
        img_save_=NULL;
        rect_.w=0;
        rect_.h=0;
        a = 0;
        b = 0;
        hp = 0;
    }
}

bool SDLcomon::checkcollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
  int left_a = object1.x;
  int right_a = object1.x + object1.w;
  int top_a = object1.y;
  int bottom_a = object1.y + object1.h;

  int left_b = object2.x;
  int right_b = object2.x + object2.w;
  int top_b = object2.y;
  int bottom_b = object2.y + object2.h;

  int kc = 10;
  //  size object 1 < size object 2
  if (left_a > (left_b+kc) && left_a < (right_b-kc))
  {
    if (top_a > (top_b+kc) && top_a < (bottom_b-kc))
    {
      return true;
    }
  }

  if (left_a > (left_b+kc) && left_a < (right_b-kc))
  {
    if (bottom_a > (top_b+kc) && bottom_a < (bottom_b-kc))
    {
      return true;
    }
  }

  if (right_a > (left_b+kc) && right_a < (right_b-kc))
  {
    if (top_a > (top_b+kc) && top_a < (bottom_b-kc))
    {
      return true;
    }
  }

  if (right_a > (left_b+kc) && right_a < (right_b-kc))
  {
    if (bottom_a > (top_b+kc) && bottom_a < (bottom_b-kc))
    {
      return true;
    }
  }

  //  size object 1 < size object 2
  if (left_b > (left_a+kc) && left_b < (right_a-kc))
  {
    if (top_b > (top_a+kc) && top_b < (bottom_a-kc))
    {
      return true;
    }
  }

  if (left_b > (left_a+kc) && left_b < (right_a-kc))
  {
    if (bottom_b > (top_a+kc) && bottom_b < (bottom_a-kc))
    {
      return true;
    }
  }

  if (right_b > (left_a+kc) && right_b < (right_a-kc))
  {
    if (top_b > (top_a+kc) && top_b < (bottom_a-kc))
    {
      return true;
    }
  }

  if (right_b > (left_a+kc) && right_b < (right_a-kc))
  {
    if (bottom_b > (top_a+kc) && bottom_b < (bottom_a-kc))
    {
      return true;
    }
  }

   //  size object 1 = size object 2
  if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
  {
    return true;
  }

  return false;
}

bool print::kill_monster(const int& n, SDL_Renderer* screen)
{
    if(hp>n || this->b>SCREEN_HEIGHT) {
        this->Free();
        hp = 0;
        return true;
    }

}
