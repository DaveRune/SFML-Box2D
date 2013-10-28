#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdio>

#define SCALE 0.5f
#define DEGTORAD 0.0174532925f
#define RADTODEG 57.2957795f

using namespace std;

// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering engine in your game engine.
int main(int argc, char** argv)
{

	sf::RenderWindow window(sf::VideoMode(768, 768), "SFML works!");

  

	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -5.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);

	// Define the ground body.l
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(384.0f*SCALE, 384.0f*SCALE);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(50.0f*0.5f,10.0f*0.5f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
  bodyDef.angle = -30.0f * DEGTORAD;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(384.0f * SCALE , 700 * SCALE);
	b2Body* body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(10.0f*0.5f, 10.0f*0.5f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
  fixtureDef.restitution = 0.25f;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for (int32 i = 0; i < 60; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		
	}

	// When the world destructor is called, all bodies and joints are freed. This can
	// create orphaned pointers, so be careful about your world management.



  sf::RectangleShape* ground = new sf::RectangleShape;
  ground->setSize(sf::Vector2f(50.0f/SCALE,10.0f/SCALE));
  ground->setOrigin(50.0f*0.5f/SCALE,10.0f*0.5f/SCALE);
  ground->setFillColor(sf::Color::Cyan);
  
  sf::RectangleShape* ball = new sf::RectangleShape;
  ball->setSize(sf::Vector2f(10.0f/SCALE,10.0f/SCALE));
  ball->setOrigin(10.0f*0.5f/SCALE,10.0f*0.5f/SCALE);
  ball->setFillColor(sf::Color::Red);

  while (window.isOpen())
  {
      sf::Event event;
      while (window.pollEvent(event))
      {
          if (event.type == sf::Event::Closed)
              window.close();
      }

      world.Step(timeStep, velocityIterations, positionIterations);

      ground->setPosition(groundBody->GetPosition().x/SCALE, window.getSize().y - (groundBody->GetPosition().y/SCALE));
      ball->setPosition(body->GetPosition().x/SCALE, window.getSize().y - (body->GetPosition().y/SCALE));
      ball->setRotation(body->GetAngle()*RADTODEG);

      window.clear();
      window.draw(*ground);
      window.draw(*ball);
      window.display();
      
  }

	return 0;
}
