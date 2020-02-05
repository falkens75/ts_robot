//gcc algo.c -o algo -Wall -pedantic -DMAIN

#include <stdio.h>
#include <stdint.h>

const uint8_t x_size = 4;
const uint8_t y_size = 5;
const uint8_t node_size = x_size*2 + y_size*2;
uint8_t matris[4][5];

enum dir {_low=2, _east=3, _south=4, _west=5, _north=6, _high=7, _higher=8};

/*
const uint8_t east = 3;
const uint8_t south = 4;
const uint8_t west = 5;
const uint8_t north = 6;
*/

uint8_t turn_left (enum dir direction)
{
    direction = direction-1;
    if(direction == _low)
        direction = _north;

    return direction;
}


uint8_t turn_right (enum dir direction)
{
    direction = direction+1;
    if(direction == _high)
        direction = _east;

    return direction;
}

uint8_t turn_180 (enum dir direction)
{
    direction = direction + 2;
    if (direction == _high)
        direction = _east;
        else if(direction == _higher)
        {
            direction = _south;
        }
        
    return direction;
}


void pass_line(uint8_t direction, uint8_t * x1, uint8_t * y1)
{
    switch (direction) 
    {
        case _east :  *y1 = *y1+1; break;
        case _south : *x1 = *x1+1; break;
        case _west :  *y1 = *y1-1; break;
        case _north : *x1 = *x1-1; break;
        default : break;
    }

    return;
}


#ifdef MAIN
int main()
{
    printf("%s\n", "_east=3, _south=4, _west=5, _north=6");
    uint8_t a = 3;
    uint8_t b = turn_left(a);
    printf("turn_left %d, %d\n", a, b);
    
    a=6;
    uint8_t c = turn_right(a);
    printf("turn_right %d, %d\n", a, c);

    a=_west;
    c = turn_180(a);
    printf("turn_180 %d, %d\n", a, c);

    a=_north;
    uint8_t x1 = 1;
    uint8_t y1 = 2;
    printf(" %d, %d, ", x1, y1);
    pass_line(_north, &x1, &y1);
    printf(", pass_line %d, %d\n", x1, y1);



    return 0;
}

#endif