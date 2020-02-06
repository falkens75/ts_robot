// standard test: gcc Edvard5.c -o edvard -Wall -pedantic
// This function show the path from one position to the next.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


#include "algo.h"

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



void coordinator(uint8_t Node_Actual, uint8_t Node_Target, uint8_t * direction, uint8_t * distance, uint8_t * turn_direction)

char y_Target, y_Actual, y_Dist, x_Dist, x_Target, x_Actual, Node_Actual, Node_Target, Direction, Distance, y_Max, x_Max;
char y_Direction, x_Direction;


y_Actual = nodes[Node_Actual].yk;
x_Actual = nodes[Node_Actual].xk;

y_Target = nodes[Node_Target].yk;
x_Target = nodes[Node_Target].xk;



y_Dist= y_Target-y_Actual;
x_Dist = x_Target-x_Actual);

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
if (y_Dist > 0 &&(y_Actual == 0 || y_Actual==y_Max+1) )  	//Första och sista kolumn och inte rakt över
{
	Direction = y_Direction
    Distance = y_Dist
    if(y_Actual == 0 ||x_Target == 0) || ((y_Actual == y_Max+1 || x_Target == x_Max+1)
    {
        Turn_Direction = L;
    }
    else
    {
        Turn_Direction = R;
    }
}


if (x_Dist > 0 &&(x_Actual == 0 || x_Actual==x_Max+1) )  	//Första och sista rad och inte rakt över
{
	Direction = x_Direction;
    Distance = x_Dist;
    if(x_Actual == 0 ||y_Target == 0) || ((x_Actual == x_Max+1 || y_Target == y_Max+1)
    {
        Turn_Direction = R;
    }
    else
    {
        Turn_Direction = L;
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
		Direction = _east; 		//East 
		Distance = 1;
		if (x_Dist > 0) 	//North  T ex 12 till 14
			{
			//Turn (L);
			Distance = x_Dist;		//14-12 = 2
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			//Distance = 1;	
			}
		else 				//South t ex 14 till 12
			{
			//Turn (R);
			Distance = abs(x_Dist);   // 12-14 = -2 gör om till positivt tal för att inte backa.
			//Turn(R);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
	}	
	if (y_Actual == y_Max+1) //Bortre eller östra kolumn. Gå väst, norr/söder, öst. T ex fr 5 till 7
	{
		Direction = 5; 		//West 
		Distance = 1;
		if (x_Dist > 0) 	//South
			{
			//Turn (L);
			Distance = x_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
		else 				//North
			{
			//Turn (R)
			Distance = abs(x_Dist);
			//Turn(R) //Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
	}
	else
	{
		if(x_Actual==0)
		{
			Direction = _south; //South Gå tills ny kod hittas eller banan tar slut
		}
	}
}
//Slut på special om det är till och från samma kolumn




if (x_Dist == 0) //Special om det är till och från samma rad
{
	if (x_Actual == 0) //Övre raden. Gå ner, öst/väst, upp
	{
		Direction = _south; 		//South 
		Distance = 1;
		if (y_Dist > 0) 	//East
		{
			//Turn (L);
			Distance = y_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
		else //West
		{
			//Turn (R);
			Distance = abs(y_Dist);
			//Turn(R);	//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
	}	
	else //undre raden. Gå upp, öst/väst, ner
	{
		Direction = _north; 		//North 
		Distance = 1;
		if (y_Dist > 0) 	//West
		{
			//Turn (L);
			Distance = y_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
		else //East
		{
			//Turn (R)
			Distance = y_Dist;
			//Turn(R);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
	}
}
//Slut på special om det är till och från samma rad


}
