// Mandelbulber fractal //

#include "SPI.h"
#include "GD.h"
#include "randreg.h"

#define WIDTH 256
#define HEIGHT 256

  int maxIterations = 2;
  float power = 5.0f;
  float xmin = -1.0f;
  float ymin = -1.0f;
  float zmin = -1.0f;
  float wh = 2.0f;
  float addx = wh/WIDTH;
  float addy = wh/HEIGHT;
  float x = xmin, y = ymin;  
   
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

void mandelbulber(){ 

  x = xmin;

  for (int i = 0; i < WIDTH; i++) {

    y = ymin;

    for (int j = 0; j < HEIGHT; j++) {
      
      float z = zmin;
      
      for (int k = 255; k > 0 && z < -1.0f*zmin; k--) {
        
        float dr = 1;
        float nx = x;
        float ny = y;
        float nz = z;
        float rad = sqrtf(x*x+y*y+z*z);
        int n = 0;
        
        while (n < maxIterations) {
          
          float powRad = powf(rad, power);
          float theta = atan2f(sqrtf(nx*nx+ny*ny), nz)*power;
          float phi = atan2f(ny, nx)*power;
          nx = sinf(theta)*cosf(phi)*powRad+x;
          ny = sinf(theta)*sinf(phi)*powRad+y;
          nz = cosf(theta)*powRad+z;
          dr = dr*power*powf(rad, power-1.0f);
          rad = sqrtf(nx*nx+ny*ny+nz*nz);
          if (rad > 4.0f) break;
          n++;  
            
        }
        
        if (0.5f*logf(rad)*rad/dr < 0.00001f) {   
  
          setpixel (i, j, replicate((RGB(k,k,k) + 1) & 3));

          break;
          
        } else {
          k--;
          z += 0.5f*logf(rad)*rad/dr;  
        }   
      }
      
      y += addy;
    }

  x += addx;

  }

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
  GD.putstr(16, 1, "Mandelbulber fractal");
  GD.putstr(16, 36, "Created by JLS 2020");

  srand(GD.rd(0x280E));
  
  mandelbulber();

}

void loop(){
  
  GD.waitvblank();
  
}
