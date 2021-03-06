#define TRUE	1
#define FALSE	0

#define SGN(x)	((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

#define FORCE   0
#define NOFORCE 1

#define MOUSEK	MouseButtons()

#define TEXT_TRANSPARENT	1
#define TEXT_OVERWRITE		7

typedef unsigned short word;

#define FontHeight(x)	textheight( "Mg")
#define FontWidth(x)	textwidth( "M");

#define hidemouse HideMouse
#define showmouse(x) ShowMouse(x)

void HideMouse( void );
void ShowMouse( int force );
int  MouseButtons( void );
void SetMouseShape( word *shape );
void SetMouseArrow( void );
void Pause( int ticks );

int  GetFont( void );
void SetColor( word c );
word GetColor( void );
void SetBgColor( word c );
word GetBgColor( void );
void SetLineStyle( word c );
word GetLineStyle( void );
void SetWriteMode( word c );
word GetWriteMode( void );
void SetTextStyle( word c );
word GetTextStyle( void );
void SetTextMode( word c );
word GetTextMode( void );

void Line( int x1, int y1, int x2, int y2 );
void OutTextXY( int x, int y, char *s );

int  Kbshift( int x );
int  ReadKey( void );
