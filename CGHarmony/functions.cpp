#include <iostream>
#include <iomanip>
#include <cmath>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "PCharacter.h"

#define PI 3.14159265

float checkBorders(float speed, float x_coord) {
    if (x_coord <= 0 && speed < 0)
        return 0;
    if (x_coord >= 554 && speed > 0)
        return 0;
    return speed;
}

int moveCharacter(sf::Time t_delta, float x_coord, float speed) {
    float x_change = t_delta.asSeconds() * speed;
    if (x_change > 0) {
        if (x_change < 1)
            x_change = 1;
        else
            x_change += 1;
    }
    float new_x = x_coord + x_change;
    return new_x;
}

bool checkCollision(Bullet& bullet, PCharacter player) {
    int x_diff = bullet.x_coord - player.x_coord;
    int y_diff = bullet.y_coord - player.y_coord;
    if (player.orientation == "right") {
        if (x_diff >= 4 && x_diff <= 31) {
            if ((y_diff >= -5) && (y_diff < 47)) {
                // hit
                bullet.clck.restart();
                bullet.pause_buffer = sf::seconds(0);
                bullet.Shifted = false;
                return true;
            }
        }
    }
    if (player.orientation == "left") {
        if (x_diff >= 11 && x_diff <= 38) {
            if ((y_diff >= -5) && (y_diff <= 47)) {
                // hit
                bullet.clck.restart();
                bullet.pause_buffer = sf::seconds(0);
                bullet.Shifted = false;
                return true;
            }
        }
    }
    return false;
}

struct base_values {
    float v0, alpha_angle;
    int old_x, old_y;
};

base_values getBaseVals(int id) {
    float v0 = 0, alpha_angle = 0;
    int old_x = 0, old_y = 0;
    switch (id) {
    case 0: {
        v0 = 100;
        alpha_angle = 15;
        old_x = 0;
        old_y = 50;
        break;
    }
    case 1: {
        v0 = 150;
        alpha_angle = 30;
        old_x = 0;
        old_y = 100;
        break;
    }
    case 2: {
        v0 = 200;
        alpha_angle = 45;
        old_x = 0;
        old_y = 150;
        break;
    }
    case 3: {
        v0 = 235;
        alpha_angle = 60;
        old_x = 0;
        old_y = 200;
        break;
    }
    case 4: {
        v0 = -100;
        alpha_angle = -15;
        old_x = 600;
        old_y = 50;
        break;
    }
    case 5: {
        v0 = -150;
        alpha_angle = -30;
        old_x = 600;
        old_y = 100;
        break;
    }
    case 6: {
        v0 = -200;
        alpha_angle = -45;
        old_x = 600;
        old_y = 150;
        break;
    }
    case 7: {
        v0 = -235;
        alpha_angle = -60;
        old_x = 600;
        old_y = 200;
        break;
    }
    }
    base_values ret = { v0, alpha_angle, old_x, old_y };
    return ret;
}

std::vector<int> getShift(int64_t time_ms) {
    std::srand(time_ms);
    int v_shift = std::rand() % 80 - 40;
    int angle_shift = std::rand() % 10 - 5;
    std::vector<int> ret;
    ret.push_back(v_shift);
    ret.push_back(angle_shift);
    return ret;
}

std::vector<int> updateCoordinates_parabolic(Bullet& bullet, sf::Clock& clock) {
    int id = bullet.getId();
    base_values bval = getBaseVals(id);
    int old_x = bval.old_x;
    int old_y = bval.old_y;
    float g = 160;
    float v0 = 0, alpha_angle = 0;
    float time = bullet.pause_buffer.asSeconds();
    if (!bullet.Shifted)
    {
        v0 = bval.v0;
        alpha_angle = bval.alpha_angle;
        std::vector<int> rand_shift = getShift(clock.getElapsedTime().asMicroseconds());
        v0 += rand_shift[0];
        alpha_angle += rand_shift[1];
        bullet.Shifted = true;
        bullet.v0 = v0;
        bullet.angle = alpha_angle;
    }
    if (bullet.Shifted) {
        v0 = bullet.v0;
        alpha_angle = bullet.angle;
    }
    int new_x = 600 - (old_x + v0 * cos(alpha_angle * PI / 180) * time);
    int new_y = 400 - (old_y + v0 * sin(alpha_angle * PI / 180) * time - (g * time * time / 2));
    if (new_y > 400 || new_x < 0 || new_x > 600) {
        bullet.clck.restart();
        bullet.pause_buffer = sf::seconds(0);
        bullet.Shifted = false;
    }
    std::vector<int> ret;
    ret.push_back(new_x);
    ret.push_back(new_y);
    return ret;
}

std::string fixedFloat(float x)
{
    std::string s(16, '\0');
    auto written = std::snprintf(&s[0], s.size(), "%.2f", x);
    s.resize(written);
    return s;
}