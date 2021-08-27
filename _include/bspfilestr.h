/**
FILE:       BSPFILESTR.H
AUTHOR:     MARIUS CHINCISAN
DESC:       DEFINITIONS FOR THE BSP FILE STRUCTURES.
HISTORY:    
-NOV 15 2004 NAILED TO VERSION 1, (4 TEXTURE STAGES)
-AUG 15 2005 VERSION 2 OF THE BSP FILE
-MAY 10 2006 VERSION 200. BIG TERRAIN AND ALL STRUCTURES ARE NAILED FROM THIS VERSION ON.
 ALL INCOMING VERSIONS WILL BE COMPATIBLE WITH THIS VERSION
*/

//---------------------------------------------------------------------------------------
#ifndef __BSPFILESTR_H__
#define __BSPFILESTR_H__

//---------------------------------------------------------------------------------------
#include "baselib.h"           // Basic math types

//---------------------------------------------------------------------------------------
#ifdef PROP_PATCHES
    #define BSP_VERSION1        201        //  constant - lighmap and (usedsize << 16 | bitmapsize)
                                           // This version stores in BSP file only lit pels of the light map
                                           // and not the whole lightmap image
#else
     #define BSP_VERSION1     200             //  terrain & associations
#endif //
                                            

//---------------------------------------------------------------------------------------
#define MAX_IMG_SZ          (1024*1024*4)   // maxim image size RGB
#define MAX_SLEN            32              // maximum length of any string/filename

//---------------------------------------------------------------------------------------
#define NODE_EMPTY          0x0             // convex leaf
#define NODE_SOLID          0x1             // solid leaf
#define LEAF_HAS_OCCLUDERS  0x2000          // leaf has at least a polygon being occluder
#define LEAF_DELETED        0x2000          // internal flag
#define LEAF_MERGED         0x4000          // internal flag
#define NODE_LEAF           0x80000000      // the node is a leaf
#define N_FRONT             1               // front index
#define N_BACK              0               // back index

//---------------------------------------------------------------------------------------
// POINT/SEGMENT/VOLUME TO A PLANE
typedef enum _REL_POS
{
    ON_PLANE = 0,               
    ON_FRONT,                   
    ON_BACK,                    
    ON_SPLIT                    
}REL_POS;

//---------------------------------------------------------------------------------------
//                                  INTERNAL FLAGS
#define BRSH_CUT            0x0     
#define BRSH_SOLID          0x1     
#define BRSH_REV            0x2     
#define BRSH_DETAIL         0x4     
#define BRSH_DIRTY          0x8     
#define BRSH_SELBOX         0x10    
#define BRSH_SELVIS         0x20    
#define BRSH_NEW            0x40    
#define BRSH_CUTALL         0x80    
#define BRSH_SKYDOM         0x100   
#define BRSH_HASTRIG        0x200   
#define BRSH_BIGTERRAIN     0x400
#define BRSH_CSGTERRAIN     0x1000  
#define BRSH_ROTATES        0x2000  
#define BRSH_MAINBSP        0x4000  
#define BRSH_DETACHED       0x8000 //new   
//GAP
#define BRSH_DONTBSP        0x10000 
#define BRSH_TRIANGLES      0x20000 
//GAP
#define BRSH_DISABLED       0x80000 
#define BRSH_FLAGGED        0x100000
#define BRSH_UNCUT			0x200000
//-GAP
#define BRSH_ZONE          0x800000
#define BRSH_SPLITTER      0x1000000
#define BRSH_PROJECTOR     0x2000000
#define BRSH_REF           0x40000000
#define BRSH_WIREBOX       0x80000000
#define IS_CUT_BRUSH       0x800000000     // mask for Btf_poly::brIdx if original brush 
                                           // is cut brush
//---------------------------------------------------------------------------------------
//                          HEIGHT MAP TERRAIN FLAGS
#define TERR_UTT0         0x1000       // untiled tex stage 0
#define TERR_UTT1         0x2000       // untiled tex stage 1
#define TERR_UTT2         0x4000       // untiled tex stage 2
#define TERR_UTT3         0x8000       // untiled tex stage 3
#define TERR_UTT4         0x10000      // unused 
#define TERR_UTT5         0x20000      // unused  
#define TERR_UTT6         0x40000      // unused  
#define TERR_UTT7         0x80000      // unused  

