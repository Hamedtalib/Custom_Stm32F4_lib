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
char *test_val;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t frequency_to_duty_conversion(uint32_t frequency);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {

 	/* delay initialization */
	delay_init();

	/* Switch initialization */
	dip_init();

	/* LCD initialization */
	slcd_init();

	/* motor encoder initialization */
	encoder_init();

	

	/* Ultrasonic sensor initialization */
	ultrasonic_init();

	/* LED initialization */
	led_init();

	/* buzzer initialization */
	buz_init();
        
        /* motor control initialization */
	pwmmotor_init();

        slcd_send_string("Ready");

	while (1) { 
		// Switch 0 -> START
		// Switch 1 -> STOP
		// LED -> led 0
		// IR LED -> led 1


		/* START switch activated: active low */
		if(dip_get_switch_state(0) == 0) {
                        slcd_send_string("START");
			// Turn on buzzer at 5.5Khz
			buz_start_sound(5500);

			// Blink LED 6 times at 1 blink/second
			led_blink(0, 6, 1000);

			// turn off buzzer
			buz_stop_sound();

			// turn on IR LED
			led_set_state(1, 1);

			// Turn on motor at frequency = 30KHz, Duty = 50%
			pwmmotor_set_duty(50);
			pwmmotor_enable();

			uint8_t control_loop = 1;
                        uint32_t loop_counter = 0;
			while(control_loop == 1) {
                          // update LCD 
                          loop_counter++;
                          if(loop_counter == 10000) {
                                slcd_send_string(slcd_int_to_string(ultrasonic_get_distance_in()) + 'i' + 'n');
                                loop_counter = 0;
                                pwmmotor_set_duty(frequency_to_duty_conversion(encoder_get_frequency()));
                          }
				// set the duty cycle of the motor control based on the frequency of the motor encoder
				//pwmmotor_set_duty(frequency_to_duty_conversion(encoder_get_frequency()));

				// Check the STOP switch: active low
				if(dip_get_switch_state(1) == 0){
					control_loop = 0;
                                        slcd_send_string("STOP");
				}
			}

			/* object is at the end of the conveyor belt */

			// disable motor
			pwmmotor_set_duty(0);
			pwmmotor_disable();

			// Turn on the buzzer at 3.5KHz
			buz_start_sound(3500);

			// Blink LED 10 times at 2 blinks/second
			led_blink(0, 10, 500);

			// Turn off the buzzer
			buz_stop_sound();
                        
                        // turn off IR LED
			led_set_state(1, 0);

		}
	}
}

/**
  * @brief  Determine the motor duty cycle based on the encoder frequency
  * @param  frequency - Frequency from the motor encoder
  * @retval duty - The duty cycle to set the motor
  */
uint32_t frequency_to_duty_conversion(uint32_t frequency) {
	uint32_t round_freq = frequency / 100;
	uint8_t duty = 50;

	if(round_freq == 47) {
		duty = 90;
	}
	else if(round_freq == 49) {
		duty = 80;
	}
	else if(round_freq == 51) {
		duty = 70;
	}
	else if(round_freq == 53) {
		duty = 60;
	}
	else if(round_freq == 55) {
		duty = 50;
	}
	else if(round_freq == 57) {
		duty = 40;
	}
	else if(round_freq == 59) {
		duty = 30;
	}
	else if(round_freq == 61) {
		duty = 20;
	}
	else if(round_freq == 63) {
		duty = 10;
	}
	return duty;
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

