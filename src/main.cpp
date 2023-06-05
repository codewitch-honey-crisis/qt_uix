#include <Arduino.h>

#include "config.h"
#define LCD_IMPLEMENTATION
#include <gfx.hpp>

#include "lcd_init.h"
using namespace gfx;
#include <uix.hpp>
using namespace uix;
#include <button.hpp>
using namespace arduino;
// SVG converted to header using
// https://honeythecodewitch.com/gfx/converter
#include "bee_icon.hpp"
static const svg_doc& svg = bee_icon;
// downloaded from fontsquirrel.com and header generated with
// https://honeythecodewitch.com/gfx/generator
// #include "fonts/Rubik_Black.hpp"
// #include "fonts/Telegrama.hpp"
#include "fonts/OpenSans_Regular.hpp"
static const open_font& text_font = OpenSans_Regular;

// declare the format of the screen
using screen_t = screen<LCD_WIDTH, LCD_HEIGHT, rgb_pixel<16>>;
// declare the control types to match the screen
using label_t = label<typename screen_t::pixel_type>;
using svg_box_t = svg_box<typename screen_t::pixel_type>;
// for access to RGB565 colors which LCDs and the main screen use
using color16_t = color<rgb_pixel<16>>;
// for access to RGBA8888 colors which controls use
using color32_t = color<rgba_pixel<32>>;
// declare the buttons
using button_a_t = int_button<PIN_NUM_BUTTON_A, 10, true>;
using button_b_t = int_button<PIN_NUM_BUTTON_B, 10, true>;

// UIX allows you to use two buffers for maximum DMA efficiency
// you don't have to, but performance is significantly better
// declare 64KB across two buffers for transfer
// RGB mode is the exception. We don't need two buffers
// because the display works differently.

constexpr static const int lcd_buffer_size = 32 * 1024;
uint8_t lcd_buffer1[lcd_buffer_size];
uint8_t lcd_buffer2[lcd_buffer_size];

// our svg doc for svg_box
svg_doc doc;
// the main screen
screen_t main_screen(sizeof(lcd_buffer1),
                     lcd_buffer1,
                     lcd_buffer2);
// the controls
label_t test_label(main_screen);
svg_box_t test_svg(main_screen);
button_a_t button_a;
button_b_t button_b;

// button callbacks
void button_a_on_click(bool pressed, void* state) {
    if (pressed) {
        test_label.text_color(color32_t::red);
    } else {
        test_label.text_color(color32_t::blue);
    }
}
void button_b_on_click(bool pressed, void* state) {
    if (pressed) {
        main_screen.background_color(color16_t::light_green);
    } else {
        main_screen.background_color(color16_t::white);
    }
}

// tell UIX the DMA transfer is complete
static bool lcd_flush_ready(esp_lcd_panel_io_handle_t panel_io,
                            esp_lcd_panel_io_event_data_t* edata,
                            void* user_ctx) {
    main_screen.set_flush_complete();
    return true;
}

// tell the lcd panel api to transfer the display data
static void uix_flush(point16 location,
                      typename screen_t::bitmap_type& bmp,
                      void* state) {
    int x1 = location.x,
        y1 = location.y,
        x2 = location.x + bmp.dimensions().width - 1,
        y2 = location.y + bmp.dimensions().height - 1;
    lcd_panel_draw_bitmap(x1, y1, x2, y2, bmp.begin());
}

// initialize the screen and controls
void screen_init() {
    test_label.bounds(srect16(spoint16(0, 5), ssize16(200, 60))
                          .center_horizontal(main_screen.bounds()));
    test_label.text_color(color32_t::blue);
    test_label.text_open_font(&text_font);
    test_label.text_line_height(45);
    test_label.text_justify(uix_justify::center);
    test_label.round_ratio(NAN);
    test_label.padding({8, 8});
    test_label.text("Hello!");
    // make the backcolor transparent
    auto bg = color32_t::black;
    bg.channel<channel_name::A>(0);
    test_label.background_color(bg);
    // and the border
    test_label.border_color(bg);
    test_svg.bounds(srect16(spoint16(0, test_label.bounds().y2 + 1),
                            ssize16(60, 60))
                        .center_horizontal(main_screen.bounds()));
    test_svg.doc(&svg);
    main_screen.background_color(color16_t::white);
    main_screen.register_control(test_label);
    main_screen.register_control(test_svg);
    main_screen.on_flush_callback(uix_flush);
}

// set up the hardware
void setup() {
    MONITOR.begin(115200);
    MONITOR.println("Booting");
    pinMode(PIN_NUM_POWER, OUTPUT);
    digitalWrite(PIN_NUM_POWER, HIGH);
    lcd_panel_init(sizeof(lcd_buffer1), lcd_flush_ready);
    screen_init();
    button_a.initialize();
    button_a.on_pressed_changed(button_a_on_click);
    button_b.initialize();
    button_b.on_pressed_changed(button_b_on_click);
}

// keep our stuff up to date and responsive
void loop() {
    button_a.update();
    button_b.update();
    main_screen.update();
    vTaskDelay(5);
}
