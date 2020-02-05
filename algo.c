//gcc algo.c -o algo -Wall -pedantic -DMAIN

#include <stdio.h>
#include <stdint.h>

const uint8_t x_size = 4;
const uint8_t y_size = 5;
const uint8_t node_size = x_size*2 + y_size*2;
uint8_t matris[4][5];

enum dir {east=3; south, west,north};

const uint8_t east = 3;
const uint8_t south = 4;
const uint8_t west = 5;
const uint8_t north = 6;

uint8_t turn_left (uint8_t direction)
{
    direction = direction-1;
    if(direction == 2)
        direction = 6;

    return direction;
}


uint8_t turn_right (uint8_t direction)
{
    direction = direction+1;
    if(direction == 7)
        direction = 3;

    return direction;
}




#ifdef MAIN
int main()
{

    uint8_t a = 3;
    uint8_t b = turn_left(a);

    printf(" %d, %d ", a, b);
    
    return 0;
}

#endif