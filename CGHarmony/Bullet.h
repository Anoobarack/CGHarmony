#include "object.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#ifndef __BULLET_H__
#define __BULLET_H__
class Bullet : public object {
private:
    int id;
    sf::Texture bullet_texture;
public:
    Bullet();
    bool Shifted;
    float v0;
    float angle;
    sf::Clock clck;
    sf::Time pause_buffer;
    sf::Sprite sprt;
    int getId();
    void setId(int num);
};
#endif