/*
---------------------------------------
	GRAFX.C
	
	Diverse sm†tt & gott...
---------------------------------------
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include "grafx.h"


void ShowMouse( int force )
{
}

void HideMouse( void )
{
}

int MouseButtons( void )
{
  /*return out.w.bx;*/
	return 0;
}

void SetMouseShape( word *shape )
{
  if (shape) shape = NULL;
}

void SetMouseArrow( void )
{
}

void Pause( int ticks )
{
  /*delay( ticks);*/
}

int GetFont( void )
{
  return 0;
}

void SetColor( word c )
{
//  _setcolor( c);
////  _setfillmask( SOLID_FILL, c);
}

word GetColor( void )
{
  /*return _getcolor();*/
	return 0;
}

void SetBgColor( word c )
{
//  if (c) c = 0;
////  setfillstyle( SOLID_FILL, c);
}

word GetBgColor( void )
{
  return 0;
}

void SetLineStyle( word c )
{
  /*_setlinestyle( c);*/
}

word GetLineStyle( void )
{
  return 0;
}

void SetWriteMode( word c )
{
  if (c) c = 0;
}

word GetWriteMode( void )
{
  return 0;
}

void SetTextStyle( word c )
{
  if (c) c = 0;
}

word GetTextStyle( void )
{
  return 0;
}

void SetTextMode( word c )
{
  if (c) c = 1;
}

word GetTextMode( void )
{
  return 0;
}

void SetFillStyle( word *c )
{
  if (c) c = 0;
}

word *GetFillStyle( void )
{
  return NULL;
}

void Line( int x1, int y1, int x2, int y2 )
{
  /*_moveto( x1, y1);
  _lineto( x2, y2);*/
}


void OutTextXY( int x, int y, char *s )
{
  /*_moveto( x, y);
  _outgtext( s);*/
}

word Swap( word w )
{
  return (w >> 8) | ((w & 0xFF) << 8);
}

word Reverse( word i )
{
  return ((i & 8) >> 3) + ((i & 4) >> 1) + ((i & 2) << 1) + ((i & 1) << 3);
}

int Kbshift( int x )
{
  /*return out.h.al;*/
	return 0;
}

int ReadKey( void )
{
  int c = getch();
  if (c == 0)
    return getch() << 8;
  else
    return c;
}
