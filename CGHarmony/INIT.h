#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "PCharacter.h"
#include "Bullet.h"
#ifndef __INIT_H__
#define __INIT_H__
class game_parameters {
public:
    game_parameters(sf::RenderWindow& window, std::string& record);
    PCharacter player;
    sf::Texture background_texture;
    sf::Sprite bg;
    sf::RectangleShape healthbar;
    sf::Clock clock;
    sf::Time t_previous;
    sf::Time t_current;
    sf::Time t_delta;
    sf::ConvexShape start_button;
    sf::ConvexShape record_holder;
    sf::Font font;
    sf::Text start_text;
    sf::Text record_text;
    unsigned BULLETS_AMMOUNT;
    std::vector<Bullet> bullets;
    bool running;
};
#endif