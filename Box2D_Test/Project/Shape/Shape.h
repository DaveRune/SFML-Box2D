#include <Project/mathclass/vectors.h>

class shape {
  
 

  vector2 direction_;

  public:

   vector2 a,b,c,d;

  shape(){}
  ~shape(){}

  shape(vector2 a_, vector2 b_, vector2 c_, vector2 d_){
    a.x = a_.x;   a.y = a_.y;
    b.x = b_.x;   b.y = b_.y;
    c.x = c_.x;   c.y = c_.y;
    d.x = d_.x;   d.y = d_.y;
  }

  vector2 GetDirection () { return direction_; }

  shape(vector2 start, vector2 end, float width) {
    vector2 direction = direction.GetDirection(start,end);
    direction = direction.GetPerpendicular(direction);
    direction.Normalize();
    direction *= width;

    a.x = start.x + direction.x;
    a.y = start.y + direction.y;
    b.x = end.x + direction.x;
    b.y = end.y + direction.y;
    c.x = end.x - direction.x;
    c.y = end.y - direction.y;
    d.x = start.x - direction.x;
    d.y = start.y - direction.y;
  }

};