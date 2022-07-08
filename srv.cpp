#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <math.h>
#include <vector>
#include <list>
#include <bits/stdc++.h>
#include <random>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "Ship.h"
#include "Bullet.h"
#include "Astroid.h"

//game state
sf::Clock clocky;
float variable = -1.f;
float rate = 0.1f;
bool isJoystick = false;
int sleepy = 60;
int step_length = 0;
std::vector<Ship> ships;
std::vector<Astroid> astroids;
std::vector<Astroid*> incoming_astroids;
std::vector<Bullet> bullets;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);//uniform distribution between 0 and 1
float world_height = 600;
float world_width = 600;

Astroid safe_spawn_astroid()
{
	// radius
	float radius = 29;
	float x_suggestion;
	float y_suggestion;
	float x_distance;
	float y_distance;
	float okay_distance = 150;

	//suggest a position
	bool okay_suggestion = false;
	while (!okay_suggestion)
	{
		x_suggestion = world_width * dis(gen);
		y_suggestion = world_height * dis(gen);
		
		//check if suggestion is too close to any ship
		bool too_close = false;
		for (int i = 0; i < ships.size(); ++i)
		{
			x_distance = ships[i].x - x_suggestion;
			y_distance = ships[i].y - y_suggestion;
			if(std::pow(x_distance, 2) + std::pow(y_distance, 2) < std::pow(okay_distance, 2))
			{
				too_close = true;
				break;
			}
		}
		if (too_close)
			continue;
		else
			okay_suggestion = true;
	}
	//Astroid(float r, float x, float y, float dx, float dy, float speed);
	Astroid astroid(radius, x_suggestion, y_suggestion,
					( 1 - 2*dis(gen)), ( 1 - 2*dis(gen)));
	return astroid;
	//check if 
	//Astroid astroid;
}

//network
sf::IpAddress server_ip{"10.10.13.33"};
//sf::IpAddress server_ip{"127.0.0.1"};
sf::IpAddress sender_ip;
std::vector<std::tuple<sf::IpAddress, unsigned short>> clients;
unsigned short server_port = 55100;
unsigned short sender_port;
sf::Packet packet;
sf::UdpSocket socket;
sf::SocketSelector socket_selector;


void detach_func(sf::UdpSocket* socket);
void thread_waiting(int n, sf::UdpSocket* socket) {
	sf::Packet packet;
	sf::IpAddress client_ip;
	unsigned short client_port;
	int32_t id;
	bool isLeft;
	bool isRight;
	bool isThrottle;
	bool isFire;
	socket->receive(packet, client_ip, client_port);
	packet >> id >> isLeft >> isRight >> isThrottle >> isFire;
	//std::cout << id << "	" << isLeft << isRight << isThrottle << isFire << std::endl;

	//check if ship_id exists else make new ship
	bool add_new_ship = true;
	for (int i = 0; i < ships.size(); ++i)
	{
		if (ships[i].id == id)
		{
			add_new_ship = false;
			ships[i].isLeft = isLeft;
			ships[i].isRight = isRight;
			ships[i].isThrottle = isThrottle;
			ships[i].isFire = isFire;
		}
	}
	if (add_new_ship)
	{
		ships.push_back(Ship(id, &bullets));
	}

	//search if client_port combination exists
	sender_ip = client_ip;
	sender_port = client_port;
	bool add_new_client = true;
	for (int i = 0; i < clients.size(); ++i)
	{
		if(std::get<0>(clients[i]) == client_ip &&
		std::get<1>(clients[i]) == client_port)
		{
			add_new_client = false;
			break;
		}
	}
	if (add_new_client)
	{
		clients.push_back(
			std::tuple<sf::IpAddress, unsigned short>(client_ip, client_port) );
	}
	detach_func(socket);
}

void detach_func(sf::UdpSocket* socket)
{
    std::thread (thread_waiting,4, socket).detach();
}

void thread_call(sf::UdpSocket* socket)
{
    detach_func(socket);
}

