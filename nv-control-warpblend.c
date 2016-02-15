/*
 * Copyright (c) 2013 NVIDIA Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "nv-control-warpblend.h"
#include "nv-control-screen.h"
#include <stdlib.h>
#include <sys/utsname.h>

/*
static void print_display_device_target_indices(const int *pData)
{
  int i;
  for (i = 1; i <= pData[0]; i++) {
    printf("%s-%d\n", "DPY", pData[i]);
  }
}
*/

typedef struct __attribute__((packed)) {
  float x, y;
} vertex2f;

typedef struct __attribute__((packed)) {
  vertex2f pos;
  vertex2f tex;
  vertex2f tex2;
} vertexDataRec;


int main(int ac, char **av)
{
  Display *xDpy = XOpenDisplay(NULL);
  int i, j, gpu;
  int *pDisplayData;
  //Get connected display devices on the present X screen
  Bool ret;
  int len;
  int screenId=0;
  GC gc;
  XGCValues values;
  Pixmap blendPixmap;
  int nvDpyId;
  srand(time(NULL));

  //For each GPU, get corresponding DPYS
  for(gpu=0;gpu<2;gpu++){
    ret = XNVCTRLQueryTargetBinaryData(xDpy,
				       NV_CTRL_TARGET_TYPE_GPU,
				       gpu, // target_id
				       0, // display_mask
				       NV_CTRL_BINARY_DATA_DISPLAYS_CONNECTED_TO_GPU,
				       (unsigned char **) &pDisplayData,
				       &len);
    if (!ret) {
      fprintf(stderr, "Failed to query connected displays\n");
      return 1;
    }

    for (i = 1; i <= pDisplayData[0]; i++) {
      nvDpyId = pDisplayData[i];
      int W = 1920;
      int H = 1200;
      blendPixmap = XCreatePixmap(xDpy, RootWindow(xDpy, screenId), W, H, DefaultDepth(xDpy, screenId));
      //fprintf("%d",DefaultDepth(xDpy, screenId));
      int gradientLimit = 792;
      int resol = 254;
      int meanOffset = (gradientLimit - resol)/2;//269
      int offset_white = meanOffset +0;
      int offset_black = 2*meanOffset - offset_white;
      long unsigned int black = 0x000000;
      long unsigned int white = 0xFFFFFF;
      long unsigned int step = 0x01010101;
  
      //GPU - 0 : Right screen
      if(gpu==0){
	//Fill left with black
	values.foreground = black;
	gc = XCreateGC(xDpy, blendPixmap, GCForeground, &values);
	XFillRectangle(xDpy, blendPixmap, gc, 0, 0, offset_black, H);
	
	//Blending
	for(j = offset_black ; j < gradientLimit - offset_white; j++){
	  //printf("%d\n", j);
	  values.foreground += step;
	  XChangeGC(xDpy, gc, GCForeground, &values);
	  XFillRectangle(xDpy, blendPixmap, gc, j, 0, j+1, H);
	}

	//Fill right with white
	values.foreground = white;
	gc = XCreateGC(xDpy, blendPixmap, GCForeground, &values);
	XFillRectangle(xDpy, blendPixmap, gc, gradientLimit-offset_white, 0, W, H);
	
      }

      //GPU - 1 : Left screen
      else if(gpu==1){
	//Fill left with white
	values.foreground = white;
	gc = XCreateGC(xDpy, blendPixmap, GCForeground, &values);
	XFillRectangle(xDpy, blendPixmap, gc, 0, 0, W-gradientLimit+offset_white, H);
	//Blending
	for(j = W-gradientLimit+offset_white ; j < W-offset_black+1 ; j++){
	  values.foreground -= step;
	  XChangeGC(xDpy, gc, GCForeground, &values);
	  XFillRectangle(xDpy, blendPixmap, gc, j, 0, j+1, H);
	}	
	//Fill right with black
	values.foreground = black;
	gc = XCreateGC(xDpy, blendPixmap, GCForeground, &values);
	XFillRectangle(xDpy, blendPixmap, gc, W-offset_black, 0, W, H);  
      }		

      /*
      unsigned int w,h,xH,yH;
      Pixmap test;
      if(gpu==0){
	XReadBitmapFile(xDpy, blendPixmap, "/home/norgeot/toto1.xbm", &w, &h, &test, &xH, &yH);
	blendPixmap=test;
	//XWriteBitmapFile(xDpy, "/home/norgeot/toto.xbm", blendPixmap , 1920,1200,-1,-1);
      }
      */

      // Apply it to the display.
      //char name[200];
      //sprintf(name, "/home/norgeot/toto%d.xbm", gpu*2 + i);
      //XWriteBitmapFile(xDpy, name, blendPixmap , 1920,1200,-1,-1);
      
      XNVCTRLSetScanoutIntensity(xDpy,
				 screenId,
				 nvDpyId,
				 blendPixmap,
				 True); 
    }
    XFree(pDisplayData);


  }

  return 0;
}
