//Libraries
#include <SFML/Graphics.hpp>
#include <iostream>
//convert number into text
#include <sstream>
#include <SFML/Audio.hpp>

//Glboal variables, functions, classes

//C++ program entry point
int main()
{
	//Creating the window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

	//Settign the framerate limit to 60 FPS
	window.setFramerateLimit(60);

	window.setKeyRepeatEnabled(false);

	//Variable that keeps the game loop running
	bool play = true;

	//Event object holding all events
	sf::Event event;

	//Font
	sf::Font font;	//Font object
	if (font.loadFromFile("Data/joystix.ttf") == 0) //Safe way to load font
	{
		return 1;
	}

	sf::Text score;
	score.setFont(font);
	score.setCharacterSize(40);
	score.setColor(sf::Color::Red);
	score.setPosition(315,10);
	score.setString("0 : 0");

	//Images
	sf::Texture tex_pad; //Pad image/texutre
	sf::Texture tex_ball; //Ball texture
	sf::Texture tex_background; //Background texture

	//Load the 3 textures
	if (tex_pad.loadFromFile("Data/paddle.png") == false)
	{
		return -1;
	}

	if (tex_ball.loadFromFile("Data/ball.png") == false)
	{
		return -1;
	}

	if (tex_background.loadFromFile("Data/background.png") == false)
	{
		return -1;
	}

	//Sounds
	sf::SoundBuffer buffHit; //Hit sound
	sf::Sound hit;
	//Load the sound
	if (buffHit.loadFromFile("Data/hit.wav") == false)
	{
		return -1;
	}
	hit.setBuffer(buffHit);

	//States
	bool up = false;
	bool down = false;


	//Variables
	int yVelocityPad1 = 0;
	int yVelocityPad2 = 0;
	int xVelocityBall = -4;
	int yVelocityBall = -4;
	int pad1Score = 0;
	int pad2Score = 0;
	
	//////////Shapes
	//Background
	sf::RectangleShape background;
	background.setSize(sf::Vector2f(800, 600));
	background.setPosition(0, 0);
	background.setTexture(&tex_background);
	
	//Pad1
	sf::RectangleShape pad1;
	pad1.setSize(sf::Vector2f(20, 100));
	pad1.setPosition(10, 480);
	pad1.setTexture(&tex_pad);

	//Pad2
	sf::RectangleShape pad2;
	pad2.setSize(sf::Vector2f(20, 100));
	pad2.setPosition(770, 20);
	pad2.setTexture(&tex_pad);

	//Ball
	sf::RectangleShape ball;
	ball.setSize(sf::Vector2f(50, 50));
	ball.setPosition(400, 200);
	ball.setTexture(&tex_ball);

	//Game loop
	while (play == true)
	{
		//EVENTS
		while (window.pollEvent(event))
		{
			//Event type is window closed
			if (event.type == sf::Event::Closed)
			{
				//Set play to false in order to stop the game loop
				play = false;
			}

			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
			{
				up = true;
			}
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
			{
				down = true;
			}
			if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up)
			{
				up = false;
			}
			if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down)
			{
				down = false;
			}
		}

		//LOGIC
		//PAD1
		if(up == true)
		{
			yVelocityPad1 = -5;
		}
		if(down == true)
		{
			yVelocityPad1 = 5;
		}
		if(up == true && down == true)
		{
			yVelocityPad1 = 0;
		}
		if(up == false && down == false)
		{
			yVelocityPad1 = 0;
		}
		pad1.move(0,yVelocityPad1);

		//out of bound check for pad 1
		if(pad1.getPosition().y < 0)
		{
			pad1.setPosition(10, 0);
		}
		if(pad1.getPosition().y > 500)
		{
			pad1.setPosition(10, 500);
		}

		//////PAD 2 AI LOGIC
		if(ball.getPosition().y < pad2.getPosition().y)
		{
			yVelocityPad2 = -3;
		}
		if(ball.getPosition().y > pad2.getPosition().y)
		{
			yVelocityPad2 = 3;
		}
		pad2.move(0,yVelocityPad2);
		//out of bound check for pad 2
		if(pad2.getPosition().y < 0)
		{
			pad2.setPosition(770, 0);
		}
		if(pad2.getPosition().y > 500)
		{
			pad2.setPosition(770, 500);
		}


		//BALL
		ball.move(xVelocityBall, yVelocityBall);
		//out of bound check for ball
		if(ball.getPosition().y < 0)
		{
			yVelocityBall = (yVelocityBall * -1);
		}
		if(ball.getPosition().y > 550)
		{
			yVelocityBall = (yVelocityBall * -1);
		}
		//ball collisions 
		if(ball.getGlobalBounds().intersects(pad1.getGlobalBounds()) == true)
		{
			xVelocityBall = (xVelocityBall * -1);
			hit.play();
		}
		if(ball.getGlobalBounds().intersects(pad2.getGlobalBounds()) == true)
		{
			xVelocityBall = (xVelocityBall * -1);
			hit.play();
		}
		//score keeping + reset ball
		if(ball.getPosition().x < -50)
		{
			pad2Score ++;
			ball.setPosition(350,300);
		}
		if(ball.getPosition().x > 800)
		{
			pad1Score ++;
			ball.setPosition(350,300);
		}
		//RENDERING
		window.clear();

		//Drawing the shapes
		window.draw(background);
		window.draw(pad1);
		window.draw(pad2);
		window.draw(ball);

		//score
		std::stringstream text;
		text << pad1Score << " : " << pad2Score;
		score.setString(text.str());
		window.draw(score);

		window.display();
	}
	///////////

	//Clean up and close the window
	window.close();

	//Close the program
	return 0;
}