#define TERR_GOR          0x100000      // terrain is gouraoud lighted 
#define TERR_FULLBRGHT    0x200000      // terrain is full bright         
#define TERR_NOLIGHT      0x400000      // terrain has no lighhting shadows or maps
#define TERR_CUSTLM       0x800000      // second pass texture is a texture
#define TERR_NOCOLIDE     0x1000000     // terrain does not collide
#define TERR_ISWATER      0x2000000     // ????
// anim bits              0XF0000000 
#define TERR_TA_SHIFT     0x10000000     // texture animation by shifting
#define TERR_TA_OSC       0x20000000     // texture animation by spinning
#define TERR_TA_OSC2      0x30000000     // texture animation by spinning

//---------------------------------------------------------------------------------------
// MATERIAL FLAGS. (DETAIL BRUSHES, LEAF CONTENT, OR BRUSHES SURFACES)
// 
#define MODEL_MAT_NOTOCCUP  0x01        // cannot go in (solid by definition)
#define MODEL_MAT_OPAQUE    0x02        // material opacity BIT

#define MODEL_MAT_AIR       0x00        // gas, air 
#define MODEL_MAT_SOLID     0x03        // bits indicating the solid state of a material
#define MODEL_MAT_WATER     0x10        // water. (solid bits 0 1nd 1 anre not set)
#define MODEL_MAT_LAVA      0x22        // lava
#define MODEL_MAT_BRICK     0x33        // brick
#define MODEL_MAT_WOOD      0x43
#define MODEL_MAT_METAL     0x53
#define MODEL_MAT_ICE       0x61
#define MODEL_MAT_GLASS     0x71
#define MODEL_MAT_DIRT      0x83
#define MODEL_MAT_GELA      0x91        //
#define MODEL_MAT_PLASMA    0xA0        
#define MODEL_MAT_GAS       0xB0
#define MODEL_MAT_FOG       0xC0        // model is fog 

//-GAP
//-GAP
#define MODEL_CONTENT_MASK  0xFF       // mask to getting the content
#define GET_CONTENT(flg_)   (flg_ & MODEL_CONTENT_MASK)

//---------------------------------------------------------------------------------------
// MODEL PROPERTIES (EACH BSP - MINI BSP PROPERTIES)
#define MODEL_IS_MAINBSP    0x100           // model is the main BSP (only 1)
#define MODEL_IS_DETAIL     0x200           // model is detail (max 8912)
#define MODEL_IS_SKYDOM     0x400           // model is skydom model (omly 1)
#define MODEL_IS_TERRAIN    0x800           // model is carving terrain (detail)
#define MODEL_IS_FLAT       0x1000          // model is flat (BSP has only root node )
#define MODEL_IS_SPHBB      0x2000          // modesl is a billboard (spheric bilboard)    
#define MODEL_IS_CYLBB      0x4000          // cylindric billboard    
                                            
//-GAP
//-GAP
#define MODEL_VIS_TERRAIN   0x8000          // model is detail and visibility flag
                                            // is to be set by terrain visibility cell
#define MODEL_HIDDEN        0x10000         // model is allways hidden
#define MODEL_DYNAMIC       0x20000         // model has a motion path attached to it
#define MODEL_NOCOLIDE      0x40000         // for details only (dot collide with)

#define MODEL_STAIRS        0x100000        
#define MODEL_LADDER        0x200000
#define MODEL_GRAVITON      0x400000

//-GAP
#define MODEL_TRIANGLES     0x800000        // All model faces are triangles. 
#define MODEL_BB_CYL        0x1000000       // use for collision as cilynder
#define MODEL_BB_SPH        0x2000000       // use for collision as sphere
#define MODEL_BB_BB         0x4000000       // use for collision as bounding box
//-GAP

// Combined flags
#define MODEL_BSP_DEFAULT   (MODEL_BSP_LEAFY|MODEL_BSP_FRONT)   
#define MODEL_BSP_TERRAIN   (MODEL_BSP_FRONT)                   

//---------------------------------------------------------------------------------------
// MAIN BSP LEAF CONTENT
#define CONT_SOLID          MODEL_MAT_SOLID         
#define CONT_AIR            MODEL_MAT_AIR
#define CONT_WATER          MODEL_MAT_WATER
#define CONT_LAVA           MODEL_MAT_LAVA
#define CONT_BRICK          MODEL_MAT_BRICK
#define CONT_WOOD           MODEL_MAT_WOOD
#define CONT_METAL          MODEL_MAT_METAL
#define CONT_ICE            MODEL_MAT_ICE
#define CONT_GLASS          MODEL_MAT_GLASS
#define CONT_DIRT           MODEL_MAT_DIRT
#define CONT_GELA           MODEL_MAT_GELA
#define CONT_PLASMA         MODEL_MAT_PLASMA
#define CONT_GAS            MODEL_MAT_GAS

