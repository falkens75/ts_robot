//gcc algo.c -o algo -Wall -pedantic -DMAIN

#include <stdio.h>
#include <stdint.h>
#include "algo.h"




struct node_coordinate nodes[NODE_SIZE];


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
        case _east :
            *y1 = *y1+1;
            break;
        case _south :
            *x1 = *x1+1;
            break;
        case _west :
            *y1 = *y1-1;
            break;
        case _north :
            *x1 = *x1-1;
            break;
        default :
            break;
        }
    return;
}


void init(struct node_coordinate nodes[])
{
   uint8_t counter;
   uint8_t init_x = 0;
   uint8_t init_y = 1;
   
   for(counter = 0; counter<NODE_SIZE; counter++)
   {
        nodes[counter].xk = 0xEE;
        nodes[counter].yk = 0xEF;
   }
   
  
  //Go East.
    for(counter = 1; counter < Y_SIZE+1; counter++)
    {
        nodes[counter].xk = init_x;
        nodes[counter].yk = init_y;
        init_y++;
    }
    //Go south
    init_x = 1;
    for(counter = Y_SIZE+1; counter < Y_SIZE+X_SIZE+1; counter++)
    {
        nodes[counter].xk = init_x;
        nodes[counter].yk = init_y;
        init_x++;
    }
    //Go west
    init_y--;
    for(counter = Y_SIZE+X_SIZE+1; counter < Y_SIZE+X_SIZE+Y_SIZE+1; counter++)
    {
        nodes[counter].xk = init_x;
        nodes[counter].yk = init_y;
        init_y--;
    }
    //Go north
    init_x--;
    for(counter = Y_SIZE+X_SIZE+Y_SIZE+1; counter <Y_SIZE+X_SIZE+Y_SIZE+X_SIZE+1; counter++)
    {
        nodes[counter].xk = init_x;
        nodes[counter].yk = init_y;
        init_x--;
    }
}



// #include <orienteering.h>


 
 /*
Ex 4x3 = 4 Columns (y), 3 Rows (x)

		1		2		3		4 (Y-max)				< Row = 0
        I		I		I		I					
    14 ---------------------------- 5 (Y-max +1)		< Row = 1
		I		I		I		I
		I		I		I		I
	13 ---------------------------- 6					< Row = 2
		I		I		I		I
		I		I		I		I
	12 ---------------------------- 7 (Y-max+X-max)		< Row = 3
		I		I		I		I
		11		10		9		8 (Y-max + X-max +1)	< Row = 4
		
		^						^
		Column = 1				Column = 4
	^								^
	Column = 0						Column = 5
	
	
	 								North (6)
	 						West (5)		East (3)
	 								South (4)
*/		



