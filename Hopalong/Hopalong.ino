// Hopalong orbit fractal //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define WIDTH 256
#define HEIGHT 256
#define SCR (WIDTH * HEIGHT)
#define ITER 99

  uint16_t xx, yy;
  uint16_t coll;
  uint8_t cnt = ITER;
  char msg[30];
  
  int iterations = 15000;
  long ranfseed;
  float randf() {return (ranfseed=(ranfseed*1629+1)%1048576)/1048576.0f;}
  float a, b, c, x, y, t;
   
byte replicate(byte color)
{
  return (color << 6) | (color << 4) | (color << 2) | color;
}
           
void setpixel(byte x, byte y, byte color)
{
  unsigned int addr = RAM_SPRIMG|(x & 0xf)|(y << 4)|((x & 0x30)<<8);
  byte mask = 0xc0 >> ((x >> 5) & 6);
  GD.wr(addr, (GD.rd(addr) & ~mask) | (color & mask));
}

void rndrule() {

  x = 0.1f;
  y = 0.1f;
  t = 0.0f;
  a = 0.0f;
  b = 0.0f;
  c = 0.0f;

  ranfseed = 1+rand()%SCR;
  
  float pmax = WIDTH, logpmax = logf(pmax);
  
  a = expf(randf()*logpmax);
  b = expf(randf()*logpmax);
  c = randf()*pmax;  

}

void setup()
{
  int i;
  GD.begin();
  GD.microcode(randreg_code, sizeof(randreg_code));
  GD.ascii();
  for (i = 0; i < 256; i++) {
  int x = 72 + 16 * ((i >> 4) & 15);
  int y = 22 + 16 * (i & 15); 
  int image = i & 63;
  int pal =   3-(i >> 6);
  GD.sprite(i, x, y, image, 0x8 | (pal << 1), 0,0);}
  GD.fill(RAM_SPRIMG, 0, 16384);
  GD.wr16(PALETTE4A, RGB(256,0,0));
  GD.wr16(PALETTE4A + 2, RGB(255,255,0));
  GD.wr16(PALETTE4A + 4, RGB(0,255,255));
  GD.wr16(PALETTE4A + 6, RGB(255,0,255));
  GD.putstr(14, 1, "Hopalong orbit fractal");
  GD.putstr(16, 36, "Created by JLS 2020");
  GD.putstr(43, 36, "cnt");

  srand(GD.rd(0x280E));

  rndrule();
    
}


void loop()
{ 
  
  GD.putstr(46, 36, msg);
  sprintf(msg, "%3d", cnt);
  
  coll = rand();

  for (int i=0;i<iterations;i++) {

    float nx = x;
    float ny = y;

    t = sqrtf(fabs(b*nx-c));
    x = ny - ((nx<0) ? t : -t);
    y = a - nx;

    int ax = (WIDTH/2) + (x/2);
    int ay = (HEIGHT/2) + (y/2);
      
    if (ax>0 && ax<WIDTH && ay>0 && ay <HEIGHT) setpixel(ax, ay, replicate((coll + 1) & 3)); 

  }

  if (cnt == 0) {
    
    cnt = ITER;
    GD.fill(RAM_SPRIMG, 0, 16384);
    rndrule();
    
  }
  
  cnt--;
 
  GD.waitvblank();
    
}