//---------------------------------------------------------------------------------------
// FACE PROPS CHANGE IN SHYNC WITH LISTOBOX INDEX
#define FACE_TRANSPARENT     0x1        // alpha is different then (255)
#define FACE_MIRROR          0x2         // needs special rendering (mirror rendering)
#define FACE_SHOWBACK        0x4         // face rendering disables back culling 
#define FACE_NOLIGTMAP       0x8        // face has no light map

#define FACE_GSHADE          0x10       // Gouraound shading
#define FACE_FULLBRIGHT      0x20        // face is radiating (see Shiness, and so on...)
#define FACE_HASBUMP         0x40        // face is radiating (see Shiness, and so on...)
#define FACE_HASHALLO        0x80        // the engine builds a hallo on normal direction on face edges

#define FACE_BLACKMASK       0x100       // the texture on this face has to be build with alpha chanell
#define FACE_PROJTEX         0x200       // face is projecting it's texture away
#define FACE_OCCLUDER		 0x400       // occluder
#define FACE_NOCOLLIDE       0x800         // don't do collision with this face

#define FACE_SPLITTER        0x1000       // don't do collision with this face
#define FACE_LM_PATCH		 0x2000
#define FACE_NO_TEXTURE		 0x4000
#define FACE_CUSTLM          0x8000       // second pass texture is set by user


// in combobox atached to flags 2---------------------------------------------------------
// HIWORD ANIMATION TEXTURES
#define FACE_TAMASK          0xFFFF0000
#define FACE_TA_STARTBIT     0x10000   
 
#define FACE_TA_SHIFT        0x10000     // texture animation by shifting
#define FACE_TA_SPIN         0x20000     // -"- by spinning
#define FACE_TA_RAD          0x40000     // -"- by expanding
#define FACE_TA_WAVY         0x80000    // wave the tex coordinates (you have to came out with the rendering)

#define FACE_TA_WINDY        0x100000    // wind the tex coordinates (you have to came out with the rendering)
#define FACE_TA_FRAMING      0x200000    // frame the texture (you have to came out with the rendering)
#define FACE_TG_RANDOM       0x400000    // frame the texture (you have to came out with the rendering)
#define FACE_TG_FIRE         0x800000   // runtime generated texture as fire (store props in userVals)

#define FACE_TG_BLOB         0x1000000   // runtime generated texture as blob (store props in userVals)
#define FACE_TG_PLASMA       0x2000000   // runtime generated texture as plasma(storeprops in userVals)
#define FACE_TG_WATER        0x4000000   // runtime generated texture as water(store props in userVals)
#define FACE_TG_LAVA         0x8000000  // runtime generated texture as lava(store props in userVals)

#define FACE_TG_ENVCUBE      0x10000000  // runtime generated texture as lava(store props in userVals)
#define FACE_TG_ENVSHPERE    0x20000000  // runtime generated texture as lava(store props in userVals)
#define FACE_TG_ENVCUBE2     0x40000000  // runtime generated texture as lava(store props in userVals)
#define FACE_TG_ENVSHPERE2   0x80000000  // runtime generated texture as lava(store props in userVals)

// GAP


//--------------------------------------------------------------------------------------
// SOURCE LIGHT FLAGS 
#define LIGHT_SURFACE   0x8         
#define LIGHT_STEADY    0x1000      
#define LIGHT_HASHALLO  0x2000      
#define LIGHT_HASDYNA   0x4000      
#define LIGHT_DY_PULSE  0x8000      
#define LIGHT_DY_RAND   0x10000     
#define LIGHT_DY_SCIPT  0x20000     
#define LIGHT_DY_WAVY   0x40000     
#define LIGHT_DY_FLASHY 0x80000     
#define LIGHT_TEMPORAY  0x8000000  // introduced to calculate radiosity (editor specific)

//--------------------------------------------------------------------------------------
#define SND_AMBIENT_BITS   0xF0
#define SND_AMBIENT_SCENE  0x10
#define SND_AMBIENT_ZONE   0x20
#define SND_AMBIENT_LEAF   0x30


#define SND_COLLIDE         0x100
#define SND_REPEAT          0x200

//---------------------------------------------------------------------------------------
// Each Compiled Level have an unique GUID 
typedef struct _GGUID
{
    unsigned long   Data1;
    unsigned short  Data2;
    unsigned short  Data3;
    unsigned char   Data4[8];
} GGUID;

