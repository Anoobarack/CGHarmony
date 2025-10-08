#include <string>
#include <iostream>
#include <SFML/Window.hpp>
#include <fstream>
#include "Game.h"
#include <SFML/Window/Keyboard.hpp>
#include "functions.h"
#include "INIT.h"

std::string game(game_parameters& INIT, sf::RenderWindow& window, std::string& record, bool& paused) {

    // turning player sprite and switching movement direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (INIT.player.x_coord > 0)
        {
            INIT.player.speed = -INIT.player.SPEED_CONST;
        }
        else
            INIT.player.speed = 0;
        if (INIT.player.orientation == "right")
        {
            INIT.player.turnLeft();
        }

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (INIT.player.x_coord < 563)  //600-37, 37 is character sprite width
        {
            INIT.player.speed = INIT.player.SPEED_CONST;
        }
        else
            INIT.player.speed = 0;
        if (INIT.player.orientation == "left")
        {
            INIT.player.turnRight();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && !paused)
    {
        paused = true;
        std::cout << paused << std::endl;
        INIT.start_text.setString("Paused");
        INIT.start_text.setPosition(260, 180);
        INIT.record_text.setString("Left shift to unpause");
        for (int i = 0; i < INIT.BULLETS_AMMOUNT; i++) {
            INIT.bullets[i].pause_buffer = INIT.bullets[i].clck.getElapsedTime();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && paused)
    {
        paused = false;
        std::cout << paused << std::endl;

    }

    INIT.t_current = INIT.clock.getElapsedTime();
    INIT.t_delta = INIT.t_current - INIT.t_previous;
    INIT.t_previous = INIT.t_current;

    if (paused) {
        window.draw(INIT.start_button);
        window.draw(INIT.record_holder);
        window.draw(INIT.start_text);
        window.draw(INIT.record_text);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // check start button position
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2u windowSize = window.getSize();
            if (mousePosition.x > windowSize.x / 2 * 0.75
                && mousePosition.x < windowSize.x / 2 * 1.25) {
                if (mousePosition.y > windowSize.y / 2 * 0.925
                    && mousePosition.y < windowSize.y / 2 * 1.075) {
                    paused = false;
                }
            }
        }
    }

    if (!paused)
    {
        /*
        Drawing processes
        what's on back goes first into window.draw(drawable)
        window.draw(bg) was used in main
        after all window.display() is called
        */

        //processing player character

        INIT.player.speed = checkBorders(INIT.player.speed, INIT.player.x_coord);

        INIT.player.current_frame += INIT.t_delta.asSeconds();
        INIT.player.nextSprite();
        if (INIT.player.isHurt) {
            INIT.player.hurt_clock += INIT.t_delta.asSeconds();
            if (INIT.player.hurt_clock > 1)
                INIT.player.toRunning();

        }
        INIT.player.x_coord = moveCharacter(INIT.t_delta, INIT.player.x_coord, INIT.player.speed);
        INIT.player.sprite.setPosition(INIT.player.x_coord + INIT.player.x_pos_offset,
            INIT.player.y_coord);
        int len = 0;

        // processing bullets
        for (int i = 0; i < INIT.BULLETS_AMMOUNT; i++) {
            // render bullets' movement
            INIT.bullets[i].pause_buffer += INIT.t_delta;
            std::vector<int> ret = updateCoordinates_parabolic(INIT.bullets[i], INIT.clock);
            INIT.bullets[i].x_coord = ret[0];
            INIT.bullets[i].y_coord = ret[1];
            INIT.bullets[i].sprt.setPosition(INIT.bullets[i].x_coord, INIT.bullets[i].y_coord);
            window.draw(INIT.bullets[i].sprt);

            if (checkCollision(INIT.bullets[i], INIT.player)) {
                INIT.player.health -= 15;  // hit
                INIT.player.toHurt();
                if (INIT.player.health <= 0) {  // end of game
                    std::fstream records_file;
                    records_file.open("resources/records.txt");
                    INIT.record_text.setString("Old record: " + record + " s");
                    if (INIT.t_current.asSeconds() > stof(record)) {
                        record = fixedFloat(INIT.t_current.asSeconds());
                        INIT.record_text.setString("New record: " + record + " s");
                        records_file << record << std::endl;
                    }
                    records_file.close();
                    return "finish";
                }
            }
        }

        INIT.healthbar.setSize(sf::Vector2f(INIT.player.health, 20));
        INIT.healthbar.setPosition(sf::Vector2f(600 - INIT.player.health, 0));
        window.draw(INIT.healthbar);
        window.draw(INIT.player.sprite);
    }
    return "game";
}
