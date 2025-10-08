#include <iostream>
//hack for WinMain not to throw errors
#define NOMINMAX
#include <Windows.h>

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "INIT.h"
#include "Game.h"

using namespace std;
using namespace sf;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
    LPSTR lpszArgument, int nCmdShow)
{
    const unsigned WINDOW_SIZE_X = 600;
    const unsigned WINDOW_SIZE_Y = 400;
    const unsigned FRAME_LIMIT = 60;
    RenderWindow window;
    window.create(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Spark in Midnight");
    window.setFramerateLimit(FRAME_LIMIT);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    Music theme;
    theme.openFromFile("resources/main_theme.wav");
    float music_volume = 80;
    theme.setVolume(music_volume);
    theme.setLoop(true);
    theme.play();
    string record = "";
    game_parameters INIT(window, record);
    string gamestate = "start";
    bool paused = false;
    while (INIT.running)
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                INIT.running = false;
                break;

            default:
                break;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            INIT.running = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
        {
            if (music_volume < 100)
            {
                music_volume += 10;
                theme.setVolume(music_volume);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
        {
            if (music_volume > 0)
            {
                music_volume -= 10;
                theme.setVolume(music_volume);
            }
        }

        window.clear();

        //processing background
        window.draw(INIT.bg);

        // start button and previous record
        if (gamestate == "start") {
            window.draw(INIT.start_button);
            window.draw(INIT.record_holder);
            window.draw(INIT.start_text);
            window.draw(INIT.record_text);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                // check start button position
                Vector2i mousePosition = Mouse::getPosition(window);
                Vector2u windowSize = window.getSize();
                if (mousePosition.x > windowSize.x / 2 * 0.75
                    && mousePosition.x < windowSize.x / 2 * 1.25) {
                    if (mousePosition.y > windowSize.y / 2 * 0.925
                        && mousePosition.y < windowSize.y / 2 * 1.075) {
                        INIT.clock.restart();
                        for (int i = 0; i < INIT.BULLETS_AMMOUNT; i++) {
                            INIT.bullets[i].clck.restart();
                        }
                        gamestate = "game";
                    }
                }
            }
        }

        if (gamestate == "game") {
            gamestate = game(INIT, window, record, paused);
        }

        // new record and restart
        if (gamestate == "finish") {
            INIT.start_text.setString("Restart");
            INIT.start_text.setPosition(257, 180);
            window.draw(INIT.start_button);
            window.draw(INIT.record_holder);
            window.draw(INIT.start_text);
            window.draw(INIT.record_text);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                // check start button position
                Vector2i mousePosition = Mouse::getPosition(window);
                Vector2u windowSize = window.getSize();
                if (mousePosition.x > windowSize.x / 2 * 0.75
                    && mousePosition.x < windowSize.x / 2 * 1.25) {
                    if (mousePosition.y > windowSize.y / 2 * 0.925
                        && mousePosition.y < windowSize.y / 2 * 1.075) {
                        new (&INIT) game_parameters(window, record);  // reinitialize parameters
                        gamestate = "game";
                    }
                }
            }
        }

        window.display();
    }

    return EXIT_SUCCESS;
}

int main()
{
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}