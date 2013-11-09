#include <Project\levels\mainlevel.h>

class level1 : mainLevel {
  
  public:

  bool active;
  b2World* world1;
  b2RevoluteJoint* wheel;
  b2MouseJoint* mouseJoint;

  void CreateLevel() {
    b2Vec2 gravity = b2Vec2(0,30.0f);
	  world1 = new b2World(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0,0);
    b2Body* groundBody = world1->CreateBody(&groundBodyDef);

    vector2 bikePos(-35,-20);

    // Construct the Bike Frame
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0+bikePos.x,0+bikePos.y);
    b2Body* bikeFrame = world1->CreateBody(&bodyDef);

    b2PolygonShape frameShape;
    b2FixtureDef fixtureDef;

    b2Vec2 frameVerts [4];
      frameVerts[3].Set(8,73);
      frameVerts[2].Set(48,74);
      frameVerts[1].Set(91,22);
      frameVerts[0].Set(37,39);

    frameShape.Set(frameVerts, 4);
    fixtureDef.shape = &frameShape;
    fixtureDef.restitution = 0.1f;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    bikeFrame->CreateFixture(&fixtureDef);

    // Construct Wheel 1
    b2BodyDef bikeWheel1;
    bikeWheel1.type = b2_dynamicBody;
    bikeWheel1.position.Set(6+bikePos.x,73+bikePos.y);
    b2Body* bikeWheel1body = world1->CreateBody(&bikeWheel1);

    b2FixtureDef bikeW1fixtureDef;
    b2CircleShape bikeWheel1Shape;
    bikeWheel1Shape.m_radius = 40;
    bikeWheel1Shape.m_p.Set(0,0);
    bikeW1fixtureDef.shape = &bikeWheel1Shape;
    bikeW1fixtureDef.restitution = 0.2f;
    bikeW1fixtureDef.density = 1.0f;
    bikeW1fixtureDef.friction = 1.0f;
    bikeWheel1body->CreateFixture(&bikeW1fixtureDef);

    // Construct Wheel 2
    b2BodyDef bikeWheel2;
    bikeWheel2.type = b2_dynamicBody;
    bikeWheel2.position.Set(111+bikePos.x,70+bikePos.y);
    bikeWheel2.angle = 60 * 0.0174532925f;
    b2Body* bikeWheel2body = world1->CreateBody(&bikeWheel2);

    b2CircleShape bikeWheel2Shape;
    b2FixtureDef bikeW2fixtureDef;
    bikeWheel2Shape.m_radius = 40;
    bikeWheel2Shape.m_p.Set(0,0);
    bikeW2fixtureDef.shape = &bikeWheel2Shape;
    bikeW2fixtureDef.restitution = 0.2f;
    bikeW2fixtureDef.density = 0.2f;
    bikeW2fixtureDef.friction = 5.0f;
    bikeWheel2body->CreateFixture(&bikeW2fixtureDef);

    // Join the wheels to the Bike - Only rear wheel is a motor.
    b2RevoluteJointDef w1joint;
    w1joint.Initialize(bikeFrame, bikeWheel1body, b2Vec2(6+bikePos.x,73+bikePos.y));
    w1joint.enableMotor = true;
    w1joint.motorSpeed = 0;
    w1joint.maxMotorTorque = 5000000;
    world1->CreateJoint(&w1joint);
    // Joint 2
    b2RevoluteJointDef w2joint;
    w2joint.Initialize(bikeFrame, bikeWheel2body, b2Vec2(111+bikePos.x,70+bikePos.y));
    world1->CreateJoint(&w2joint);

    b2MouseJointDef dotDragDef;
    dotDragDef.bodyA = groundBody;
    dotDragDef.bodyB = bikeFrame;
    dotDragDef.target.Set(0,0);
    dotDragDef.maxForce = 9000000.0f;

    b2MouseJoint* dotDrag = (b2MouseJoint*)world1->CreateJoint(&dotDragDef);


    FindJoint();

    active = true;
  }
  
  // Find the joint in the world that has an enabled motor (The rear bike Wheel)
  void FindJoint () {
    for(b2Joint* joint = world1->GetJointList(); joint; joint=joint->GetNext()) {
      if(joint->GetType() == e_revoluteJoint) {
        wheel = (b2RevoluteJoint*)joint;
        if(wheel->IsMotorEnabled()) {
          return;
        }
      } else if (joint->GetType() == e_mouseJoint) {
        mouseJoint = (b2MouseJoint*)joint;
      }
    }
  }

  void Update () {
    if(!active) return;

    static float speed = 0;
    static float maxSpeed = 2;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      speed = wheel->GetMotorSpeed();
      speed += 0.005f;
      if(speed > maxSpeed)
        speed = maxSpeed;
      wheel->SetMotorSpeed(speed);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      speed = wheel->GetMotorSpeed();
      speed -= 0.005f;
      if(speed < -maxSpeed)
        speed = -maxSpeed;
      wheel->SetMotorSpeed(speed);
    }
    
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      speed = wheel->GetMotorSpeed();
      speed *= 0.95f;
      wheel->SetMotorSpeed(speed);
    }
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
      mouseJoint->SetMaxForce(999999999);
      mouseJoint->SetTarget(b2Vec2(sf::Mouse::getPosition().x - 463,sf::Mouse::getPosition().y-70));
    } else {
      mouseJoint->SetMaxForce(0);
    }
  }
};