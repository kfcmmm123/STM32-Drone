#include "App_display.h"

extern Remote_Data remote_data;
extern uint8_t post_buff[TX_PLOAD_WIDTH];

/**
 * @brief  Show bar on the display
 * @param  x: X coordinate of the bar
 * @param  y: Y coordinate of the bar
 * @param  count: Number of bars to show (0-12)
 */
void App_display_show_bar(uint8_t x, uint8_t y, uint8_t count)
{
    if (count < 13)
    {
        OLED_Show_CH(x, y, 12 + count, 12, 1);
    }
}

/**
 * @brief  Initialize the display
 */
void App_display_init(void)
{
    OLED_Init();
}

/**
 * @brief  Show content on the display
 */
void App_display_show(void)
{
    // 1st line: Show title
    OLED_ShowString(LINE1_BEGIN, Y0, "STM32 Drone Alan", 12, 1);

    // 2nd line: Show channel number and battery voltage 
    uint8_t buff[3] = { 0 };
    sprintf((char*)buff, "%03d", CHANNEL);
    OLED_ShowString(LINE2_BEGIN, Y1, buff, 12, 1);

    OLED_ShowString(LINE2_BEGIN + 64, Y1, "V:  ", 12, 1);
    OLED_ShowString(LINE2_BEGIN + 64 + 24, Y1, post_buff, 12, 1);

    // 3rd line: Show throttle value and bars
    OLED_ShowString(LINE3_BEGIN, Y2, "THR:", 12, 1);
    uint8_t count = 0;
    if (remote_data.thr > 500) 
    {
        count = (remote_data.thr - 500) / 41;
        App_display_show_bar(BAR1_BEGIN, Y2, 12);
        App_display_show_bar(BAR2_BEGIN, Y2, count);
    }
    else 
    {
        count = remote_data.thr / 41;
        App_display_show_bar(BAR1_BEGIN, Y2, count);
        App_display_show_bar(BAR2_BEGIN, Y2, 0);
    }
    
    OLED_ShowString(LINE3_BEGIN2, Y2, "ROL:", 12, 1);
    if (remote_data.rol > 500) 
    {
        count = (remote_data.rol - 500) / 41;
        App_display_show_bar(BAR1_BEGIN2, Y2, 12);
        App_display_show_bar(BAR2_BEGIN2, Y2, count);
    }
    else 
    {
        count = remote_data.rol / 41;
        App_display_show_bar(BAR1_BEGIN2, Y2, count);
        App_display_show_bar(BAR2_BEGIN2, Y2, 0);
    }

    // 4th line 
    OLED_ShowString(LINE4_BEGIN, Y3, "YAW:", 12, 1);
    if (remote_data.yaw > 500) 
    {
        count = (remote_data.yaw - 500) / 41;
        App_display_show_bar(BAR1_BEGIN2, Y3, 12);
        App_display_show_bar(BAR2_BEGIN2, Y3, count);
    }
    else 
    {
        count = remote_data.yaw / 41;
        App_display_show_bar(BAR1_BEGIN2, Y3, count);
        App_display_show_bar(BAR2_BEGIN2, Y3, 0);
    }

    OLED_ShowString(LINE4_BEGIN2, Y3, "PIT:", 12, 1);
    if (remote_data.pit > 500) 
    {
        count = (remote_data.pit - 500) / 41;
        App_display_show_bar(BAR1_BEGIN2, Y3, 12);
        App_display_show_bar(BAR2_BEGIN2, Y3, count);
    }
    else 
    {
        count = remote_data.pit / 41;
        App_display_show_bar(BAR1_BEGIN2, Y3, count);
        App_display_show_bar(BAR2_BEGIN2, Y3, 0);
    }

    OLED_Refresh_Gram();
}
