#include "Theme.hpp"
#include "SFML/Graphics.hpp"
#include "DebugLog.hpp"

sf::Font basicFont;

// dialogs
int dialog_margin;				// distance from edge of screen to dialog
int dialog_padding;
int dialog_title_font_size;
int dialog_content_font_size;
int dialog_title_rect_height;
int dialog_border_width;
sf::Color dialog_border_color;
sf::Color dialog_title_text_color;
sf::Color dialog_title_rect_color;
sf::Color dialog_content_rect_color;
sf::Color dialog_content_rect_color_2;

void loadTheme() {

	if (!basicFont.openFromFile("C:/Windows/Fonts/arial.ttf")) {
		DebugError(L"Failed to load basic font");
		exit(0);
	}

	// dialogs
	dialog_margin = 8;
	dialog_padding = 8;
	dialog_title_font_size = 17;
	dialog_content_font_size = 13;
	dialog_title_rect_height = 32;
	dialog_border_width = 1;
	dialog_border_color = sf::Color(63, 63, 63);
	dialog_title_text_color = sf::Color(191, 191, 191);
	dialog_title_rect_color = sf::Color(15, 15, 15);
	dialog_content_rect_color = sf::Color(31, 31, 31);
	dialog_content_rect_color_2 = sf::Color(47, 47, 47);


}