//---------------------------------------------------------------------------------------
// BSP SECTIONS ID'S
typedef enum _eSECT
{
    SECT_INITGAME = 1,
    SECT_SCENE,
    SECT_SKYDOM,
    SECT_TEXRGB,
    SECT_TEXINFO,
    SECT_LMINFO,
    SECT_LMRGB,
    SECT_SNDFILE,
    SECT_SNDINFO,
    SECT_ITEMCAT,
    SECT_ITEMS,
    SECT_ITEMSINFO,
    SECT_V3,
    SECT_PLANES,
    SECT_POLYS,
    SECT_NODES,
    SECT_LEAFS,
    SECT_MODELS,
    SECT_PORTALS,
    SECT_PVS,
    SECT_MOTION,
    SECT_SCRINFO,
    SECT_SCRDATA,
    SECT_MATERIALS,
    SECT_POLYPROPS,
    SECT_SHADERS,         // (not used yet)
    SECT_ZONES,          // visibility zones (see associated structure)
    SECT_TERRAIN,        // only one
    SECT_TERRAINGLIGHT,  // CLRNOA[]
    SECT_TERRAINNODES,   // BTF_Terrain_Node[] 
    SECT_TERRAINLEAFS,   // BTF_Terrain_Leaf[]
    SECT_DETAILS,        // associates a item/brush name with a variable data description BTF_Dets
    SECT_RANDOMBYTES,    // some sections store extra data here
    SECT_EOF=-1,
}eSECT;

//---------------------------------------------------------------------------------------
// EDITOR ITEMS ID'S
typedef enum _EITEM
{
    ITM_NONE = 0,
    ITM_LIGTBULB,   // editor   default
    ITM_TRIGER ,    // editor   default
    ITM_SNDSOURCE,  // not in items section -> 
    ITM_STARTPLAY,
    ITM_GAP51,
    ITM_GAP61,
    ITM_GAP71,
    ITM_GAP81,
    ITM_GAP91,
    ITM_GAP10,
    ITM_GAP11,
    ITM_GAP12,
    ITM_GAP13,
    ITM_GAP14,
    ITM_GAP15,
    ITM_GAP16,
    ITM_GAP17,
    ITM_GAP18,
    ITM_GAP19,
    ITM_GAP20,
    ITM_GAP21,
    ITM_GAP22,
    ITM_GAP23,
    ITM_GAP24,
    ITM_GAP25,
    ITM_GAP26,
    ITM_GAP27,
    ITM_GAP28,
    ITM_GAP29,
    ITM_VOID,
    ITM_GAME,   //user defined
}EITEM;

//---------------------------------------------------------------------------------------
// VARIANT 
typedef enum _VAR
{
    V_UNKNOWN=0,
    V_BYTE,         
    V_CHAR,
    V_TEXT,
    V_FILENAME,     // max MAX_SLEN in length
    V_INT,
    V_REAL,
    V_CLR,
    V_V3,
    V_UV,
    V_DOUBLE,
    V_VOID,         // not used.
    V_BUFFER        // MAX_SLEN in bytes
}VAR;

//---------------------------------------------------------------------------------------
// TEXTURE PACKET
typedef struct _TexPack
{
    unsigned short  textures[5]; 
    unsigned long   texApply[5]; 
    unsigned long   combine;     
}TexPack;

//---------------------------------------------------------------------------------------
typedef struct _BTF_Bytes
{
    unsigned long   length; // length in random
}BTF_Bytes; // follows the bytes blob of 'length'.

//---------------------------------------------------------------------------------------
// BSP ITEM STRUCTURE
typedef struct _BTF_LightBulb/*NO CATGEGORY*/             
{
    unsigned short  _specAngle;     
    unsigned short  _lmIntensity;   
    unsigned short  _radius;        
    unsigned short  _halloRadMin;   
    unsigned short  _halloRadMax;   
    unsigned short  _mimDist;       
    unsigned short  _maxDist;       
    unsigned char   __unused[4];    
    unsigned char   _colorD[4];     
    unsigned char   _colorE[4];     
    unsigned char   _colorS[4];     
    REAL            _fallOff;       
    REAL            _attenuation[3];
}BTF_LightBulb;

typedef struct _BTF_Trigger                        // TRIGGER
{
    unsigned short  _reloadtime;    
    long            _brushIndex;    
    unsigned long   _reserved;      
    unsigned long   _reserved2;      
}BTF_Trigger;

typedef struct _BTF_StartItem
{
    char    _team[32];
    char    _reserved[128];
}BTF_StartItem;

// CUSTOM GAME ITEM PROPERTIES DEFINED BY 'IDF' FILE
// CAN HOLD UP TO 32 TYPES _VAR-IANTS STREAMED ACROSS MAXIMUM 512 BYTES
typedef struct _BTF_GameItem
{
    unsigned char   _type[MAX_SLEN];    // 32 types V_*; ends with V_UNKNOWN
    unsigned char   _buff[512];         // max 32 values
}BTF_GameItem;

