/*
 * log.h
 *
 *      Author: slee
 */

#ifndef LCD_UI_H_
#define LCD_UI_H_

#include "GUI.h"

/**
 *
 * Init LCD display
 *
 *
 */
void lcd_StartUp(void);






void lcd_show_button(int button_id);
void lcd_close_button(int button_id);
void lcd_show_slice(int pos);
void lcd_draw_text(const char *text,int line);
void lcd_draw_text_delay(const char *text,int line,int delay_ms);



/**
 *
 * Display text message
 *
 * @param *format the formatted text
 * @param line the display time
 * @param formatted text arg
 *
 */
template <typename... ArgTs>
void lcd_msg(const char *format,int line, ArgTs... args ) {

		char* buffer = new char[128];
		sprintf(buffer,format,args...);

		lcd_draw_text(buffer,line);

}

/**
 *
 * Display text message period of time
 *
 * @param *format the formatted text
 * @param line the display time
 * @param close_ms time of display
 * @param formatted text arg
 *
 */

template <typename... ArgTs>
void lcd_msg_delay_close(const char *format,int line,int close_ms, ArgTs... args ) {


		lcd_msg(format,line,args...);
		lcd_draw_text_delay("",line,close_ms);


}





#define APP_INFO( x )                        printf x






#endif /* LCD_UI_H_ */
