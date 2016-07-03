#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

void led_matrix_start(void);
void led_matrix_end(void);

void led_matrix_display_wind(char *wind_kind);
void led_matrix_display_rain(int isRain);
void led_matrix_display_hefei();

void led_matrix_clear_buffer();
void led_matrix_display();
void led_matrix_end();

#endif
