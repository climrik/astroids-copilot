#include <iostream>
#include "Astroid.h"
#include <bits/stdc++.h>

Astroid::Astroid()
{
	
}

Astroid::Astroid(float r, float x, float y, float dx, float dy) 
	: x(x), y(y), radius(r), dx(dx), dy(dy)
{
	
}

Astroid::Astroid(float r, float x, float y, float dx, float dy, float speed) 
	: x(x), y(y), radius(r), dx(dx), dy(dy), speed(speed)
{
	
}

void Astroid::update()
{

}

void Astroid::iterate(int stepin)
{
	
	float step = static_cast<float>(stepin)/1000.f;
	//std::cout << stepin << " " << step << std::endl;
	x =  std::fmod(x + dx * speed * step, world_height);
	y =  std::fmod(y + dy * speed * step, world_width);
	if (x < 0) x = world_width -x;
    if (y < 0) y = world_height -y;

}


sf::Packet& operator <<(sf::Packet& packet, const Astroid& m)
{
    return packet << m.x << m.y << m.radius;
}
sf::Packet& operator >>(sf::Packet& packet, Astroid& m)
{
    return packet >> m.x >> m.y >> m.radius;
}



