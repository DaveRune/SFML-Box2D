#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Project/Shape/Shape.h>
#include <cstdio>

int main2 (int argc, char** argv) {
  sf::RenderWindow window(sf::VideoMode(768, 768), "Physics Game");

  // Create the world with Gravity
  b2Vec2 gravity (0, 5);
  b2World world(gravity);

  // Create the Box2D physics box.
  b2FixtureDef myFixture;
  b2BodyDef myBody;
  myBody.type = b2_dynamicBody;
  myBody.position.Set(50, 50);
  b2Body* body = world.CreateBody(&myBody);
  b2Vec2 vertices[4];

  // Caluclate the positions of the Box2D Verts
  vertices[0].Set(-10, 10);
  vertices[3].Set( 10, 10);
  vertices[2].Set( 10,-10);
  vertices[1].Set(-10,-100);

  // vertices[0].Set(sf_shape.a.x * (1/60.0f), sf_shape.a.y * (1/60.0f));
  // vertices[3].Set(sf_shape.b.x * (1/60.0f), sf_shape.b.y * (1/60.0f));
  // vertices[2].Set(sf_shape.c.x * (1/60.0f), sf_shape.c.y * (1/60.0f));
  // vertices[1].Set(sf_shape.d.x * (1/60.0f), sf_shape.d.y * (1/60.0f));

  b2PolygonShape polyShape;
  polyShape.Set(vertices, 4);
                 
  myFixture.shape = &polyShape;
  myFixture.density = 1;
  body->CreateFixture(&myFixture);


  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      }
    }
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
  window.display();
  }
  return 0;
}