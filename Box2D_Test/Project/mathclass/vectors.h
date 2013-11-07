#include <math.h>

class vector2 {

  public:
  
  float x,y;

  vector2(){
    x=0;y=0;
  }

  vector2(float x_, float y_) {
    x=x_;
    y=y_;
  }

  void Normalize() {
    float l = GetLength(x,y);
    x/=l;
    y/=l;
  }
  static vector2 Normalize(vector2 v) {
    float l = GetLength(v.x,v.y);
    v.x/=l;
    v.y/=l;
    return v;
  }

  static float GetLength (float x_, float y_) {
    return sqrt((x_*x_) + (y_*y_));
  }

  static float DotProduct (vector2 a, vector2 b) {
    return (a.x*b.x) + (a.y*b.y);
  }
  float DotProduct (vector2 v) {
    return (x*v.x) + (y*v.y);
  }

  static vector2 GetDirection (const vector2 start, const vector2 end) {
    vector2 v;
    v.x = start.x-end.x;
    v.y = start.y-end.y;
    return v;
  }

  static vector2 GetPerpendicular (vector2 v) {
    vector2 p;
    p.x = v.y;
    p.y = -v.x;
    return p;
  }

  void operator*=(const float f) {
    x*=f;
    y*=f;
  }

  void operator+=(const vector2 v) {
    x+=v.x;
    y+=v.y;
  }

  void operator-=(const vector2 v) {
    x-=v.x;
    y-=v.y;
  }

  bool operator==(const vector2 v) {
    if(x==v.x && y==v.y)
    return true;
    else return false;
  }

  vector2 operator+(const vector2 v) {
    return vector2(x+v.x, y+v.y);
  }

  vector2 operator-(const vector2 v) {
    return vector2(x-v.x, y-v.y);
  }

  static vector2 GetCenter(const vector2 start, const vector2 end) {
    vector2 c;
    c.x = (start.x + end.x)*0.5f;
    c.y = (start.y + end.y)*0.5f;
    return c;
  }


};