// INTERNALLY USED (SEE BSPVIEW SOURCE CODE FOR MORE INFO)
typedef struct _BTF_Void
{
    int     _type;                      // _VAR type
    void*   _pvoid;                     // pointer to type
}BTF_Void;


typedef struct _BTF_Item
{
    unsigned char   _type;               // Item type _EITEM
    V3              _pos;                
    V3              _dir;                
    Box             _box;                
    unsigned long   _flags;              
    char            _name[MAX_SLEN];     
    char            _catname[MAX_SLEN];  
    unsigned long   _areavis;            
    TexPack         _texp;
    unsigned long   _dummy[4];           
    
    union
    {
        BTF_LightBulb _ligtbulb;
        BTF_Trigger   _trigger;
        BTF_StartItem _startitem;
        BTF_GameItem  _gameitem;
        BTF_Void      _void;
    };
}BTF_Item;


//---------------------------------------------------------------------------------------
// BSP FILE HEADER. ANY GETIC BSP FILE STARTS WITH THIS STRUCTURE. 
typedef struct _BTF_Header              // has no section
{
    char            sig[5];             // "GGBT\0"
    unsigned long   version;            // editor version
    GGUID           uuid;               // unique id for this file
    char            comments[128];       // level comments
}BTF_Header;


//---------------------------------------------------------------------------------------
// BSP SECTION HEADER
typedef struct _BTF_Section
{
    char            _type;              // _eSECT
    unsigned long   _elems;             // number of elements
    unsigned long   _bytes;             // number of bytes
}BTF_Section;

//---------------------------------------------------------------------------------------
// SCENE PROPERTIES.
enum{LEV_QUEST, LEV_TEAM, LEV_MULTITEAM};
struct BST_SceneInfo
{
    char            author[MAX_SLEN];   // level author
    char            comments[256];      // name of the level
    unsigned char   levelType;          // level type (not implemented yet) see enum section above
    unsigned char   maxActors;          // maxim actors
    unsigned short  maxFrags;           // maxim frags
    unsigned long   maxTime;            // maxim time
    CLR             defLumin;           // ilumination
    unsigned long   gravAcc;            // gravitation acceleration
    V3              camOrient1;         // not used
    V3              camOrient2;         // not used
    REAL            camFar;             // optimum camera far plane for this level
    REAL            camFov;             // optimum camera fowfor this level 
                                        // (fov angle can be changed in editor bt 'L'+Lef+MouseMove 
                                        // (status bar shows current angle))
    REAL            sceSpeed;           // optimal maxim scene speed
    unsigned long   sceFlags;           // scene user flags
    int             scrNotUsed;         // <2
    REAL            fogNear;            // fogs properties
    REAL            fogFar;             //
    CLR             fogColor;           //
    REAL            fogDens;            //
    REAL            fogFallOff;         //
    int             sceneSpash;         // scene splash bitmap offset in sect bytes
    char            sceneLogoFile[32];  // scene splash image file
    unsigned char   lightMapAspect;     // 1 - scene lightmaps scalled to real aspect then if (next value)
    unsigned char   lightMappowof2;     // 1 - scene lightmaps tex should be created for closest up pow of 2
    char            dummy[94];
    GGUID	        gamekeyID;          // game ID 
};


//---------------------------------------------------------------------------------------
// SKYDOM STRUCTURE
typedef struct _BTF_SkyDom
{
    unsigned short   skyModelIdx;            // this model index
    unsigned short   subModels;              // number of integers that follow this
    char             dummy[MAX_SLEN];        // ! used    
}BTF_SkyDom;// follwed by integers pointing to detail skydom models

//---------------------------------------------------------------------------------------
// ZONE STRUCTURE
typedef struct _BTF_Zone
{
    Box            box;                 // zone bounding box        
    unsigned long  visData;             // zone visibility data
    unsigned long  reserved;            // reserved
}BTF_Zone;

//---------------------------------------------------------------------------------------
// RGB(A) TEXTURE.
typedef struct _BTF_TextureRGB                   // SECT_TEXRGB
{
    unsigned short  index;              // index of this
    unsigned long   sz;                 // size in bytes
    unsigned char   bpp;                // bits per pixel
    unsigned short  cx;                 // x in pixels
    unsigned short  cy;                 // y in pixels
    unsigned long   flags;              // not used (compresion & rgb data format)
    unsigned long   reserved;           // not used (compresion & rgb data format)
}BTF_TextureRGB;

