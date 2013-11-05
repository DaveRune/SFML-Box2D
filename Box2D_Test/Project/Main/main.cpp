#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Project/Shape/Shape.h>
#include <cstdio>

#define SCALE 0.5f
#define DEGTORAD 0.0174532925f
#define RADTODEG 57.2957795f

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
	float32 timeStep = 1.0f / 120.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for (int32 i = 0; i < 60; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		
	}

  // Create the shapes here
  sf::RectangleShape* ground = new sf::RectangleShape;
  ground->setSize(sf::Vector2f(50.0f/SCALE,10.0f/SCALE));
  ground->setOrigin(50.0f*0.5f/SCALE,10.0f*0.5f/SCALE);
  ground->setFillColor(sf::Color::Cyan);
  
  sf::RectangleShape* ball = new sf::RectangleShape;
  ball->setSize(sf::Vector2f(10.0f/SCALE,10.0f/SCALE));
  ball->setOrigin(10.0f*0.5f/SCALE,10.0f*0.5f/SCALE);
  ball->setFillColor(sf::Color::Red);

  // create an array of 3 vertices that define a triangle primitive
  

  // // define the position of the triangle's points
  // quad[0].position = sf::Vector2f(10, 10);
  // quad[1].position = sf::Vector2f(100, 10);
  // quad[2].position = sf::Vector2f(100, 100);
  // quad[3].position = sf::Vector2f(10, 100);
  // 
  // // define the color of the triangle's points
  // quad[0].color = sf::Color::Red;
  // quad[1].color = sf::Color::Blue;
  // quad[2].color = sf::Color::Red;
  // quad[3].color = sf::Color::Blue;

  std::vector<shape> lines;

  while (window.isOpen())
  {
      sf::Event event;
      while (window.pollEvent(event))
      {
          switch (event.type)
          {
          case sf::Event::Closed:
             window.close();
             break;
          
          case sf::Event::MouseButtonPressed:
              static bool pressed = false;
              static int sX,sY,eX,eY;

              if(event.mouseButton.button == sf::Mouse::Left) {
                if(!pressed) {
                  sX = event.mouseButton.x;
                  sY = event.mouseButton.y;
                  pressed = true;
                } else {
                  eX = event.mouseButton.x;
                  eY = event.mouseButton.y;

                  lines.push_back(shape(sX,sY,eX,eY));
                  //shape::ConstructLine(sX,sY,eX,eY);

                  pressed = false;
                }
              }
          }
             
      }

      world.Step(timeStep, velocityIterations, positionIterations);

      ground->setPosition(groundBody->GetPosition().x/SCALE, window.getSize().y - (groundBody->GetPosition().y/SCALE));
      ball->setPosition(body->GetPosition().x/SCALE, window.getSize().y - (body->GetPosition().y/SCALE));
      ball->setRotation(body->GetAngle()*RADTODEG);

      window.clear();
      window.draw(*ground);
      window.draw(*ball);
      //window.draw(quad);

      // draw all elements in line vector
      for(int i = 0; i != lines.size(); ++i) {
        static sf::VertexArray quad(sf::Quads, 4);
        quad[0].position = sf::Vector2f(lines[i].aX, lines[i].aY);
        quad[1].position = sf::Vector2f(lines[i].cX, lines[i].cY);
        quad[2].position = sf::Vector2f(lines[i].dX, lines[i].dY);
        quad[3].position = sf::Vector2f(lines[i].bX, lines[i].bY);
        window.draw(quad);
      }

      window.display();
      
  }

	return 0;
}
