#include <Project\levels\mainlevel.h>

class level1 : mainLevel {
  
  public:

  void CreateLevel() {
    b2Vec2 gravity = b2Vec2(0,5.0f);
	  b2World* world1 = new b2World(gravity);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(250 * SCALE , 150 * SCALE);
    b2Body* body = world1->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    b2FixtureDef fixtureDef;
    dynamicBox.SetAsBox(10.0f*0.5f, 10.0f*0.5f);
    fixtureDef.shape = &dynamicBox;
    fixtureDef.restitution = 1.0f;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
  }
};