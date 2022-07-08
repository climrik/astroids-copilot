#ifndef BULLET_H_
#define BULLET_H_
#include <SFML/Network.hpp>

class Bullet{
public:
    Bullet();
	Bullet(float rot, float x, float y);
    float x = 100;
    float y = 100;
    float rot = 0;
    float dx = 0;
    float dy = 0;
    float drot = 7;
    float collision_radius = 20;
    float cushion = 0.98;
    float speed = 15;
    sf::Clock bullet_clock;
    unsigned int start_time = 0;
    unsigned int fly_time = 1900;
    

    bool isDone = false;

    void update();
    void iterate(int stepin);

    float world_width = 600;
    float world_height = 600;
};


sf::Packet& operator <<(sf::Packet& packet, const Bullet& m);
sf::Packet& operator >>(sf::Packet& packet, Bullet& m);

#endif //BULLET_H_
