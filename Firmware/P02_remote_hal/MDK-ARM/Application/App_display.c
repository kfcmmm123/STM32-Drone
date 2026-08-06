#include "App_display.h"

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
    OLED_ShowString(0, 0, "Hello, World!", 16, 1);
}