//---------------------------------------------------------------------------------------
// Vertex structure
typedef struct _BTF_Vertex   
{
    int     vxIdx;      // index in main veterx pool
    UV      tc0;        // primary texture coordinates
    UV      tc1;        // second pass texture coordinates (light map)
    UV      tc2;        // third pass texture coordinates
    UV      tc3;        // fourth 
    CLR     clr;        // byte[4] rgba vertex color
}BTF_Vertex;


//---------------------------------------------------------------------------------------
// LIGHT MAPP INFORMATION
typedef struct _BTF_LmapInfo             // SECT_LMINFO
{
    unsigned long   index;      // offset in BYTES the Light map bytestream (SECT_LMRGB)
    char            bpp;        // bits per pixel of the lmap
    unsigned short  xsz;        // x dim in pixels for version BSP_VERSION1 = 201  (bmpsize<<8|uzedsz)
    unsigned short  ysz;        // y dim in pixels                                  (bmpsize<<8|uzedsz)
    unsigned long   flags;      // lm flags (not used)
}BTF_LmapInfo;


//---------------------------------------------------------------------------------------
typedef unsigned char* LMBYTES;     //  SECT_LMRGB
typedef struct _BTF_Buffer
{
    unsigned long   _size;          // size in bytes of the buffer that folows 
}BTF_Buffer;

//---------------------------------------------------------------------------------------
typedef struct _BTF_TextureInfo                  // SECT_TEXINFO
{
    unsigned long   creaFlags;          // texture target
    unsigned long   packFlags;          // (unused)
    char            texname[MAX_SLEN];  // texture file url-name 
}BTF_TextureInfo;

//---------------------------------------------------------------------------------------
// Script info.
typedef struct _BTF_ScrInfo                  // SECT_SCRINFO scripts
{
    unsigned long      flags;       // script flags / compression etc/ language/ interpreter
    char			   scrName[MAX_SLEN];      // url for script file. prev member is 0 then
}BTF_ScrInfo;

//---------------------------------------------------------------------------------------
// Sound is not used in this version. You may use user defined item to attach a sound to 
// the map
#define SND_MIDI    0x1
#define SND_WAV     0x2
#define SND_MPG     0x3
typedef struct _BTF_SndInfo                      // SECT_SNDINFO
{
    unsigned long   flags;              // sound, wav, mpeg midi, use bsp fo
    unsigned short  radius;             // max radius influence
    unsigned short  volume;             // default volume
    unsigned short  atten;              // attenuation
    long            reserved;           // reserved
    char            sndname[MAX_SLEN];  // url for sound file
}BTF_SndInfo;

typedef struct _BTF_SndFile                       // SECT_SNDFILE
{
    char            sndname[MAX_SLEN];  // file-name of the sound file
    unsigned long   length;             // length of sound data (type is in snd-info)
    unsigned long   flags;              // flags
    //follows snd data as wav ile or whatever file was used (if included).
}BTF_SndFile;


//---------------------------------------------------------------------------------------
// Reference model. (Not used yet). 
typedef struct _BTF_ModelInfoRef
{
    unsigned short  index;              // this model index
    REAL            tm[16];             // transform matrix 
    unsigned short  modelRef;           // original model index.
}BTF_ModelInfoRef;

//---------------------------------------------------------------------------------------
// Each BSP is refered by a model. 
typedef struct _BTF_ModelInfo
{
    unsigned long   index;              // this model index
    long            firstNode;          // first node in nodes array
    unsigned long   nodes;              // number of nodes this model has
    long            firstLeaf;          // first leaf of this model in leafs array
    unsigned long   leafs;              // number of leafs this model has
    long            firstPoly;          // first polygon in polys structure
    unsigned long   polys;              // number of polysgons this model has
    long            firstPortal;        // first portal in portal structure       (only main model)
    unsigned long   portals;            // number of portals see portal structure (only main model)
    unsigned long   pvs;                // number of pvs bytes                    (only main model)
    unsigned long   flags;              // content and so on
    short           unused;             // ???
    short           dynamicIdx;         // motion data
    REAL            userVals[8];        // rottation parameters for skydom
    REAL            tm[16];             // transform matrix (for virtual models only)
    unsigned long   areavis;            // <2 changed to area visibility bits (max 32 areas)
    char            name[MAX_SLEN];     // model name
    unsigned long   props;
    unsigned long   reserved[3];        // reserved    
}BTF_ModelInfo;

//---------------------------------------------------------------------------------------
// Plane structure
typedef struct _BTF_Plane
{
    V3      normal;
    REAL    dist;
}BTF_Plane;


