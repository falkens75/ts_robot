#ifdef AVR_3PI
// The 3pi include file must be at the beginning of any program that
// uses the Pololu AVR library and 3pi.
#include <pololu/3pi.h>

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

#else
#include "stubs.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "orienteering.h"
#include "debug_print.h"
#include "utils.h"
#include "nodesLog.h"
#include "algo.h"

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void initialize()
{
	// This must be called at the beginning of 3pi code, to set up the
	// sensors.  We use a value of 2000 for the timeout, which
	// corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
	pololu_3pi_init(2000);
    set_analog_mode(MODE_10_BIT);
}

/* Speed constants. */
const int           LowSpeedLeft   = 40;
const int           LowSpeedRight  = 40;
const int           cal_delay_time = 30;

/* This function handle the calibration of the line sensors. */
void calibrate_sensors(void)
{
	int i;
	for (i = 0; i<10;i++) 
	{
		set_m1_speed(-LowSpeedLeft);
		set_m2_speed(LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(cal_delay_time);
	}
	stop();
	for (i = 0; i<20;i++) 
	{
		set_m1_speed(LowSpeedLeft);
		set_m2_speed(-LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(cal_delay_time);
	} 
	stop();
	for (i = 0; i<10;i++) 
	{
		set_m1_speed(-LowSpeedLeft);
		set_m2_speed(LowSpeedRight);
		calibrate_line_sensors(IR_EMITTERS_ON);
		delay_ms(cal_delay_time);
	}
	stop();
}

unsigned char buttonPressed;
  
void startup(void)
{

  // set up the 3pi
  initialize();
	
  // Set up the LCD
  lcd_init_printf_with_dimensions(8,2);
  
  clear();
  lcd_goto_xy(0,0);
  print("Press");
  lcd_goto_xy(0,1);
  print("Button A");
  
  wait_for_button(BUTTON_A);

  delay_ms(200);
  // Calibrate the line sensors
  calibrate_sensors();
  // Set baud rate of the serial interface
  serial_set_baud_rate(115200);
  
  /* Indicate calibration completed. */
  play_beep();
  clear();
  lcd_goto_xy(0,0);
  print("Ready");
  lcd_goto_xy(0,1);
  print("Pres ABC");

  while(1)
  {
    buttonPressed = button_is_pressed(ANY_BUTTON);
    if(buttonPressed)
    {
      break;
    }
  }

  orienteering();
  
	// This part of the code is never reached.  A robot should
	// never reach the end of its program, or unpredictable behavior
	// will result as random code starts getting executed.  If you
	// really want to stop all actions at some point, set your motors
	// to 0,0 and run the following command to loop forever:
	//
  stop();
  while(1);
}

// -----------------------------------------------------------------
// -----------------------------------------------------------------

void turn(char dir)
{
        switch(dir)
        {
        case 'L':
                // Turn left.
                set_motors(-40,40);
                delay_ms(400);
                break;
        case 'R':
                // Turn right.
                set_motors(40,-40);
                delay_ms(400);
                break;
        case 'B':
                // Turn around.
                set_motors(80,-80);
                delay_ms(400);
                break;
        case 'S':
                // Don't do anything!
                break;
        }
}


//***********************************************
//***********************************************
//***********************************************

uint8_t read_binary_code()
{
  uint8_t line;
  uint16_t sensors[5]; 
  uint8_t counter = 0;
  uint8_t values[8] = {0,0,0,0,0,0,0,0};
  uint8_t not_read = TRUE;
  uint8_t run = TRUE;
  uint8_t retval = 0;
  while(run)
  {
    read_line(sensors,IR_EMITTERS_ON);

    if((sensors[LEFT_OUTHER_SENSOR] > 500) && not_read)
    {
      values[counter] = 1;
      not_read = FALSE; 
    }

    if((sensors[RIGHT_OUTHER_SENSOR] > 500) && (not_read))
    {
      values[counter] = 0;
      not_read = FALSE; 
    }

  if(!not_read )
  { 
    if((values[counter] == 1) && (sensors[LEFT_OUTHER_SENSOR] < 100)) 
    {
      not_read = TRUE;
      counter++;
    }
   
   if((values[counter] == 0) && (sensors[RIGHT_OUTHER_SENSOR] < 100))
    {
      not_read = TRUE;
      counter++;
    }

    if((sensors[LEFT_INNER_SENSOR] > 700 ) /* && (sensors[RIGHT_INNER_SENSOR] > 700 ) */ )
    {
      run = FALSE;
    }

    if (counter > 7)
      run = FALSE;
    } 
  
follow_line_narrow(sensors);
     } //while

  for (counter=0; counter < 8; counter++)
    retval = retval + (values[counter] *  counter /*(uint8_t) pow(counter, 2) */ )  ;

  return retval;
}



//***********************************************
//***********************************************
//***********************************************

void run_orienteering(uint8_t stop_at_landmark) 
{
  uint16_t line;
  uint16_t values[5];
	uint8_t event;
  uint8_t CountLine = 0;
  uint8_t Direction = _east;
  uint8_t bit_block = 0;
  uint8_t Node_Target = Y_SIZE+1;

  left_led(0);
  right_led(0);  
  

  //Start läge
  //start = Y_SIZE+1;

  while(1)
  {
	/* This is a sample code  */
    line = read_line(values,IR_EMITTERS_ON);
    
  event = drive_forward_act_on_event();

  if(event == crossing) {
    CountLine++;
    set_base_speed(50);
   
  }

  if(event == bitblock) {
    bit_block++;
     set_base_speed(25);
     play_beep();
    Node_Target = read_binary_code();
    clear();
    lcd_goto_xy(0,0);
    printf("NT: %d",Node_Target);
  }

    clear();
    lcd_goto_xy(0,0);
    printf("CL: %d",CountLine); 
    lcd_goto_xy(0,1);
    printf("BB: %d",bit_block); 
    


   if(CountLine == 50)
   {
     CountLine = 0;
    delay_ms(100);
    stop();
    delay_ms(50);
    turn('L');
    delay_ms(50);

   } 
    

//  if(event == offsite)
//  {
//   stop();
//  turn('B');
// }


    //follow_line(line);
	/* End of sample code*/
	
	/****************************************************************************
	* Here you should add your code												*
	****************************************************************************/
	
	/**************************End of your code *********************************/
  }
}

uint16_t log_buffer[500];

void print_log()
{
  uint16_t i, temp_val;
  int8_t j;
  char print_buf[5];
  uint8_t digit;
  
  serial_send_blocking("  --- START ---\n", 16);
  
  for(i = 0; i < 500; i++)
  {
    temp_val = log_buffer[i]; 
	if((i % 5) == 0)
    {
      serial_send_blocking(" ", 1);
    }
    for(j = 3; j >= 0; j--)
    {
      digit = temp_val % 10; 
      if(temp_val == 0 && j != 3)
      {
        print_buf[j] = ' ';
      }
      else
      {
        print_buf[j] = '0' + digit;
      }
      temp_val /= 10;
    }
    serial_send_blocking(print_buf, 4);
    if((i % 5) == 4)
    {
      serial_send_blocking("\n", 1);
    }
    else
    {
      serial_send_blocking(", ", 2);
    }
  }
  
  serial_send_blocking("  --- END ---\n", 14);
  
}

void record_bonus_marker(void)
{
  uint16_t log_index = 0;
  uint8_t sensor_index;
  uint8_t start_logging = 0;
  uint16_t line;
  uint16_t values[5];

  delay_ms(1000);

  clear_integral();
  while(1)
  {
    line = read_line(values,IR_EMITTERS_ON);
    follow_line(line);
	/*******************************************************************************************************************
	* Suggestion: You might want to add something here which consumes some time (similar to your intended orienteering *
	* code) otherwise the logging runs a bit fast                                                                      *
	********************************************************************************************************************/
    if((values[1] > 350 && values[3] > 350) || (values[0] > 250 && values[4] > 250))
    {
       start_logging = 1;
    }
    if(start_logging)
    {
      for(sensor_index = 0; sensor_index < 5; sensor_index++)
      {
        log_buffer[log_index++] = values[sensor_index];
      }
    }
    if(log_index >= 500)
    {
      print_two_messages("Buf full", "Press A");
      break;
    }
  }
  
  while(1)
  {
    if(button_is_pressed(BUTTON_A))
    {
      print_two_messages("Printing","");
      print_log();
      delay_ms(500);
      print_two_messages("Resend", "Press A");
    }
  }
}

void orienteering(void)
{

  if(buttonPressed == BUTTON_A)
  {
	/* Read the values measured by the sensors */
   /* When displayed "Press ABC" -> press button A to enter this function.
	  Put the robot over a black and white pattern.
	  Then press again button A to start the measurement.
	  The values measured by the most left two sensors will be displayed.
      Press B to view the values measured by the next two sensors.
      Press B again to see the value measured by the last sensor.
	  If you press B again the process will be repeated until you reset the robot.
   */
    sensor_printing();
  }
  else if(buttonPressed == BUTTON_B)
  {
	/* Run the challenge */
    /* When displayed "Press ABC" -> press button B to enter this function. */
    /* Add you application software to this function in order to solve the challenge */
    run_orienteering(0);
  }
  else // BUTTON_C
  {
	/* Run the robot for a short while and record the values measured by the sensors  */
	/* When displayed "Press ABC" -> press button C to enter this function.
	This function will move the robot forward along the line until the following condition is fulfilled:
	Sensor 1 and 3 measures a value larger than 350
	OR 
	Sensor 0 and 4 measures a value larger than 250.
    Then it will continue forward for approx. 3-4 cm (depending on your speed in "follow_line" function)
	and the values measured by the sensors will be recorded to a buffer. When the distance was traveled 
	the message "Buf full Press A" will come up. If you press A the recorded data will be transmitted over 
	the serial port to a terminal software running on the PC while the message "Printing" will be displayed.
    Then the message "Resend Press A" will be displayed. If you press button A the same buffer will be 
    retransmitted to the PC terminal. For more information related to RS 232 communication setup please
    refer to the user's guide.
    */
    record_bonus_marker();
  }
  stop_with_message("END");

}

// -----------------------------------------------------------------
// -----------------------------------------------------------------
