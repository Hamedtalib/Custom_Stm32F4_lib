/**
 * ECE4510 Final Project
 *
 * @author      Tyler Thompson
 * @date        4/5/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Custom_STM32F4_lib\Timing_Delay.h"
#include "Custom_STM32F4_lib\DIP_Switch.h"
#include "Custom_STM32F4_lib\Serial_LCD.h"
#include "Custom_STM32F4_lib\Motor_Encoder.h"
#include "Custom_STM32F4_lib\PWM_Motor_Control.h"
#include "Custom_STM32F4_lib\UltraSonicSensor.h"
#include "Custom_STM32F4_lib\LED_Panel_X8.h"
#include "Custom_STM32F4_lib\Buzzer.h"


/* Private variables ---------------------------------------------------------*/
uint32_t encoder_freq = 0;
uint32_t motor_duty = 0;
uint32_t round_freq = 0;
uint32_t lower_freq = 0;
uint32_t ultrasonic_dist = 0;
uint8_t start_dist = 3; // inches
uint8_t stop_dist = 12; // inches
uint8_t motor_direction = 3;
uint32_t lcd_counter = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t frequency_to_duty_conversion(uint32_t frequency, uint8_t old_duty);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {
        delay_init();

	/* Switch initialization */
	dip_init();

	/* LCD initialization */
	slcd_init();	

	/* Ultrasonic sensor initialization */
	ultrasonic_init();

	/* LED initialization */
	led_init();

	/* buzzer initialization */
	buz_init();
        
        /* motor control initialization */
	pwmmotor_init();
        pwmmotor_disable(0); // disable forward
        pwmmotor_disable(1); // disable reverse
        
        /* motor encoder initialization */
	encoder_init();	
        
        
	while (1) { 
          lcd_counter++;
		// Switch 0 -> START
		// Switch 1 -> STOP
		// LED -> led 0
		// IR LED -> led 1
                // START Distance = 1in
                // STOP Distance = 10 in

                // determine if box it at start or stop position and set direction
                ultrasonic_dist = ultrasonic_get_distance_in();
                if(ultrasonic_dist == start_dist){ // forward
                  motor_direction = 0;
                }
                else if (ultrasonic_dist == stop_dist) { // reverse
                  motor_direction = 1;
                }
                else {
                  motor_direction = 3; // disabled
                }
                
                // LCD update
                if(lcd_counter == 840000) { // approximate update speed of 100ms
                  lcd_counter = 0;
                  slcd_set_direction(2);
                  slcd_set_dist_start(ultrasonic_dist - start_dist);
                  slcd_set_dist_stop(stop_dist - ultrasonic_dist);
                  slcd_send_update();
                }
                
		/* START switch activated: active low */
		if(dip_get_switch_state(0) == 0 || motor_direction == 1 || motor_direction == 0 ) {
                  
                        // if the STOP switch is active, don't start
                        if(dip_get_switch_state(1) == 0){
                          continue;
                        }
                                         
                        // if no direction can be determined by ultrasonic, assume forward
                        if ( motor_direction == 3) {
                          motor_direction = 0;
                        }
 
                        slcd_set_dist_start(ultrasonic_dist - start_dist);
                        slcd_set_dist_stop(stop_dist - ultrasonic_dist);
                        slcd_set_direction(motor_direction);
                        slcd_send_update();
                        
			// Turn on buzzer at 5.5Khz
			buz_start_sound(5500);

			// Blink LED 6 times at 1 blink/second
			led_blink(0, 6, 1000);

			// turn off buzzer
			buz_stop_sound();

			// turn on IR LED
			led_set_state(1, 1);

			// Turn on motor at frequency = 30KHz, Duty = 50%
			motor_duty = 50;
			pwmmotor_enable(motor_direction); // enable motor

			uint8_t control_loop = 1;
                        uint32_t loop_counter = 0;
			while(control_loop == 1) {
                          
                          // LCD update
                          lcd_counter++;
                          if(lcd_counter == 840000){ // approximate update speed of 100ms
                            lcd_counter = 0;
                            slcd_set_dist_start(ultrasonic_dist - start_dist);
                            slcd_set_dist_stop(stop_dist - ultrasonic_dist);
                            slcd_send_update();  
                          }
                                                    
                          // slow update
                          loop_counter++;
                          if(loop_counter == 84000) {  // approximate update speed of 10ms
                                loop_counter = 0;
                                
                                ultrasonic_dist = ultrasonic_get_distance_in();                                                              
                                
                                // set the duty cycle of the motor control based on the frequency of the motor encoder
                                encoder_freq = encoder_get_frequency();
                                motor_duty = frequency_to_duty_conversion(encoder_freq, motor_duty);
                                pwmmotor_set_duty(motor_duty, motor_direction);
                                                                
                                // Check ultrasonic sensor distance
				if( (ultrasonic_dist == stop_dist && motor_direction == 0) || (ultrasonic_dist == start_dist && motor_direction == 1) ){
					control_loop = 0;
                                        slcd_set_dist_start(ultrasonic_dist - start_dist);
                                        slcd_set_dist_stop(stop_dist - ultrasonic_dist);
                                        slcd_set_direction(2);
                                        slcd_send_update();
				}
                          }
				
                          // fast update - every cycle                          
				// Check the STOP switch: active low
				if(dip_get_switch_state(1) == 0){
					control_loop = 0;
                                        slcd_set_dist_start(ultrasonic_dist - start_dist);
                                        slcd_set_dist_stop(stop_dist - ultrasonic_dist);
                                        slcd_set_direction(2);
                                        slcd_send_update();
				}
			}

			/* object is at the end of the conveyor belt */

			// disable motor
			pwmmotor_set_duty(0, 0);
                        pwmmotor_set_duty(0, 1);
			pwmmotor_disable(0); // disable forward
                        pwmmotor_disable(1); // disable reverse
                        
			// Turn on the buzzer at 3.5KHz
			buz_start_sound(3500);

			// Blink LED 10 times at 2 blinks/second
			led_blink(0, 10, 500);

			// Turn off the buzzer
			buz_stop_sound();
                        
                        // turn off IR LED
			led_set_state(1, 0);
                        
                        lcd_counter = 0;

		}
	}
}

/**
  * @brief  Determine the motor duty cycle based on the encoder frequency
  * @param  frequency - Frequency from the motor encoder
  * @retval duty - The duty cycle to set the motor
  */
uint32_t frequency_to_duty_conversion(uint32_t conv_frequency, uint8_t old_duty) {
        
        /* round frequency on tens value */
        lower_freq = (conv_frequency / 10 ) % 10;
	round_freq = conv_frequency / 100;
        if(lower_freq >= 5){
          round_freq++;
        }
        
	uint8_t new_duty = old_duty;

	if(round_freq == 47) {
		new_duty = 90;
	}
	else if(round_freq == 49) {
		new_duty = 80;
	}
	else if(round_freq == 51) {
		new_duty = 70;
	}
	else if(round_freq == 53) {
		new_duty = 60;
	}
	else if(round_freq == 55) {
		new_duty = 50;
	}
	else if(round_freq == 57) {
		new_duty = 40;
	}
	else if(round_freq == 59) {
		new_duty = 30;
	}
	else if(round_freq == 61) {
		new_duty = 20;
	}
	else if(round_freq == 63) {
		new_duty = 10;
	}
	return new_duty;
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{

  /* Infinite loop */
  while (1)
  {}
}
#endif

