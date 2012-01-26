
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
using namespace std;

#define EMPTY -2000.000000

#define MIN_X -20.0000000
#define MAX_X 30.000000

#define MIN_Y 0.000000
#define MAX_Y 10.000000

#define MIN_Z 0.000000
#define MAX_Z 500.000000

#define Y_BOUND_TOP 3
#define Y_BOUND_BOT -3

float **coords;
float ruggedness = .5;

int main()
{
  FILE *ofp;
  int faceCount = 0;
  float xCoord, yCoord, zCoord;
  ofp = fopen("grid.m", "w");
  int coordCount = 0;
  int index2, index;
  int sizeRow = ((int)(MAX_X - MIN_X)), sizeCol = ((int) MAX_Z - MIN_Z);
  
  coords = (float**) malloc(sizeCol * (sizeof( float* )));
  for (index = 0; index < sizeCol; index++) {
    coords[index] = (float*)malloc(sizeRow * sizeof( float ));
    for (index2 = 0; index2 < sizeRow; index2++) {
      coords[index][index2] = EMPTY;
    }
  }

  coords[0][0] = MIN_Y;
  coords[((int)sizeRow - 1)][0] = MIN_Y;
  coords[0][((int)sizeCol - 1)] = MIN_Y;
  coords[((int)sizeRow-1)][((int)sizeCol - 1)] = MIN_Y;

  for (index2 = 0; index2 < sizeRow; index2++) {
    coords[index2][0] = MIN_Y + (rand()*1.0 / RAND_MAX * 2.0 - 1.0)*ruggedness;
  }

  for (index = 1; index < sizeCol; index++) {
    for (index2 = 0; index2 < sizeRow; index2++) {
      coords[index][index2] = coords[index][index2 - 1] + 
	(rand()*1.0 / RAND_MAX * 2.0 -1.0) * ruggedness;
      if (coords[index][index2] > Y_BOUND_TOP) {
	coords[index][index2] -= ruggedness;
      }
      if (coords[index][index2] < Y_BOUND_BOT) {
	coords[index][index2] += ruggedness;
      }
    }
  }
   

  for (index = 0; index < sizeCol; index++) {
    for (index2 = 0; index2 < sizeRow; index2++) {
      coordCount++;
      //2?
      xCoord = 2*(MIN_X + 2 * index2);
      yCoord = coords[index][index2];
      zCoord = MIN_Z + 2 * index;
      fprintf(ofp, "Vertex %d %f %f %f\n", coordCount, xCoord, yCoord, 
	      zCoord);
      
    }
  }
  
  int colPos = 0, rowPos = 0;
  int colDispl = 1, rowDispl = sizeRow;
  int pos, row, col;

  cout << "SizeCol: " << sizeCol << "\n";
  cout << "SizeRow: " << sizeRow << "\n";

  for (row = 1; row < sizeRow; row++) {
    for (col = 0; col < sizeCol - 1; col++) {
      
      faceCount++;
      pos = row + (sizeRow * col);

      fprintf(ofp, "Face %d %d %d %d\n", faceCount,
	      pos + colDispl, pos, pos + rowDispl);
      faceCount++;
      fprintf(ofp, "Face %d %d %d %d\n", faceCount,
	      pos + rowDispl, pos + colDispl + rowDispl, pos + colDispl);
    }
  }
}


