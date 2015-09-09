#include "cgl/texture.h"
#include "cgl/lodepng.h"

using namespace std;

void CglTexture::loadBMP(string filename)
{
  // Data read from the header of the BMP file
  unsigned char header[54]; // Each BMP file begins by a 54-bytes header
  unsigned int dataPos;     // Position in the file where the actual data begins
  width = 128;
  height = 128;
  unsigned int imageSize;   // = width*height*3
  // Actual RGB data
  unsigned char * data;
  // Open the file
  FILE * file = fopen(filename.c_str(),"rb");
  if (!file){
    cout << "Could not open " << filename << endl;
    exit(0);
  }

  if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
    exit(0);
  }
  if ( header[0]!='B' || header[1]!='M' ){
    printf("Not a correct BMP file\n");
    exit(0);
  }
    // Some BMP files are misformatted, guess missing information
  if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
  if (dataPos==0)      dataPos=54; // The BMP header is done that way
  // Create a buffer
  data = new unsigned char [imageSize];

  // Read the actual data from the file into the buffer
  fread(data,1,imageSize,file);
  fclose(file);

  glEnable(GL_TEXTURE_2D);
  glGenTextures( 1, &ID );
  glBindTexture( GL_TEXTURE_2D, ID );

  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glDisable(GL_TEXTURE_2D);
  free( data );
}


void CglTexture::loadPNG(string filename){
  // Load file and decode image.
  std::vector<unsigned char> image;
  unsigned error = lodepng::decode(image, width, height, filename.c_str());
  if(error != 0){
    std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl; exit(1);
  }

  // Enable the texture for OpenGL.
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
}
