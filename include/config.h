#ifndef CONFIG_H
#define CONFIG_H
#define MONITOR Serial
#define LCD_SPI_HOST    SPI3_HOST
#define LCD_BCKL_ON_LEVEL 0
#define LCD_BCKL_OFF_LEVEL !LCD_BCKL_ON_LEVEL
#define LCD_PIN_NUM_MOSI 2
#define LCD_PIN_NUM_CLK 3
#define LCD_PIN_NUM_CS 5
#define LCD_PIN_NUM_DC 6
#define LCD_PIN_NUM_RST 1
#define LCD_PIN_NUM_BCKL 10
#define LCD_PANEL esp_lcd_new_panel_st7789
#define LCD_HRES 128
#define LCD_VRES 128
#define LCD_COLOR_SPACE ESP_LCD_COLOR_SPACE_BGR
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 1
#define LCD_GAP_Y 1
#define LCD_MIRROR_X true
#define LCD_MIRROR_Y true
#define LCD_INVERT_COLOR true
#define LCD_SWAP_XY false

#ifdef LCD_SWAP_XY
#if LCD_SWAP_XY
#define LCD_WIDTH LCD_VRES
#define LCD_HEIGHT LCD_HRES
#else
#define LCD_WIDTH LCD_HRES
#define LCD_HEIGHT LCD_VRES
#endif
#else
#define LCD_WIDTH LCD_HRES
#define LCD_HEIGHT LCD_VRES
#endif

#define PIN_NUM_POWER 4
#define PIN_NUM_BUTTON_A            0
#define PIN_NUM_BUTTON_B            47


#endif // CONFIG_H