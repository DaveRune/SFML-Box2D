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
  

  std::vector<shape> lines;
 // std::vector<b2Body> b2lines;

  float radiuss = 5;
  sf::CircleShape circle1;
  circle1.setRadius(radiuss);
  circle1.setFillColor(sf::Color::Red);

  sf::CircleShape circle2;
  circle2.setRadius(radiuss);
  circle2.setFillColor(sf::Color::Yellow);

  sf::CircleShape circle3;
  circle3.setRadius(radiuss);
  circle3.setFillColor(sf::Color::Green);

  sf::CircleShape circle4;
  circle4.setRadius(radiuss);
  circle4.setFillColor(sf::Color::Blue);



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
              static vector2 start,end;

              if(event.mouseButton.button == sf::Mouse::Left) {
                if(!pressed) {
                  start.x = event.mouseButton.x;
                  start.y = event.mouseButton.y;
                  pressed = true;
                } else {
                  end.x = event.mouseButton.x;
                  end.y = event.mouseButton.y;

                 // Create the SFML visual box
                 shape sf_shape = shape(start,end, 5);
                 lines.push_back(sf_shape);
        
                 // Create the Box2D physics box.
        
                 vector2 center = center.GetCenter(start, end);
        
                // b2FixtureDef myFixture;
                // b2BodyDef myBody;
        
                 //myBody.type = b2_staticBody;
                 //myBody.position.Set(center.x, center.y);
                 //b2Body* body = world.CreateBody(&myBody);
                 //b2Vec2 vertices[4];
     //
     //            center to start + direction
     //
     //
     //

                 vector2 edge = edge.GetDirection(start, center);
                 edge += sf_shape.GetDirection();
                 //vertices[0].Set(edge.x, edge.y);
                 edge += center;
                 circle1.setPosition(edge.x, edge.y);

                 edge = edge.GetDirection(start, center);
                 edge -= sf_shape.GetDirection();
                 edge += center;
                 //vertices[1].Set(edge.x, edge.y);
                 circle2.setPosition(edge.x, edge.y);

                 edge = edge.GetDirection(end, center);
                 edge += sf_shape.GetDirection();
                 edge += center;
                 //vertices[2].Set(edge.x, edge.y);
                 circle3.setPosition(edge.x, edge.y);

                 edge = edge.GetDirection(end, center);
                 edge -= sf_shape.GetDirection();
                 edge += center;
                 //vertices[3].Set(edge.x, edge.y);
                 circle4.setPosition(edge.x, edge.y);
        




                 //b2PolygonShape polyShape;
                 //polyShape.Set(vertices, 4);
                 //
                 //myFixture.shape = &polyShape;
                 //myFixture.density = 1;
                 //body->CreateFixture(&myFixture);
                 
        







                   // // define the color of the triangle's points
                   // quad[0].color = sf::Color::Red;
                   // quad[1].color = sf::Color::Blue;
                   // quad[2].color = sf::Color::Red;
                   // quad[3].color = sf::Color::Blue;

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


      // draw all elements in line vector
      for(int i = 0; i != lines.size(); ++i) {
        static sf::VertexArray quad(sf::Quads, 4);
        quad[0].position = sf::Vector2f(lines[i].a.x, lines[i].a.y);
        quad[1].position = sf::Vector2f(lines[i].b.x, lines[i].b.y);
        quad[2].position = sf::Vector2f(lines[i].c.x, lines[i].c.y);
        quad[3].position = sf::Vector2f(lines[i].d.x, lines[i].d.y);
        window.draw(quad);
      }


      window.draw(circle1);
      window.draw(circle2);
      window.draw(circle3);
      window.draw(circle4);

      window.display();
      
  }

	return 0;
}
