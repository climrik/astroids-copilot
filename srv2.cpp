#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <math.h>
#include <vector>
#include <list>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "Ship.h"
#include "Bullet.h"

//game state
sf::Clock clocky;
float variable = -1.f;
float rate = 0.1f;
bool isJoystick = false;
int sleepy = 120;
int step_length = 0;
std::vector<Ship> ships;


//network
//sf::IpAddress server_ip{"10.10.13.33"};
sf::IpAddress server_ip{"127.0.0.1"};
sf::IpAddress sender_ip;
std::vector<std::tuple<sf::IpAddress, unsigned short>> clients;
unsigned short server_port = 55100;
unsigned short sender_port;
sf::Packet packet;
sf::UdpSocket socket;
sf::SocketSelector socket_selector;


void thread_waiting(int n) {

}

void detach_func()
{
    std::thread (thread_waiting,4).detach();
}

void thread_call()
{
    detach_func();
}

int main ()
{
	std::cout << "Hello server" << std::endl;
	socket.bind(server_port, server_ip);
	socket.setBlocking(false);
	socket_selector.add(socket);

	while (true)
	{
		packet = sf::Packet();
		if(socket_selector.wait())
		{
			if(socket_selector.isReady(socket))
			{
				sf::IpAddress client_ip;
				unsigned short client_port;
				int32_t id;
				bool isLeft;
				bool isRight;
				bool isThrottle;
				bool isFire;
				socket.receive(packet, client_ip, client_port);
				packet >> id >> isLeft >> isRight >> isThrottle >> isFire;
				std::cout << id << "	" << isLeft << isRight << isThrottle << isFire << std::endl;

				
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
					ships.push_back(Ship(id));
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
			}	
		}
		
		
		//std::cout << "Result: " << isJoystick << std::endl;
		std::cout << "Clients: " << clients.size() << " Ships" << ships.size() << std::endl;

		//do stuff
		if (isJoystick)
		{
			variable += rate;
		}
		else
		{
			variable -= rate;
		}
		
		for (int i = 0; i < ships.size(); ++i)
		{
			ships[i].iterate(clocky.getElapsedTime().asMilliseconds());
		}

		//check collision
		//
		// ... todo ...
		//
		
		//pack stuff for client
		packet = sf::Packet();
		packet << variable;
		//pack ships
		packet << static_cast<unsigned short>(ships.size());
		for (int i = 0; i < ships.size(); ++i)
		{
			packet << ships[i];
		}
		
		//pack astroids
		
		//pack bullets
		
		//send all the clients
		for (int i = 0; i < clients.size(); ++i)
		{
			socket.send(packet, std::get<0>(clients[i]),
			std::get<1>(clients[i]) );
		}
		std::this_thread::sleep_for (std::chrono::milliseconds(sleepy));
		clocky.restart();
	}
	
	
	
    return 0;
}