//---------------------------------------------------------------------------------------
// BSP node structure
typedef struct _BTF_Node
{
    long            nodeidx[2]; // the 2 nodes: back & front
    long            idxParent;  // parent node index;
    long            planeIdx;   // plane index which this polygon is onto
    long            leafIdx;    // leaf index this poly is part of
    Box             bbox;       // node bounding box
    unsigned long   flags;      // flags (see flags for polygon Line: 162)
    long            cluster;    // used in next version 4
    unsigned long   areavis;    // <2 area visibility bits (max 32 areas)
}BTF_Node;

//---------------------------------------------------------------------------------------
// Leaf Structure. For BSP Engines.
typedef struct _BTF_Leaf
{
    unsigned long   flags;          // content and so on
    unsigned long   pvxIdx;         // index of PVS bits of this leaf
    long            nodeIdx;        // linked node to this leaf
    CLR             defLumin;       // default ambient light of this leaf
    unsigned short  firstPoly;      // first polygon in polygon array
    unsigned short  polys;          // no of polygons
    short           portals;        // portals
    short           cluster;        // cluster areea    
    short           firstItem;      // first item
    unsigned short  items;          // items
    unsigned short  models;         // models landing in this leaf
    unsigned long   areavis;        // <2 used in next version 4
    REAL            userVals[8];    // extra user vals (Unused)
}BTF_Leaf;

//---------------------------------------------------------------------------------------
// Portal Structure. For Portal Engines.
typedef struct _BTF_Portal
{
    unsigned long  planeidx;     // plane index
    unsigned long  bLeaf;        // back leaf
    unsigned long  fLeaf;        // front leaf
    unsigned long  vxes;         // vertex indexes of this portal polygon
    unsigned long  reserved;     // 
}BTF_Portal;

//---------------------------------------------------------------------------------------
typedef struct  _BTF_Shader
{
}BTF_Shader;


//---------------------------------------------------------------------------------------
// SECT_MATERIALS,
// not used yet
// Polygon will have an index to Material array, which will decrease the polygon structure
typedef struct  _BTF_Material             // MATERIAL 
{
    CLR             colorS;      // spec color MATERIAL
    CLR             colorD;      // diffuse color MATERIAL
    CLR             colorE;      // emmisive color MATERIAL
    int             nShader;     // shader Index   
    REAL            shiness;     // shiness
    REAL            bump;        // bump factor
    REAL            friction;    // friction factor
    CLR             radiosity;
    REAL            intensity;
    unsigned long   flags;       // flags
    unsigned long   reserve[8];
}BTF_Material;

//---------------------------------------------------------------------------------------
//  SECT_POLYPROPS,
typedef struct  _BTF_PolyProps   // incomming to version 4
{
    TexPack         _texp;
    unsigned long   flags;       // flags
    unsigned long   flags2;      // flags
    UV              tanimA;      // texture anim
    unsigned long   reserve[8];
}BTF_PolyProps;

//---------------------------------------------------------------------------------------
// (not used yet)
// Polygon structure.
typedef struct  _BTF_Poly2       
{
    long            planeIdx;    // plan index
    long            brIdx;       // original brush index (for rebuilding purposes)
    long            propIndex;   // properties index
    unsigned char   matIndex;    // material index               
    unsigned short  vxCount;     // vertex count
    V3              lmAxes[3];   // light map ax
}BTF_Poly2;


//---------------------------------------------------------------------------------------
// (not used yet)
// Polygon structure.
typedef struct  _BTF_PolyRef     
{
    long            polyIdx;     // all props as the ref
    unsigned short  vxCount;     // vertex count
}BTF_PolyRef;

//---------------------------------------------------------------------------------------
// Polygon structure.
typedef struct  _BTF_Poly                 // 
{
    unsigned long   planeIdx;    // plane index
    unsigned long   brIdx;       // original brush index (for rebuilding purposes)
    unsigned short  vxCount;     // vertex count
    V3              lmAxes[3];   // light map axes
    unsigned long   flags;       // flags
    unsigned long   flags2;      // extra flags
    UV              tanimA;      // texture animation
    CLR             colorS;      // specular color MATERIAL
    CLR             colorD;      // diffuse color MATERIAL
    CLR             colorE;      // emmisive color MATERIAL
    unsigned char   shiness;     // shiness
    unsigned char   bump;        // bump factor
    unsigned char   friction;    // friction factor
    TexPack         _texp;
    unsigned long   userVals[4]; // 
}BTF_Poly;

//---------------------------------------------------------------------------------------
#define M_OPEN      0               // motion path is openend
#define M_LOOP      1               // motion path is closed
typedef struct  _BTF_Motion
{
    unsigned long   modelIndex;     // model index this is linked with
    unsigned short  cntNodes;       // number of steps
    unsigned long   flags;          // flags
    long            reserved;       
}BTF_Motion;

