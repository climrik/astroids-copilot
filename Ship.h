#ifndef SHIP_H_
#define SHIP_H_
#include <SFML/Network.hpp>
#include "Bullet.h"
#include <vector>

void ShipAhoy();



class Ship{
public:
	Ship() {};
	Ship(int32_t id);
    Ship(int32_t id, std::vector<Bullet> * bullets);
    int32_t id;
    float x = 100;
    float y = 100;
    float rot = 0;
    float dx = 0;
    float dy = 0;
    float drot = 0;
	float ddrot = 5.0;
	float rot_cushion = 0.66;
    float collision_radius = 20;
    float ship_acceleration = 1.0;
    float cushion = 0.98;
    std::vector<Bullet> * bullets_goes_here;

    bool isFire = false;
    bool wasFired = false;
    sf::Clock cooldown;
    int fire_rate = 100;
    bool isThrottle = false;
    bool isLeft = false;
    bool isRight = false;
    bool isDead = false;
    bool is_restart = false;

    void update();
    void iterate(int stepin);

    float world_width = 600;
    float world_height = 600;
};


sf::Packet& operator <<(sf::Packet& packet, const Ship& m);
sf::Packet& operator >>(sf::Packet& packet, Ship& m);

#endif //SHIP_H_
