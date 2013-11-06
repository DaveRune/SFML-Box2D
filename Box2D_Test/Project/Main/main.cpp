#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Project/Shape/Shape.h>
#include <cstdio>

#define SCALE 0.5f
#define DEGTORAD 0.0174532925f
#define RADTODEG 57.2957795f

bool paused = true;
vector2 cubeStart(100,100);

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");

	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, 5.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(cubeStart.x * SCALE ,cubeStart.y * SCALE);
	b2Body* body = world.CreateBody(&bodyDef);
  bodyDef.awake = false;
	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(10.0f*0.5f, 10.0f*0.5f);
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
  fixtureDef.restitution = 1.25f;
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

  std::vector<shape> lines;
  std::vector<b2Body> *b2lines;

  bool doConstantLines = false;

  while (window.isOpen()){
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

            // When we click save the positions to a Vector2
            if(event.mouseButton.button == sf::Mouse::Left) {
              // If we are drawing lines constantly, dont perform click drawing
              if(doConstantLines) break;
              if(!pressed) {
                start.x = event.mouseButton.x;
                start.y = event.mouseButton.y;
                pressed = true;
              } else {
                end.x = event.mouseButton.x;
                end.y = event.mouseButton.y;

                // Create the SFML visual box based on click locations
                shape sf_shape = shape(start,end, 2);
                lines.push_back(sf_shape);
        
                vector2 center = center.GetCenter(start, end);

                // Create the Box2D physics box.
                static b2FixtureDef myFixture;
                static b2BodyDef myBody;
                myBody.type = b2_staticBody;
                myBody.position.Set(center.x, center.y);
                b2Body* body = world.CreateBody(&myBody);
                b2Vec2 vertices[4];

                // Caluclate the positions of the Box2D Verts
                sf_shape.Box2DVertPos();
                vertices[0].Set(sf_shape.a.x * SCALE -start.x*0.5f, sf_shape.a.y * SCALE -start.y*0.5f);
                vertices[3].Set(sf_shape.b.x * SCALE -start.x*0.5f, sf_shape.b.y * SCALE -start.y*0.5f);
                vertices[2].Set(sf_shape.c.x * SCALE -start.x*0.5f, sf_shape.c.y * SCALE -start.y*0.5f);
                vertices[1].Set(sf_shape.d.x * SCALE -start.x*0.5f, sf_shape.d.y * SCALE -start.y*0.5f);

                b2PolygonShape polyShape;
                polyShape.Set(vertices, 4);
                 
                myFixture.shape = &polyShape;
                myFixture.density = 1;
                body->CreateFixture(&myFixture);

                pressed = false;
              }
            }
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      for(b2Body* b = world.GetBodyList(); b; b = b->GetNext()) {
        if(b->GetType() == b2_dynamicBody) {
          b->SetTransform(b2Vec2(cubeStart.x * SCALE ,cubeStart.y * SCALE), 0 );
          b->SetLinearVelocity(b2Vec2(0.1f,0));
          b->SetAngularVelocity(0);
        }
      }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
      lines.clear();
    }

    static bool toggle = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      if(!toggle) {
        paused = !paused;
        toggle = true;
      }
    } else {
      toggle = false;
    }

    if(!paused) world.Step(timeStep, velocityIterations, positionIterations);

    window.clear();

    
    size_t size = 0;
    sf::ConvexShape cShape;
    cShape.setFillColor(sf::Color::Red);
    //This loops through every single body in the game world
    for(b2Body* b = world.GetBodyList(); b; b = b->GetNext()) {
      //This loops through every fixture in the current body
      for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
        //This checks to see if the type of the fixture is a polygon, if it is then draw the polygon
        if(f->GetType() == b2Shape::e_polygon) {
          //Stores a pointer to the shape stored in the fixture
          b2PolygonShape* s = (b2PolygonShape*)f->GetShape();
          //Get the amount of vertices stored in the shape
          size = s->GetVertexCount();
          //Set the size of the object in SFML so it knows how many vertices the shape should have
          cShape.setPointCount(size);
          //Loop through the vertices and send them from Box2D to the SFML shape
          for(int i = 0; i < size; i++) {
            //Stores the current vertex in v
            b2Vec2 v = s->GetVertex(i);
            //Converts the vertex from its local space to where it is in the world
            cShape.setPoint(i, sf::Vector2f((b->GetWorldVector(v).x + b->GetPosition().x), (b->GetWorldVector(v).y + b->GetPosition().y)));
          }
          //Draws the shape onto the window
          window.draw(cShape);
        }
      }
    }

    // draw all elements in line vector
    for(int i = 0; i != lines.size(); ++i) {
      static sf::VertexArray quad(sf::Quads, 4);
      quad[0].position = sf::Vector2f(lines[i].a.x, lines[i].a.y);
      quad[1].position = sf::Vector2f(lines[i].b.x, lines[i].b.y);
      quad[2].position = sf::Vector2f(lines[i].c.x, lines[i].c.y);
      quad[3].position = sf::Vector2f(lines[i].d.x, lines[i].d.y);
      window.draw(quad);
    }

    // Change the play and pause Icon on screen based on state
    if(!paused) {
      static sf::VertexArray play(sf::Triangles, 3);
      vector2 location(10,10);
      play[0].position = sf::Vector2f(0  +location.x,  0 +location.y);
      play[1].position = sf::Vector2f(20 +location.x, 15 +location.y);
      play[2].position = sf::Vector2f(0  +location.x, 30 +location.y);
      
      play[0].color = sf::Color::Green;
      play[1].color = sf::Color::Green;
      play[2].color = sf::Color::Green;
      window.draw(play);
    } else {
      static sf::VertexArray pause1(sf::Quads, 4);
      static sf::VertexArray pause2(sf::Quads, 4);
      vector2 location(10,10);
      pause1[0].position = sf::Vector2f(0 +location.x, 0 +location.y);
      pause1[1].position = sf::Vector2f(5 +location.x, 0 +location.y);
      pause1[2].position = sf::Vector2f(5 +location.x, 30 +location.y);
      pause1[3].position = sf::Vector2f(0 +location.x, 30 +location.y);
      pause1[0].color = sf::Color::Blue;
      pause1[1].color = sf::Color::Blue;
      pause1[2].color = sf::Color::Blue;
      pause1[3].color = sf::Color::Blue;
      pause2[0].position = sf::Vector2f(10 +location.x, 0 +location.y);
      pause2[1].position = sf::Vector2f(15 +location.x, 0 +location.y);
      pause2[2].position = sf::Vector2f(15 +location.x, 30 +location.y);
      pause2[3].position = sf::Vector2f(10 +location.x, 30 +location.y);
      pause2[0].color = sf::Color::Blue;
      pause2[1].color = sf::Color::Blue;
      pause2[2].color = sf::Color::Blue;
      pause2[3].color = sf::Color::Blue;
      window.draw(pause1);
      window.draw(pause2);
    }

  window.display();
  }
	return 0;
}
