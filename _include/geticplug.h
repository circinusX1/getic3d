/**
    File:   
    Author: Marius Chincisan
    Desc:   Interfaces definition for importing/exporting plugins for geometry files
    History:
*/
#ifndef _GETIC_PLUGIN_H__
#define _GETIC_PLUGIN_H__

//---------------------------------------------------------------------------------------
#include "baselib.h"            //  uses #pragma pack(push, 8)
#include "bspfilestr.h"         // for brushes and faces flags

#define _PLUG_MSG0  55000       // internaly used by editor


//---------------------------------------------------------------------------------------
// Item structure, passed out to export things and in ...
typedef enum _ClassType
{
    CLASS_IS_PAIR=0,
    CLASS_IS_LIGHT,
    CLASS_IS_ITEM,
}ClassType;
//---------------------------------------------------------------------------------------
typedef struct _Plg_Item
{
    ClassType   classType;      // set this one to make the classname bridgeable
	char        classname[16];  // a new class will be created
    int         nPairs;
    V3          position;
    union 
    {
        // generic types. will be packed in as they are "key"="value"
        struct 
        {
            char key[16];
            char value[48];
        }_Pair;
        // editor fundamental default items.
        struct 
        {
            float   radius;
            float   intensity;
            float   direction;          // euler
            float   specularangle;
            float   attenuation;
            BYTE    color[4];           // ambient, spec diffuse
        }_Light;
        struct 
        {
            REAL     bouningbox[4];
            REAL     efectpoint[3];
        }_Triger;
    }_U;
    
}Plg_Item;

//---------------------------------------------------------------------------------------
// Poly flags set up by editor when exports are same as in bspfilestr FACE_**** props
#define NAF         99999.9

//---------------------------------------------------------------------------------------
// Polygon structure
typedef struct _Plg_Poly
{
	size_t		flags;			        // polygon flags (see above values)
	size_t		user;			        // polygon flags (see above values)
	CLR 		color;		            // polygon color across al vertexes (0-255)
    int         texIdx[3];              // texture indexes for 3 stages(in the scene textures)
    size_t       texapply[3];            // how to apply each/all textures/ or (gen tc) on this poly (see irender.h)
    size_t       combine;                // how to set up register combiners
    V3          texax[2];               // texture axis         (use them to save q and ut map file)
    UV          texShift;               // shift on u and v     (use them to save q and ut map file)
    UV          texScale;               // scale on u and v     (use them to save q and ut map file)
    Vtx*        vXes;			        // polygon vertexes.    (has all proper texture coordinates)
	int			nvXes;			        // number of vertexes   
}Plg_Poly;

// brushes flags are as in the bspfilestr.h
//---------------------------------------------------------------------------------------
// brush structure
typedef struct _Plg_Brush
{
	size_t		flags;			        // see above flags
	Plg_Poly*   pPolys;		            // brush polygons
	int			nPolys;			        // number of polygons
	char		name[64];		        // name of the brush
}Plg_Brush;

//-----------------------------------------------------------------------------
// query flags for texture if a plugin asks the editor about this texture
#define WANT_TEX_BUFEER                 0x1
// Texture structure. 
typedef struct _Plg_Texture
{
	size_t       flags;                   // pass in engine what you want from this texture
	size_t		target;                  // creation flags, 1D, 2D, 3D, CUBE (see irender.h)
    char        filename[64];            // filename to load. absolute path or relative to 
                                         // current directory from where the file was imported
                                         // filename has to have proper extension 
                                         // a file as 'GOTHIC/METAL_GRILL' as seen in q maps
                                         // will not be loaded. use instead 'GOTHIC/METAL_GRILL.JPG'
    int         dims[2];                 // texture dimensions
    int         bpp;                     // texture bpp
    BYTE*       pBuffer;                 // texture Buffer (RGB triplets)   
}Plg_Texture;

//-----------------------------------------------------------------------------
// scene structure
#define		CALC_TEXCOORD1	    1       // after importing texture coordinates for stage 1 are recalculated      
//                                         ALWAYS TEXTURE COORDINATES FOR LIGHT MAPS (STAGE 2) ARE RECALCULATED 
#define		CALC_TEXCOORD3	    3       // after importing texture coordinates for stage 2 are recalculated      
#define		CALC_TEXCOORD4	    4       // after importing texture coordinates for stage 3 are recalculated      
/*
    Internal texture coordinates are calculated as:
    U = XYZ*TEXAX(U)/SCALEX*BMP_WIDTH  + TEXOFFSET_U/TEXSHIFT_U;
    V = XYZ*TEXAX(V)/SCALEY*BMP_HEIGHT + TEXOFFSET_V/TEXSHIFT_V;
*/

typedef struct _Plg_Scene
{
	size_t		    flags;              // scene flags (not used)
	Plg_Brush*	    pBrushes;           // pointer to the brushes
	int			    nBrushes;           // number of brushes    
    Plg_Texture*    pTextures;          // pointer to textures
    int			    nTextures;          // numbr of textures
    Plg_Item*       pItems;
    int             nItems;
}Plg_Scene;

//------------------------------------------------------------------------------
class IGeticEditor
{
public:
    virtual long  GetTextureInfo(_Plg_Texture* pt)     = 0;

    
};

//------------------------------------------------------------------------------
#define 	PLUG_IMPORTER       1       
#define 	PLUG_EXPORTER       2       
class IGeticPlug 
{
public:
	virtual long  ImportFile(IGeticEditor* pEditor, char* ,char* bsFileName, Plg_Scene** ppScene)       = 0;
	virtual long  GetMenuStringAndType(char* bsFileName,size_t* type)      = 0;
	virtual long  ReleaseScene(Plg_Scene* ppBrush)                         = 0;
	virtual long  ExportFile(IGeticEditor* pEditor, char* , char* bsFileName, const Plg_Scene* ppScene)  = 0;
	virtual long  GetVersion()                                             = 0;
};

// Yuu  can create all structures using this function to get them properly initialized
// or if you use another heap allocation routine dont foget to init all members properly
/*
eg.
    Plg_Scene* pScene = New<Plg_Scene>();
    
    pScene->nBrushes  = 128;
    pScene->pBrushes  = New<Plg_Brush>(128);
    for(int i=0;i<128;i++)
    {
        pScene->pBrushes[i].nPolys = 6;
        pScene->pBrushes[i].pPolys = New<Plg_Poly>(6);
        // ...        
    }
    // ...
*/
#define RELEASE_SCENE(ps_)    if(ps_){                                  \
                                if(ps_->pItems)                          \
                                    delete[] ps_->pItems;               \
                                if(ps_->pTextures)                      \
                                    delete[] ps_->pTextures;            \
		                        for(int i=0; i< ps_->nBrushes;i++){     \
                                    Plg_Brush& b = ps_->pBrushes[i];    \
			                        for(int j=0; j< b.nPolys;j++){      \
                                        Plg_Poly& p = b.pPolys[j];      \
                                        delete[] p.vXes;                \
			                        }                                   \
			                        delete[]b.pPolys;                   \
		                        }                                       \
                                delete[] ps_->pBrushes;                 \
                                delete ps_;                             \
                            }                                           


#endif 
