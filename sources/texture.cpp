#include "cgl/texture.h"
using namespace std;

void CglTexture::load(string filename)
{
  /*
  //http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture
  unsigned char * data;

  FILE * file;
  file = fopen( filename.c_str(), "rb" );

  if ( file == NULL ){
    cout << "Unable to open texture " << filename << endl;
    exit(1000);
  }

  width  = 128;
  height = 128;

  data = (unsigned char *)malloc( width * height * 3 );
  fread( data, width * height * 3, 1, file );
  fclose( file );

  for(int i = 0; i < width * height ; ++i){
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];
    data[index] = R;
    data[index+2] = B;
  }
  */


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

  //Everything is in memory now, the file can be closed
  fclose(file);

  glEnable(GL_TEXTURE_2D);
  glGenTextures( 1, &ID );
  glBindTexture( GL_TEXTURE_2D, ID );


  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


  /*
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
  */
  glDisable(GL_TEXTURE_2D);
  free( data );
}
