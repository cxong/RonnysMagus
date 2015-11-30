#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include "grafx.h"
#include "pics.h"


static word Swap( word x )
{
  return (x >> 8) | ((x&0xFF) << 8);
}

void SwapPicInfo( word *p )
{
  *p = Swap( *p);
  p++;
  *p = Swap( *p);
  p++;
  *p = Swap( *p);
  p++;
}

void RearrangePic( word *pic, int size )
{
  int w, h, words, planes;
  word *p1, *p2, *p3;
  word *copy;

  w = pic[0];
  w = (w + 15) >> 4;
  h = pic[1];
  copy = malloc( size-4);
  memcpy( copy, pic + 2, size-4);

  p1 = pic + 2;
  p2 = copy;
  while (h > 0)
  {
    for (planes = 1; planes <= 4; planes++)
    {
      p3 = p2;
      for (words = 1; words <= w; words++)
      {
        *p1 = *p3;
        p3 += 4;
        p1++;
      }
      p2++;
    }
    p2 += (w - 1)*4;
    h--;
  }
  free( copy);
}

void ReadPics( const char *name, word **pics, int maxPics, word *p )
{
	FILE *f = fopen(name, "rb");
	if (f != NULL)
	{
		if (p)
			if (fread(p, 32, 1, f) != 1) goto bail;
		else
			fseek(f, 32, SEEK_SET);
		int index = 0;
		size_t bytesRead;
		do
		{
			word size;
			bytesRead = fread(&size, sizeof size, 1, f);
			if (bytesRead != 0 && size != 0)
			{
				pics[index] = malloc(size + 2);
				if (fread(pics[index], 4, 1, f) != 1) goto bail;
				if (fread(pics[index] + 3, size - 4, 1, f) != 1) goto bail;
				pics[index][0]++;
				pics[index][1]++;
			}
			else
				pics[index] = NULL;
			index++;
		}
		while (index < maxPics && bytesRead != 0);

	bail:
		fclose(f);
	}
}

void ErasePics( word **pics, int maxPics )
{
  int index;

  for (index = 0; index < maxPics; index++)
  {
    if (pics[ index] != NULL)
    {
      free( pics[ index]);
      pics[ index] = NULL;
    }
  }
}

void PutPic(int x, int y, word *addr, long mode)
{
  /*_putimage( x, y, addr, _GPSET);*/
}
