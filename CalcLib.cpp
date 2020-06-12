
#include <allegro5/allegro.h>
#include <cmath>
#include "CalcLib.h"


namespace Calculations
{

    void Calculations::drawStar(int x, int y) {
        al_draw_pixel(x, y, al_map_rgb(255, 255, 255));
        al_draw_pixel(x + 1, y, al_map_rgb(255, 255, 255));
        al_draw_pixel(x, y + 1, al_map_rgb(255, 255, 255));
        al_draw_pixel(x - 1, y, al_map_rgb(255, 255, 255));
        al_draw_pixel(x, y - 1, al_map_rgb(255, 255, 255));
    }


    void Calculations::drawSmallStar(int x, int y) {
        al_draw_pixel(x, y, al_map_rgb(255, 255, 255));
    }


    float Calculations::distanceCalculate(float x1, float y1, float x2, float y2)
    {
        return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
    }
}