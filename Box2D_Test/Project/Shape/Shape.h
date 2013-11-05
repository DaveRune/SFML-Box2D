class shape {
  
  public:

  int aX,aY,bX,bY,cX,cY,dX,dY;

  shape(int a, int b, int c, int d, int e, int f, int g, int h){
    aX = a;   aY = b;
    bX = c;   bY = d;
    cX = e;   cY = f;
    dX = g;   dY = h;
  }

  shape(){}

  shape(int sX, int sY, int eX, int eY) {
    aX = sX;
    aY = sY + 10;
    bX = sX;
    bY = sY - 10;
    cX = eX;
    cY = eY + 10;
    dX = eX;
    dY = eY - 10;
  }

  void Normalize() {
    
  }

  void GetDirection () {

  }

};