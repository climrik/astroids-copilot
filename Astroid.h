#ifndef ASTROID_H_
#define ASTROID_H_
#include <SFML/Network.hpp>

class Astroid {
public:
	Astroid();
	Astroid(float r, float x, float y, float dx, float dy);
	Astroid(float r, float x, float y, float dx, float dy, float speed);
    float x = 100;
    float y = 100;
    float rot = 0;
    float dx = 0;
    float dy = 0;
    float drot = 7;
    float radius = 20;
    float collision_radius = 20;
    float cushion = 1.0;
	float speed = 70.0;
    //std::mt19937 * gen;
	//std::uniform_real_distribution<> *dis;//uniform distribution between 0 and 1

    void update();
    void iterate(int stepin);

    float world_width = 600;
    float world_height = 600;
};


sf::Packet& operator <<(sf::Packet& packet, const Astroid& m);
sf::Packet& operator >>(sf::Packet& packet, Astroid& m);

#endif //ASTROID_H_