//---------------------------------------------------------------------------------------
typedef struct _BTF_MotionNode               // status nodes along th path
{
    REAL            pauseTime;      // time to sit in this node
    REAL            trTime;         // Transition Time to next Node
    V3              vPos;           // World translation of this Node
    V3              vDir;           // World orientation
    unsigned long   flags;          // TBD flags for this node position
    long            reserved;
}BTF_MotionNode;

//---------------------------------------------------------------------------------------
// Item cathegories. Applys only when using user defined items.
// Holds maximum 32 members and maximum 512 byte object size for object definition. 
typedef struct _BTF_ItemCat          // SECT_ITEMCAT
{
    char            catName[MAX_SLEN];  // category name
    unsigned long   nDataLength;        // 'byData' length
    union {
        struct
        {
            unsigned char   cTypes[MAX_SLEN];   // category type (for type check)
            unsigned char   byData[956];       // category data
        }UserCat;
    };
}BTF_ItemCat;

//---------------------------------------------------------------------------------------
//   SECT_TERRAIN,   
typedef struct _BTF_Terrain_Tree
{
    unsigned long   flags;       // flags
    CLR             colorS;      // specular color MATERIAL
    CLR             colorD;      // diffuse color MATERIAL
    CLR             colorE;      // emmisive color MATERIAL
    unsigned char   shiness;     // shiness
    unsigned char   bump;        // bump factor
    unsigned char   friction;    // friction factor
    Box             box;
    unsigned short  tiles[2];
    unsigned short  heightMap;         // follows this header BYTE[heightMap]
    unsigned short  gouraud;           // REAL[heightMap], in (RGB)
    unsigned char   nodes;             // follows nodes BTF_Terrain_Node
    unsigned char   leafs;             // follows nodes BTF_Terrain_Node
    TexPack         _texp;
    UV              _tanim[2];        // 2 animations          
    unsigned long   _stage;
    unsigned long   unused[13];       // add and subtract
}BTF_Terrain_Tree; 

//---------------------------------------------------------------------------------------
typedef struct _BTF_Terrain_Node
{
    Box             bbox;
    char            lidx;
    unsigned char   dsplit;      // direction split 0 on x 1 on (z/y)
    char            inodes[2];   // if index is - is an index in leafs struct
    char            iparentNode;
    unsigned long   unused[16];
}BTF_Terrain_Node; 

//---------------------------------------------------------------------------------------
typedef struct _BTF_Terrain_Leaf
{
    unsigned char   tiles[4];     // start tile, end tile
    char            nidx;
    unsigned short  lightMapInfo;
    unsigned long   userVals[4];   // 
    unsigned long   xflags;      
    unsigned long   unused[16];      
}BTF_Terrain_Leaf; 


//---------------------------------------------------------------------------------------
//  SECT_DETAILS,   // associates a item/brush name with a variable data description BTF_Dets
typedef struct _BTF_Details
{
    char             name[MAX_SLEN];
    unsigned int     length;
    // char* // zerotermstring with description for name
}BTF_Details; 

//---------------------------------------------------------------------------------------
// NON BSP RELATED
// COLLISION TEST. A COLLIDING OBJECT MAY TEST FOR COLLISION ONE OF THESE FLAGS (INTERNALLY)
#define TEST_BSP            0x10000         // test the main bsp's
#define TEST_MODELS         0x20000         // test the models
#define TEST_SKYDOM         0x30000         // test the skydoms models    
#define TEST_FACES          0x40000         // faces
#define TEST_ITEMS          0x100000        // other items
#define TEST_TRIGS          0x200000        // trigers
#define TEST_ACTORS         0x400000        // obsolette
#define TEST_STAIRS         0x800000        // jump over stair test
#define TEST_ENV            (TEST_BSP|TEST_MODELS)  // colide with the world

//---------------------------------------------------------------------------------------
// NON BSP RELATED
#define CONT_GRAVITY        0x10000000      // check leaf extends
#define CONT_AMBLIGHT       0x20000000      // leaf has different ambient light than the scene
#define CONT_CANTGO         0x80000000      // cant'go in

//---------------------------------------------------------------------------------------
// NOT BSP RELATED
#define ON_SURFACE          0x1         // state of  a model/object
#define ON_AIR              0x2         // no touchings
#define ON_STUCK            0x4         // stuck

//---------------------------------------------------------------------------------------
// NOT BSP RELATED
#define ITM_POINT   0x0
#define ITM_SEG     0x1
#define ITM_SPHERE  0x2
#define ITM_BBOX    0x3
#define ITM_CYL     0x4

#endif //BSPFILESTR.H
