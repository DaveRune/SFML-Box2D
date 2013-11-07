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

b2Vec2 gravity(0.0f, 5.0f);
b2World* world = new b2World(gravity);
b2BodyDef bodyDef;
b2Body* body = world->CreateBody(&bodyDef);
b2PolygonShape dynamicBox;
b2FixtureDef fixtureDef;

void CreateWorld() {
  // Define the gravity vector.
	b2Vec2 gravity(0.0f, 5.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World* world = new b2World(gravity);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(cubeStart.x * SCALE ,cubeStart.y * SCALE);
	b2Body* body = world->CreateBody(&bodyDef);
  bodyDef.awake = false;
	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(10.0f*0.5f, 10.0f*0.5f);
	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
  fixtureDef.restitution = 1.0f;
	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;
	// Override the default friction.
	fixtureDef.friction = 0.3f;
	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);
}

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Physics Game!");

  delete world;
  CreateWorld();

  #pragma region Background Graphics
  // Load the background images, create a sprite and assign the image.
  // Sky
  sf::Texture backgroundTexture;
  backgroundTexture.loadFromFile("assets/Sky.png");
  backgroundTexture.setSmooth(true);
  backgroundTexture.setRepeated(true);
  sf::Sprite background;
  background.setPosition(0,0);
  background.setScale((float)window.getSize().x,1.0f);
  background.setTexture(backgroundTexture);

  // Sun
  sf::Texture sunTexture;
  sunTexture.loadFromFile("assets/Sun.png");
  sunTexture.setSmooth(true);
  sunTexture.setRepeated(true);
  sf::Sprite sun;
  sun.setPosition(window.getSize().x - 250 - window.getSize().x*0.2f,100);
  sun.setScale(1.0f,1.0f);
  sun.setTexture(sunTexture);

  // Cloud
  sf::Texture cloud1Texture;
  cloud1Texture.loadFromFile("assets/Cloud1.png");
  cloud1Texture.setSmooth(true);
  cloud1Texture.setRepeated(false);
  sf::Sprite cloud1;
  cloud1.setPosition(window.getSize().x*0.15f,80);
  cloud1.setScale(1.0f,1.0f);
  cloud1.setTexture(cloud1Texture);

  // Hills
  sf::Texture hillsTexture;
  hillsTexture.loadFromFile("assets/Hills.png");
  hillsTexture.setSmooth(false);
  hillsTexture.setRepeated(true);
  sf::Sprite hills1;
  hills1.setPosition(0,window.getSize().y - 184);
  hills1.setScale(1.0f ,1.0f);
  hills1.setTexture(hillsTexture);
  sf::Sprite hills2;
  hills2.setPosition(766,window.getSize().y - 184);
  hills2.setScale(1.0f,1.0f);
  hills2.setTexture(hillsTexture);
  sf::Sprite hills3;
  hills3.setPosition(766*2,window.getSize().y - 184);
  hills3.setScale(1.0f,1.0f);
  hills3.setTexture(hillsTexture);
  #pragma endregion

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

  std::vector<shape> lines;
  std::vector<b2Body> *b2lines;

  #pragma region events
  while (window.isOpen()){
    sf::Event event;
    while (window.pollEvent(event))
    {
      // If the window is closed, close the SFML window
      switch (event.type) {

      case sf::Event::Closed:
          window.close();
          break;

      // Resize window : set new size
      case sf::Event::Resized:
          window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)event.size.width, (float)event.size.height)));

          // Rescale and Reposition the background elements
          background.setScale((float)window.getSize().x,1.0f);
          sun.setPosition(window.getSize().x - 250 - window.getSize().x*0.2f,100);
          hills1.setPosition(0,window.getSize().y - 184);
          hills2.setPosition(766,window.getSize().y - 184);
          hills3.setPosition(766*2,window.getSize().y - 184);
          break;

      case sf::Event::MouseButtonPressed:
          static bool pressed = false;
          static vector2 start,end;

          // When we click save the positions to a Vector2
          if(event.mouseButton.button == sf::Mouse::Left) {
            // If we are drawing lines constantly, dont perform click drawing
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
              b2Body* body = world->CreateBody(&myBody);
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
    #pragma endregion

    #pragma region Reset 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      // Clear all SFML Lines
      lines.clear();

      // Reset the Box
      for(b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
        if(b->GetType() == b2_dynamicBody) {
          b->SetTransform(b2Vec2(cubeStart.x * SCALE ,cubeStart.y * SCALE), 0 );
          b->SetLinearVelocity(b2Vec2(0.1f,0));
          b->SetAngularVelocity(0);
        }
      }
      
      // Pause the game
      paused = true;

      // Reset the World
      delete world;
      CreateWorld();
    }
    #pragma endregion 

    #pragma region Pause
    static bool toggle = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      if(!toggle) {
        paused = !paused;
        toggle = true;
      }
    } else {
      toggle = false;
    }
    #pragma endregion

    if(!paused) world->Step(timeStep, velocityIterations, positionIterations);

    window.clear();

    #pragma region BackgroundArt
    window.draw(background);
    window.draw(sun);
    window.draw(cloud1);
    window.draw(hills1);
    window.draw(hills2);
    window.draw(hills3);
    #pragma endregion

    #pragma region DrawSFML
    size_t size = 0;
    sf::ConvexShape cShape;
    cShape.setFillColor(sf::Color::Red);
    //This loops through every single body in the game world
    for(b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
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
    #pragma endregion

    #pragma region DrawClickedLines
    // draw all elements in line vector
    for(int i = 0; i != lines.size(); ++i) {
      static sf::VertexArray quad(sf::Quads, 4);
      quad[0].position = sf::Vector2f(lines[i].a.x, lines[i].a.y);
      quad[1].position = sf::Vector2f(lines[i].b.x, lines[i].b.y);
      quad[2].position = sf::Vector2f(lines[i].c.x, lines[i].c.y);
      quad[3].position = sf::Vector2f(lines[i].d.x, lines[i].d.y);
      for(int j = 0; j != quad.getVertexCount(); ++j)
        quad[j].color = sf::Color::Green;
      window.draw(quad);

      // Create shadows under all the lines
      static sf::VertexArray shadow(sf::Quads, 4);
      shadow[0].position = sf::Vector2f(lines[i].a.x, lines[i].a.y);
      shadow[1].position = sf::Vector2f(lines[i].b.x, lines[i].b.y);
      shadow[2].position = sf::Vector2f(lines[i].c.x, lines[i].c.y+80);
      shadow[3].position = sf::Vector2f(lines[i].d.x, lines[i].d.y+80);
      shadow[0].color = sf::Color::Color(0,0,0,80);
      shadow[1].color = sf::Color::Color(0,0,0,80);
      shadow[2].color = sf::Color::Color(0,0,0,0);
      shadow[3].color = sf::Color::Color(0,0,0,0);
      window.draw(shadow);
    }
    #pragma endregion

    #pragma region Play/Pause
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
    #pragma endregion


  window.display();
  }
	return 0;
}