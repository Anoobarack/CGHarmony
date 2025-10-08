#include "object.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#ifndef __PCHARACTER_H__
#define __PCHARACTER_H__

class PCharacter : public object {
    sf::Texture running_texture;
    sf::Texture hurt_texture;
    sf::SoundBuffer sound_memory;
public:
    PCharacter();
    sf::Sprite sprite;
    float current_frame;
    sf::Sound hitsound;
    int SPEED_CONST;
    int health;
    float speed;
    std::string orientation;
    int x_pos_offset;  // turning left will displace character sprite
    bool isRunning;
    bool isHurt;
    float hurt_clock;
    void nextSprite();
    void turnRight();
    void turnLeft();
    void toHurt();
    void toRunning();
};

#endif