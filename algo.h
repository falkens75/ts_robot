#ifndef _ALGO_H
#define _ALGO_H

struct node_coordinate
{
    uint8_t xk;
    uint8_t yk;
};

enum dir {_low=2, _east=3, _south=4, _west=5, _north=6, _high=7, _higher=8};

uint8_t turn_left (enum dir direction);
uint8_t turn_right (enum dir direction);
uint8_t turn_180 (enum dir direction);
void pass_line(uint8_t direction, uint8_t * x1, uint8_t * y1);
void init(struct node_coordinate nodes[]);

#endif