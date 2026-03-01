#pragma once
#include "SFML/Graphics.hpp"

extern sf::Font basicFont;

// dialogs
extern int dialog_margin;				// distance from edge of screen to dialog
extern int dialog_padding;
extern int dialog_title_font_size;
extern int dialog_content_font_size;
extern int dialog_title_rect_height;
extern int dialog_border_width;
extern sf::Color dialog_border_color;
extern sf::Color dialog_title_text_color;
extern sf::Color dialog_title_rect_color;
extern sf::Color dialog_content_rect_color;
extern sf::Color dialog_content_rect_color_2;

void loadTheme();