// Gumowski-Mira strange attractor //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define WIDTH 256
#define HEIGHT 256
#define ITER 60

  uint16_t xx, yy;
  uint16_t coll;
  uint8_t cnt = ITER;
  char msg[30];
  
  int iterations = 10000;
  float a = 0.000001f;
  float b = 0.05f;
  float m = -0.031f;

  float nx = 0.0f;
  float ny = 0.5f;
   
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

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}  

float mira(float x) {
  
  float xx = x * x;
  return (m * x) + ((2.0f * (1.0f - m) * xx) / (1.0f + xx));
  
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
  GD.putstr(9, 1, "Gumowski-Mira strange attractor");
  GD.putstr(16, 36, "Created by JLS 2020");
  GD.putstr(43, 36, "cnt");

  srand(GD.rd(0x280E));
    
}


void loop()
{ 
  
  GD.putstr(46, 36, msg);
  sprintf(msg, "%3d", cnt);
  
  coll = rand();

  for (int i = 0; i < iterations; i++) {

    float x = nx;      
    float y = ny;
    
    nx = y + a * (1.0f - (b * (y * y))) * y + mira(x);
    ny = -x + mira(nx);

    xx = -8 + (WIDTH/2) + 3.0f * nx; 
    yy = (HEIGHT/2) + 3.0f * ny;
    
    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) setpixel(xx, yy, replicate((coll + 1) & 3)); 

  }

  if (cnt == 0) {
    
    cnt = ITER;
    GD.fill(RAM_SPRIMG, 0, 16384);
    m = randomf(-0.95f, -0.05f);
    
  }
  
  cnt--;
 
  GD.waitvblank();
    
}
