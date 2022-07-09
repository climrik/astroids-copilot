#include <iostream>
#include "Ship.h"
#include "Bullet.h"
#include <bits/stdc++.h>
void ShipAhoy ()
{
    std::cout << "Ship Ahoy!" << std::endl;
}

Ship::Ship(int32_t id) : id(id)
{

}

Ship::Ship(int32_t id, std::vector<Bullet> * bullets) : id(id), bullets_goes_here(bullets)
{

}

void Ship::update()
{

}

void Ship::iterate(int stepin)
{
	if (!isDead)
	{
		float step = static_cast<float>(stepin)/1000.f;
		//std::cout << "Yupp" << std::endl;
		if(isLeft) drot -= ddrot;
		if(isRight) drot += ddrot;
	
		rot += drot;
		
		
		if (isThrottle) {
			dx += ship_acceleration * std::cos(3.1415*rot/180.f); 
			dy += ship_acceleration * std::sin(3.1415*rot/180.f);
		}
		x = std::fmod(dx + x,world_width);
		y = std::fmod(dy + y,world_height);
		if (x < 0) x = world_width -x;
		if (y < 0) y = world_height -y;

		if (isFire && !wasFired)
		{
			bullets_goes_here->push_back(Bullet(rot, x + 20.f * std::cos(3.1415*rot/180.f), y +  20.f * std::sin(3.1415*rot/180.f)));
			wasFired = true;
			cooldown.restart();
		}
		if(cooldown.getElapsedTime().asMilliseconds() > fire_rate)
		{
			wasFired = false;
		}

		//cushion
		dx *= cushion;
		dy *= cushion;
		drot *= rot_cushion;
	}
}


sf::Packet& operator <<(sf::Packet& packet, const Ship& m)
{
    return packet << m.x << m.y << m.rot << m.id << m.isDead;
}
sf::Packet& operator >>(sf::Packet& packet, Ship& m)
{
    return packet >> m.x >> m.y >> m.rot >> m.id >> m.isDead;
}



