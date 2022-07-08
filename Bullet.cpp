#include <iostream>
#include "Bullet.h"
#include <bits/stdc++.h>

Bullet::Bullet()
{

}


Bullet::Bullet(float rot, float x, float y) : rot(rot), x(x), y(y)
{

//rot, x + 10.f * std::cos(3.1415*rot/180.f), y +  10.f * std::sin(3.1415*rot/180.f)
dx =  speed * std::cos(3.1415*rot/180.f);
dy =  speed * std::sin(3.1415*rot/180.f);
start_time = bullet_clock.getElapsedTime().asMilliseconds();
}

void Bullet::update()
{

}

void Bullet::iterate(int stepin)
{
	float step = static_cast<float>(stepin)/1000.f;
	//std::cout << stepin << " " << step << std::endl;
	x =  std::fmod(x + dx * speed * step, world_height);
	y =  std::fmod(y + dy * speed * step, world_width);
	if (x < 0) x = world_width -x;
    if (y < 0) y = world_height -y;
    
    
    if (bullet_clock.getElapsedTime().asMilliseconds() - start_time > fly_time)
    {
		isDone = true;
	}
}


sf::Packet& operator <<(sf::Packet& packet, const Bullet& m)
{
    return packet << m.x << m.y;
}
sf::Packet& operator >>(sf::Packet& packet, Bullet& m)
{
    return packet >> m.x >> m.y;
}



