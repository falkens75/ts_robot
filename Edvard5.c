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

char Col_Target, Col_Actual, Col_Dist, Row_Dist, Row_Target, Row_Actual, Node_Actual, Node_Target, Direction, Distance, Col_Max, Row_Max;

int main ()
{
Node_Actual = 4;
Node_Target = 9;
Col_Target = 6;  //Hittepåvärden för att kolla koden
Col_Actual = 5;

Col_Dist= abs(Col_Target-Col_Actual);	//Är det rätt med abs (...) här ???
Row_Dist = abs(Row_Target-Row_Actual);

// Här börjar de "vanligare" fallen, dvs bara en sväng krävs.

// Kommentarer nedan att gå från 3 till 11 resp 9 till 1 och 13 till 4
if (Col_Dist > 0)		//1-3 = -2 (W) resp 1-3 = -2 (W) och 4-0 = 4 (E)
// Här kollas i vilken riktning kolumnerna ska ändras, West eller East?
{
	Direction = _east; //Go east	
}
else
{
	Direction = _west; //Go west
}                       
	

if (Row_Dist > 0 )		//4-0 = 4 (S) resp 0-4 = -4 (N) och 0-2 = -2 (N)
{
	Direction = _south; //Go south
}
else
{
	Direction = _north; //Go north
}

// Här nedan fattas det lite. 
// Vi ska välja första riktning och hur långt.
// Sedan ska vi välja att svänga höger eller vänster innan vi går sista biten.
// Kanske behövs Direction_Col och Direction_Row istället för bara Direction som använts ovan

// Hitta första riktning
if (Col_Actual == 0 || Col_Actual==Col_Max+1)  	//Första och sista kolumn
{
	//Gå först East/West
}
if (Row_Actual == 0 || Row_Actual == Row_Max+1) 	//Första och sista rad. Else-sats istället?
{
	//Gå först South/North
}

// Slut på "vanligare" fall



if (Col_Dist == 0) //Specialfall om det är till och från samma kolumn. T ex Fr 12 till 14.
{
	if (Col_Actual == 0) //Västra kolumn. Gå öst, syd/nord, sedan väst, Col_Actual = 12
	{
		Direction = _east; 		//East 
		Distance = 1;
		if (Row_Dist > 0) 	//North  T ex 12 till 14
			{
			//Turn (L);
			Distance = Row_Dist;		//14-12 = 2
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			//Distance = 1;	
			}
		else 				//South t ex 14 till 12
			{
			//Turn (R);
			Distance = abs(Row_Dist);   // 12-14 = -2 gör om till positivt tal för att inte backa.
			//Turn(R);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
	}	
	if (Col_Actual == Col_Max+1) //Bortre eller östra kolumn. Gå väst, norr/söder, öst. T ex fr 5 till 7
	{
		Direction = 5; 		//West 
		Distance = 1;
		if (Row_Dist > 0) 	//South
			{
			//Turn (L);
			Distance = Row_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
		else 				//North
			{
			//Turn (R)
			Distance = abs(Row_Dist);
			//Turn(R) //Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
			}
	}
	else
	{
		if(Row_Actual==0)
		{
			Direction = _south; //South Gå tills ny kod hittas eller banan tar slut
		}
	}
}
//Slut på special om det är till och från samma kolumn




if (Row_Dist == 0) //Special om det är till och från samma rad
{
	if (Row_Actual == 0) //Övre raden. Gå ner, öst/väst, upp
	{
		Direction = _south; 		//South 
		Distance = 1;
		if (Col_Dist > 0) 	//East
		{
			//Turn (L);
			Distance = Col_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
		else //West
		{
			//Turn (R);
			Distance = abs(Col_Dist);
			//Turn(R);	//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
	}	
	else //undre raden. Gå upp, öst/väst, ner
	{
		Direction = _north; 		//North 
		Distance = 1;
		if (Col_Dist > 0) 	//West
		{
			//Turn (L);
			Distance = Col_Dist;
			//Turn(L);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
		else //East
		{
			//Turn (R)
			Distance = Col_Dist;
			//Turn(R);		//Gå tills ny kod hittas eller banan tar slut
			// Distance = 1;	
		}
	}
}
//Slut på special om det är till och från samma rad


}