void coordinator(uint8_t Node_Actual, uint8_t Node_Target, uint8_t * Direction, uint8_t * Distance, uint8_t * Turn_Direction)
{

char y_Target, y_Actual, y_Dist, x_Dist, x_Target, x_Actual;
char y_Direction, x_Direction;


y_Actual = nodes[Node_Actual].yk;
x_Actual = nodes[Node_Actual].xk;

y_Target = nodes[Node_Target].yk;
x_Target = nodes[Node_Target].xk;



y_Dist= y_Target-y_Actual;
x_Dist = x_Target-x_Actual;

// Här börjar de "vanligare" fallen, dvs bara en sväng krävs.

// Kommentarer nedan att gå från 3 till 11 resp 9 till 1 och 13 till 4
if (y_Dist > 0)		//1-3 = -2 (W) resp 1-3 = -2 (W) och 4-0 = 4 (E)
// Här kollas i vilken riktning kolumnerna ska ändras, West eller East?
{
	y_Direction = _east; //Go east	
}
else
{
	y_Direction = _west; //Go west
}                       
	

if (x_Dist > 0 )		//4-0 = 4 (S) resp 0-4 = -4 (N) och 0-2 = -2 (N)
{
	x_Direction = _south; //Go south
}
else
{
	x_Direction = _north; //Go north
}

// Här nedan fattas det lite. 
// Vi ska välja första riktning och hur långt.
// Sedan ska vi välja att svänga höger eller vänster innan vi går sista biten.
// Kanske behövs Direction_Col och Direction_Row istället för bara Direction som använts ovan

// Hitta första riktning och vilket håll att svänga
if (y_Dist > 0 &&(y_Actual == 0 || y_Actual==Y_SIZE+1) )  	//Första och sista kolumn och inte rakt över
{
	*Direction = y_Direction;
    *Distance = y_Dist;
    if((y_Actual == 0) || (x_Target == 0) || (y_Actual == Y_SIZE+1) || (x_Target == X_SIZE+1))
    {
        *Turn_Direction = 'L';
    }
    else
    {
        *Turn_Direction = 'R';
    }
}


if (x_Dist > 0 &&(x_Actual == 0 || x_Actual==X_SIZE+1) )  	//Första och sista rad och inte rakt över
{
	*Direction = x_Direction;
    *Distance = x_Dist;
    if((x_Actual == 0) || (y_Target == 0) || (x_Actual == X_SIZE+1) || (y_Target == Y_SIZE+1))
    {
        *Turn_Direction = 'R';
    }
    else
    {
        *Turn_Direction = 'L';
    }
}


// Nu kan vi låta roboten gå:
// Rikting: Direction
// Avstånd: Distance
// sedan svänga i riktning Turn_Direction
// sedan fortsätta i den riktningen till bitkod för att få nästa mål.


// Slut på "vanligare" fall



if (y_Dist == 0) //Specialfall om det är till och från samma kolumn. T ex Fr 12 till 14.
{
	if (y_Actual == 0) //Västra kolumn. Gå öst, syd/nord, sedan väst, y_Actual = 12
	{
		*Direction = _east; 		//East 
		*Distance = 1;
		if (x_Dist > 0) 	//North  T ex 12 till 14
			{
			//Turn (L);
			*Distance = x_Dist;		//14-12 = 2
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			//Distance = 1;	
			}
		else 				//South t ex 14 till 12
			{
			//Turn (R);
			*Distance = abs(x_Dist);   // 12-14 = -2 gör om till positivt tal för att inte backa.
			//Turn(R);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
	}	
	if (y_Actual == Y_SIZE+1) //Bortre eller östra kolumn. Gå väst, norr/söder, öst. T ex fr 5 till 7
	{
		*Direction = 5; 		//West 
		*Distance = 1;
		if (x_Dist > 0) 	//South
			{
			//Turn (L);
			*Distance = x_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
		else 				//North
			{
			//Turn (R)
			*Distance = abs(x_Dist);
			//Turn(R) //Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
	}
	else
	{
		if(x_Actual==0)
		{
			*Direction = _south; //South Gå tills ny kod hittas eller banan tar slut
		}
	}
}
//Slut på special om det är till och från samma kolumn




if (x_Dist == 0) //Special om det är till och från samma rad
{
	if (x_Actual == 0) //Övre raden. Gå ner, öst/väst, upp
	{
		*Direction = _south; 		//South 
		*Distance = 1;
		if (y_Dist > 0) 	//East
		{
			//Turn (L);
			*Distance = y_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
		else //West
		{
			//Turn (R);
			*Distance = abs(y_Dist);
			//Turn(R);	//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
	}	
	else //undre raden. Gå upp, öst/väst, ner
	{
		*Direction = _north; 		//North 
		*Distance = 1;
		if (y_Dist > 0) 	//West
		{
			//Turn (L);
			*Distance = y_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
		else //East
		{
			//Turn (R)
			*Distance = y_Dist;
			//Turn(R);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
	}
}
//Slut på special om det är till och från samma rad

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

    init(nodes);
    

    uint8_t counter;
    for(counter=1; counter < NODE_SIZE; counter++ )
        printf("N:%d \t x: %d\t y: %d\n",counter,nodes[counter].xk, nodes[counter].yk);

    return 0;
}

#endif // end of Main