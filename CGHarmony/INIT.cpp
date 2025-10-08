#include "INIT.h"
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <fstream>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "PCharacter.h"
#include "Bullet.h"
#include "Game.h"

game_parameters::game_parameters(sf::RenderWindow& window, std::string& record) {
    unsigned WINDOW_SIZE_X = 600;
    unsigned WINDOW_SIZE_Y = 400;

    this->background_texture.loadFromFile("resources/dark_forest.png",
        sf::IntRect(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y));
    this->bg.setTexture(background_texture);

    this->start_button.setPointCount(6);
    this->start_button.setPoint(0, sf::Vector2f(225.f, 185.f));
    this->start_button.setPoint(1, sf::Vector2f(375.f, 185.f));
    this->start_button.setPoint(2, sf::Vector2f(345.f, 200.f));
    this->start_button.setPoint(3, sf::Vector2f(375.f, 215.f));
    this->start_button.setPoint(4, sf::Vector2f(225.f, 215.f));
    this->start_button.setPoint(5, sf::Vector2f(255.f, 200.f));
    this->start_button.setFillColor(sf::Color(20, 180, 50));  // green

    this->record_holder.setPointCount(4);
    this->record_holder.setPoint(0, sf::Vector2f(180.f, 300.f));
    this->record_holder.setPoint(1, sf::Vector2f(420.f, 300.f));
    this->record_holder.setPoint(2, sf::Vector2f(420.f, 350.f));
    this->record_holder.setPoint(3, sf::Vector2f(180.f, 350.f));
    this->record_holder.setFillColor(sf::Color(10, 140, 40));  // dark green

    this->font.loadFromFile("resources/Frostbite.ttf");
    this->start_text.setFont(font);
    this->start_text.setString("Start");
    this->start_text.setCharacterSize(28);
    this->start_text.setFillColor(sf::Color(0, 0, 0));  // black
    this->start_text.setPosition(270, 180);

    std::fstream records_file;
    records_file.open("resources/records.txt");
    getline(records_file, record);
    if (record == "") {
        record.assign("0");
        records_file << "0" << std::endl;
    }
    int len = record.length() + 1;
    char* buffer = new char[len];
    snprintf(buffer, len, "%.2f", stof(record));  // formatting to 2 digits after dot
    record.assign(buffer);
    records_file.close();

    this->record_text.setFont(font);
    this->record_text.setString("Old record: " + record + " s");
    this->record_text.setCharacterSize(24);
    this->record_text.setFillColor(sf::Color(245, 222, 179));  // rye
    this->record_text.setPosition(185, 310);

    this->BULLETS_AMMOUNT = 8;
    bullets = std::vector<Bullet>(BULLETS_AMMOUNT);
    for (int i = 0; i < BULLETS_AMMOUNT; i++) {
        bullets[i].setId(i);
    }

    this->player;
    player.x_coord = 282;  // in the middle of grass field
    player.y_coord = 350;

    this->healthbar.setSize(sf::Vector2f(player.health, 20));
    this->healthbar.setFillColor(sf::Color::Red);
    this->healthbar.setPosition(sf::Vector2f(WINDOW_SIZE_X - player.health, 0));

    this->clock;
    this->t_previous = sf::seconds(0);
    this->t_current = sf::seconds(0);
    this->t_delta;

    // main game cycle
    this->running = true;
}