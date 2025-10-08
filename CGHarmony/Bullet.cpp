#include <iostream>
#include <SFML/Graphics.hpp>
#include "Bullet.h"

Bullet::Bullet() {
    id = -1;
    Shifted = false;
    v0 = 0;
    angle = 0;
    bullet_texture.loadFromFile("resources/bullet.png");
    sprt.setTexture(bullet_texture);
    clck;
    pause_buffer;
}

int Bullet::getId() {
    return id;
}

void Bullet::setId(int num) {
    id = num;
}