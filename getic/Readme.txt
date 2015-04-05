========================================================================
MYG FILE FORMAT (VERSION 8)

========================================================================
	bin/creamshape.exe
	   /edfiles /*.bmp
				/*.tga
				/*.jpg

========================================================================
The file contain brushes in their inital shape, but already transformed
in the world position. There resulting brushes done by CSG are not saved 
in file yet. 
TBD: To build a BSP some steps have to be done with the saved geometry.

========================================================================
1.	Perform all the cuts witht he cut brushes against solid brushes.
2.	Run a HSR algotithm on resulted geometry.
3.	Fix the polys that get out of their plane by running the HSR and CUT
4.	Build the BSP
5.	Fix the T Junction.
6	Save the BSP in file.
	   	
========================================================================
----------------------------------myg file structure:

structure member alignment is 8 bytes.

---------------------compiler data types used in file:

BYTE	- 8  bits (unsigned)
int		- 32 bits (signed)
float	- 32 bits (floating number)
DWORD	- 32 bits (unsigned)
char	- 8 bits  (signed) (ANSI not unicode)

octet   = 8 bits  (generic 8 bits)	
-------------------------------------user data types:

- |3d point structure|------------------------------
struct V3	
{
	float x;
	float y;
	float z;
};

-|color structure|-----------------------------------
struct CLR
{
	byte rgba[4];
};

-|texture coord structure|---------------------------
struct UV
{
	float u;
	float v;
};

- vertex structure ----------------------------------
struct VTCI //(vector, texture1, color, texture2 )
{
	BYTE	user;
	V3		xyz;
	UV		tex[2];
	CLR		color;
};

- matrix structure ------------------------------------
- open-gl column based transformation matrix.
struct M4
{
	float mat[16];
};

- plane structure -------------------------------------

struct plane
{
	V3		normal;
	float	const;
	int		type;
};
where type is

PLANE_X		0 - plane lays down on y0z
PLANE_Y		1 - plane lays down on x0z
PLANE_Z		2 - plane lays down on x0y
PLANE_ANYX	3 - plane lays close to y0z
PLANE_ANYY	4 - plane lays close to x0z
PLANE_ANYZ	5 - plane lays close to x0y
PLANE_ANY	6 - plane is not typed yet

- bounding sphere structure ----------------------------

struct BSphere
{
	V3     center;
    REAL   radius;
};

-- bounding box structure ------------------------------
struct BBox
{
	V3	 vmin;
	V3	 vmax;
};

- texture coord parameters polygon structure------------
struct TexCoords
{
	REAL	rotate;
    REAL	scalex;
    REAL	scaley;
    REAL	shiftx;
    REAL	shifty;
    V3      texNorm[2];	// texture normal 
};

//== FILE LAYOUT==========================================
** brushes have tr matrix already applyed to the saved brush
** tr matrix is built by scalation, rotation and translation
** vectors. Thay are saved in file but do not apply the values 
** to the brush you read. 

data--------------description-----------------------------
int				  file version (8)	
V3				  camera position
V3				  camera azimuth, elevation, roll-over
int				  brush count
------------------Each Brush(0 to brush count-1)
 V3				  brush translation (further dev (! used))		
 V3				  brush rotation    (further dev (! used))		
 V3				  brush scalation   (further dev (! used))		
 DWORD			  brush flags (see brush flags section)
 int			  brush group 
 int			  polygon count for this brush
 -----------------Each Polygon in this Brush (0 to poly count-1)
  char[80] 		  texture file name.  (no path just filename.ext)
  DWORD			  polygon flags (see poly flags section)
  TexCoords		  texture coord arttributes. (already applyed to the texture)
  int			  vertex count
  ----------------Each vertex from (0 to vertexcount-1)
   VTCI			  polygon vertex	
  -next vertex
 -next poly
-next brush
-----------------------------------------------------------

========================================================================
Polygon Flags:

BACK_CUT_POLY	0x1	- internally used. It may end up in the file 
SPLITTER_POLY	0x2 - internally used. It may end up in the file 
POLY_USER       0x4 - internally used. It may end up in the file 
POLY_DELETED	0x8 - internally used. It may end up in the file 

========================================================================
Brush Flags:

BRSH_SOLID      0x1		-	Brush is solid (USED)/ otherwise is cut
BRSH_REV        0x2		-	Brush has the faces reversed
BRSH_DETAIL	0x4		-   Brush is not split or unioned with other brushes
BRSH_DIRTY      0x8		-	Internally used
BRSH_SELBOX     (0x4|0x10)  - Internally used
BRSH_SELVIS     0x20	- Internally used
BRSH_NEW        0x40	- Internally used


