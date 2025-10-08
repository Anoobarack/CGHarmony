#include "PCharacter.h"
#include <iostream>

PCharacter::PCharacter() {
    running_texture.loadFromFile("resources/running.png");
    hurt_texture.loadFromFile("resources/hurt.png");
    sound_memory.loadFromFile("resources/hitsound.wav");
    current_frame = 0;
    sprite.setTexture(running_texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
    hitsound.setBuffer(sound_memory);
    health = 100;
    speed = 100;
    SPEED_CONST = 100;
    orientation = "right";
    x_pos_offset = 0;
    isRunning = true;
    isHurt = false;
    hurt_clock = 0;
}

void PCharacter::nextSprite() {
    float total = 8;
    int sps = 6;  // sprite per second
    if (current_frame >= total / sps)
        current_frame = 0;
    int frame = floor(current_frame * sps);
    sprite.setTextureRect(sf::IntRect(50 * frame, 0, 50, 50));
}

void PCharacter::turnRight() {
    sprite.setScale(1.0f, 1.0f);
    x_pos_offset = 0;
    orientation = "right";
}

void PCharacter::turnLeft() {
    sprite.setScale(-1.0f, 1.0f);
    x_pos_offset = 50;
    orientation = "left";
}

void PCharacter::toHurt() {
    hurt_clock = 0;
    current_frame = 0;
    hitsound.play();
    sprite.setTexture(hurt_texture);
    isRunning = false;
    isHurt = true;
}

void PCharacter::toRunning() {
    sprite.setTexture(running_texture);
    nextSprite();
    isRunning = true;
    isHurt = false;
}