int main ()
{
	std::cout << "Hello server" << std::endl;
	socket.bind(server_port, server_ip);
	socket.setBlocking(true);
	socket_selector.add(socket);
	thread_call(&socket);

    int level = 0;
    int spawn_amount = 3;
	astroids.push_back(Astroid(29, 300, 300, -1, 1));

	while (true)
	{
		
		//std::cout << "Result: " << isJoystick << std::endl;
		//std::cout << "Stuff: " << clients.size() << " " << ships.size() << " " << astroids.size() << " " << bullets.size() << std::endl;

		//do stuff
		if (isJoystick)
		{
			variable += rate;
		}
		else
		{
			variable -= rate;
		}
		
		int timey = clocky.getElapsedTime().asMilliseconds();
		for (int i = 0; i < ships.size(); ++i)
		{
			ships[i].iterate(timey);
		}
		for (int i = 0; i < astroids.size(); ++i)
		{
			astroids[i].iterate(timey);
		}
		for (int i = 0; i < bullets.size(); ++i)
		{
			bullets[i].iterate(timey);
		}

		//check collision
		//
		// ... todo ...
		//
		// bullets vs astroids (also check fly_time)
		float xdist;
		float ydist;
		float collision_distance;
		for (std::vector<Bullet>::iterator b = bullets.begin(); b != bullets.end(); b++) {
            for (std::vector<Astroid>::iterator a = astroids.begin(); a != astroids.end(); ) {
                xdist = a->x - b->x;
                ydist = a->y - b->y;
                collision_distance = 3.f + a->radius;
                //std::cout << xdist * xdist + ydist * ydist << " xy:r " <<  collision_distance * collision_distance << std::endl;
                if(xdist * xdist + ydist * ydist < collision_distance * collision_distance) {
                    //when an astroid gets hit we device what happens here.
                    //preferbly it breaks into 2 or more tiny:er astroids.
                    //astroids will come in sizes: 30.f, 20.f, and 10.f
                    //lager sizes will break into smaller sizes
                    float x = a->x;
                    float y = a->y;

                    if (a->radius > 11.f) {
                        incoming_astroids.push_back(
							new Astroid(a->radius - 10.f, a->x, a->y,
							( 1 - 2*dis(gen)),
							( 1 - 2*dis(gen)),
							a->speed * 1.2f));
                        incoming_astroids.push_back(
							new Astroid(a->radius - 10.f, a->x, a->y,
							( 1 - 2*dis(gen)),
							( 1 - 2*dis(gen)),
							a->speed + 2.f));
                        incoming_astroids.push_back(
							new Astroid(a->radius - 10.f, a->x, a->y,
							( 1 - 2*dis(gen)),
							( 1 - 2*dis(gen)),
							a->speed * 1.2f));
                        a = astroids.erase(a);
                    } else {
                        a = astroids.erase(a);
                    }
                    b->isDone = true; 
                } else {
                    ++a;
                }                
            }
        }
        //astroid vs ship
        for (std::vector<Astroid>::iterator a = astroids.begin(); a != astroids.end(); ++a)
        {
			for (std::vector<Ship>::iterator s = ships.begin(); s != ships.end(); ++s)
			{
				xdist = a->x - s->x;
				ydist = a->y - s->y;
				collision_distance = std::pow(s->collision_radius,2) + std::pow(a->radius,2);
				if(xdist * xdist + ydist * ydist < collision_distance)
				{
					s->isDead = true;
				}
			}
        }
		
		
		//cleanup out of range bullets
        for (std::vector<Bullet>::iterator it=bullets.begin(); it != bullets.end(); /*it++*/) {
            if(it->isDone)
                it = bullets.erase(it);
            else
                ++it;
        }
        
        //imcoming astroids come inside
        for (std::vector<Astroid*>::iterator a = incoming_astroids.begin(); a != incoming_astroids.end(); ) {
            astroids.push_back(**a);
            *a = nullptr;
            a = incoming_astroids.erase(a);
        }
        
        //next level
        if (astroids.size() < 1) {
            spawn_amount *= 1.2;
            spawn_amount += 1;
            level++;
            std::cout << "Current level: " << level << std::endl;
            for (int i = 0; i < spawn_amount; i++)
            {
                astroids.push_back(safe_spawn_astroid());
            }
            for (int i = 0; i < ships.size(); ++i)
            {
				ships[i].isDead = false;
			}
        } 
		
		
		//pack stuff for client
		packet = sf::Packet();
		packet << variable;
		//pack ships
		packet << static_cast<unsigned short>(ships.size());
		for (int i = 0; i < ships.size(); ++i)
		{
			packet << ships[i];
		}
		packet << static_cast<unsigned short>(astroids.size());
		for (int i = 0; i < astroids.size(); ++i)
		{
			packet << astroids[i];
		}
		packet << static_cast<unsigned short>(bullets.size());
		for (int i = 0; i < bullets.size(); ++i)
		{
			packet << bullets[i];
		}
		
		//pack astroids
		
		//pack bullets
		
		//send all the clients
		for (int i = 0; i < clients.size(); ++i)
		{
			socket.send(packet, std::get<0>(clients[i]),
			std::get<1>(clients[i]) );
		}
		
		clocky.restart();
		std::this_thread::sleep_for (std::chrono::milliseconds(sleepy));

	}
	
	
	
    return 0;
}
