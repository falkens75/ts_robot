#ifndef _ALGO_H
#define _ALGO_H

#define X_SIZE (5)
#define Y_SIZE (5)
#define NODE_SIZE 21 // (x_size*2 + y_size*2) +1



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
void coordinator(uint8_t Node_Actual, uint8_t Node_Target, uint8_t * Direction, uint8_t * Distance, uint8_t * Turn_Direction);

#endif