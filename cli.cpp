#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <math.h>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Ship.h"
#include "Bullet.h"
#include "Astroid.h"

//draw functions
void draw_ship(sf::RenderWindow * window, Ship &ship)
{
	sf::ConvexShape shape;
	shape.setPointCount(3);
	shape.setPoint(0, sf::Vector2f(40, 0));
	shape.setPoint(1, sf::Vector2f(0, -16));
	shape.setPoint(2, sf::Vector2f(0, 16));
	shape.setOrigin(sf::Vector2f(10,0));
	if (ship.isDead) shape.setFillColor(sf::Color::Red);
	else shape.setFillColor(sf::Color::Green);
	shape.setPosition(ship.x, ship.y);
	shape.rotate(ship.rot);
    window->draw(shape);
}

void draw_astroid(sf::RenderWindow * window, Astroid &astroid)
{
	sf::CircleShape shape;
	shape.setRadius(astroid.radius);
	shape.setOrigin(sf::Vector2f(astroid.radius,astroid.radius));
	shape.setOutlineColor(sf::Color::Red);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(5);
	shape.setPosition(astroid.x, astroid.y);
    window->draw(shape);
}

void draw_bullet(sf::RenderWindow * window, Bullet &bullet)
{
	sf::CircleShape shape;
	shape.setRadius(3.f);
	shape.setOrigin(sf::Vector2f(3.f,3.f));
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(bullet.x, bullet.y);
    window->draw(shape);
}


//game state
float variable = 0.f;
bool isJoystick = true;
int32_t id = 0;
int sleepy = 15;
unsigned short ships_size;
unsigned short astroids_size;
unsigned short bullets_size;
std::vector<Ship> ships;
std::vector<Astroid> astroids;
std::vector<Bullet> bullets;
bool isLeft = false;
bool isRight = false;
bool isThrottle = false;
bool isFire = false;

float world_width = 600;
float world_height = 600;

//network
sf::IpAddress server_ip{"213.164.207.80"};
//sf::IpAddress server_ip{"127.0.0.1"};
unsigned short port = 55100; 
sf::Packet packet;
sf::UdpSocket socket;

int main ()
{
	std::cout << "Enter ship id: ";
	std::cin >> id;
	//send inputs to server
	std::cout << "Hello client " << id << std::endl;
	
	// sfml utility
	unsigned int loop_iteration_duration;
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(world_width, world_height),"Astroids multiplayer");
    sf::Time elapsed1 = clock.getElapsedTime();
    sf::Time elapsed2 = clock.getElapsedTime();

	while (window.isOpen())
	{
		//get inputs from keyboard
		isThrottle = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        isRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        isLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        isFire = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
        
		//send inputs to server
		packet = sf::Packet();
		packet << id << isLeft << isRight << isThrottle << isFire;
		socket.send(packet, server_ip, port);
		
		//handle response from server
		packet = sf::Packet();
		socket.receive(packet, server_ip, port);
		packet >> variable;
		//upack ships
		ships = std::vector<Ship>(); //clear array
		packet >> ships_size;
		for (int i = 0; i < ships_size; ++i)
		{
			Ship ship;
			packet >> ship;
			ships.push_back(ship);
		}
		
		//unpack astroids
		astroids = std::vector<Astroid>(); //clear array
		packet >> astroids_size;
		for (int i = 0; i < astroids_size; ++i)
		{
			Astroid astroid;
			packet >> astroid;
			astroids.push_back(astroid);
		}
		
		//unpack bullets
		bullets = std::vector<Bullet>(); //clear array
		packet >> bullets_size;
		for (int i = 0; i < bullets_size; ++i)
		{
			Bullet bullet;
			packet >> bullet;
			bullets.push_back(bullet);
		}
		
		//draw stuff
		window.clear();
		//std::cout << "Result: "
		//		  << ships_size << " " << astroids_size << " " << bullets_size
		//		  << " " << variable << std::endl;
		for (int i = 0; i < ships.size(); ++i)
		{
			draw_ship(&window, ships[i]);
		}
		for (int i = 0; i < astroids.size(); ++i)
		{
			draw_astroid(&window, astroids[i]);
		}
		for (int i = 0; i < bullets.size(); ++i)
		{
			draw_bullet(&window, bullets[i]);
		}
		
		window.display();
		
		//pause and thing about what you've done
		std::this_thread::sleep_for (std::chrono::milliseconds(sleepy));
		
		sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }		
	}
    return 0;
}
