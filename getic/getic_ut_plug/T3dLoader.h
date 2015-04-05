#ifndef _T3DLOADER_H_
#define _T3DLOADER_H_

#include "Common/Tokenizer.h"
#include "Common/Stack.h"

#define PF_Invisible        1
#define PF_Masked           2
#define PF_Translucent      4
#define PF_NotSolid         8
#define PF_Environment      16
#define PF_ForceViewZone    16
#define PF_Semisolid        32
#define PF_Modulated        64
#define PF_FakeBackdrop     128
#define PF_TwoSided         256
#define PF_AutoUPan         512
#define PF_AutoVPan         1024
#define PF_NoSmooth         2048
#define PF_BigWavy          4096
#define PF_SpecialPoly      4096
#define PF_SmallWavy        8192
#define PF_Flat             16384
#define PF_LowShadowDetail  32768
#define PF_NoMerge          65536
#define PF_CloudWavy        131072
#define PF_DirtyShadows     262144
#define PF_BrightCorners    524288
#define PF_SpecialLit       1048576
#define PF_Gouraud          2097152
#define PF_NoBoundRejection 2097152
#define PF_Unlit            4194304
#define PF_HighShadowDetail 8388608
#define PF_Portal           67108864
#define PF_Mirrored         134217728

#define PF_NoOcclude (PF_Masked | PF_Translucent | PF_Invisible | PF_Modulated)
#define PF_NoShadows (PF_Unlit | PF_Invisible | PF_Environment | PF_FakeBackdrop)

enum STATE {
	NONE,
	MAP,
	BRUSH,
	POLYGON,
	POLYGONLIST,
	ACTOR
};

/* ########################################################################################################### */

struct TexName {
    char *name;
    TextureID baseTexture;
    TextureID bumpMap;
    unsigned int sizeU, sizeV;
    unsigned int flags;
};

class TextureTable {
private:
    Set <TexName *> textureNames;
public:
    TextureTable(){}
    ~TextureTable(){
        TexName *texName;
        int i, len = textureNames.getSize();

        for (i = 0; i < len; i++){
            texName = textureNames[i];
            delete texName->name;
            delete texName;
        }
    }

    void addTextureName(char *name, unsigned int sizeU, unsigned int sizeV, TextureID baseTexture, TextureID bumpMap, unsigned int flags){
        TexName *texName = new TexName;

        texName->name        = new char[strlen(name) + 1];
        strcpy(texName->name, name);
        texName->sizeU       = sizeU;
        texName->sizeV       = sizeV;
        texName->baseTexture = baseTexture;
        texName->bumpMap     = bumpMap;
        texName->flags       = flags;
        textureNames.add(texName);        
    }

    bool getTexturesFromName(char *name, unsigned int &sizeU, unsigned int &sizeV, TextureID &baseTexture, TextureID &bumpMap, unsigned int &flags){
        TexName *texName;
        int i, len = textureNames.getSize();

        for (i = 0; i < len; i++){
            texName = textureNames[i];

            if (stricmp(name, texName->name) == 0){
                sizeU       = texName->sizeU;
                sizeV       = texName->sizeV;
                baseTexture = texName->baseTexture;
                bumpMap     = texName->bumpMap;
                flags       = texName->flags;
                return true;
            }
        }

        texName = textureNames[0];
        sizeU       = texName->sizeU;
        sizeV       = texName->sizeV;
        baseTexture = texName->baseTexture;
        bumpMap     = texName->bumpMap;
        flags       = texName->flags;
        return false;
    }
        
};

/* ----------------------------------- */

class T3dLoader {
private:
	Tokenizer tok;

	Vertex fix(Vertex &v){
//		return Vertex(v.x, -v.z, -v.y);
		return Vertex(v.x, v.z, -v.y);
	}

	Vertex readVertex(){
		char str[256];
		float x,y,z;
		
		tok.next(str);  // sign
		x = (*str == '+')? 1.0f : -1.0f;
		tok.next(str);  // number
		x *= (float) atof(str);
		
		tok.next(NULL); // ,
		tok.next(str);  // sign
		y = (*str == '+')? 1.0f : -1.0f;
		tok.next(str);  // number
		y *= (float) atof(str);
		
		tok.next(NULL); // ,
		tok.next(str);  // sign
		z = (*str == '+')? 1.0f : -1.0f;
		tok.next(str);  // number
		z *= (float) atof(str);

		return Vertex(x,y,z);
	}
public:
	T3dLoader();
	~T3dLoader();

	bool loadFromFile(char *fileName, Set <class Polygon *> &polygons, TextureTable &textureTable);

};

/* ------------------------------------------------------------------- */


T3dLoader::T3dLoader(){

}

T3dLoader::~T3dLoader(){

}

bool T3dLoader::loadFromFile(char *fileName, Set <class Polygon *> &polygons, TextureTable &textureTable){
	char str[256];
	Stack <STATE> states;
	STATE state = NONE;
	int i,len;

	// Polygon variables
	class Polygon *poly;
	//class Polygon *poly;
	Set <Vertex> vertices;
	unsigned int flags, pflags;
	Vertex normal;
	Vertex origo, textureU, textureV;
    float panU, panV;
    TextureID baseTexture, bumpMap;
    unsigned int sizeU, sizeV;

	// Actor variables
	bool usePolygons = true;
	Vertex location(0,0,0);
	int vertexComponent;




	tok.setFile(fileName);

	while (tok.next(str)){
		if (stricmp(str, "Begin") == 0){
			tok.next(str);
			states.push(state);

			if (stricmp(str, "Map") == 0){
				state = MAP;
			} else if (stricmp(str, "Polygon") == 0){
				state = POLYGON;
				if (usePolygons){
					vertices.clear();
					flags = pflags = 0;
                    panU = panV = 0.0f;
				}
			} else if (stricmp(str, "Actor") == 0){
				state = ACTOR;
				location = Vertex(0,0,0);
				usePolygons = true;
			} else {
				state = NONE;
			}
		} else if (stricmp(str, "End") == 0){
			tok.next(NULL);

			switch(state){
			case POLYGON:
				if (usePolygons){
					if ((flags & PF_Invisible) == 0){
						len = vertices.getSize();
						poly = new class Polygon(len);
						for (i = 0; i < len; i++){
							//poly->setVertex(i, vertices[i]);
                            poly->setVertex(len - i - 1, vertices[i]);
						}
						if (flags & PF_TwoSided){
							poly->setFlags(PF_DOUBLESIDED, true);
						}
						if (flags & PF_Unlit){
							poly->setFlags(PF_UNLIT, true);
						}
						if (flags & PF_Translucent){
							poly->setFlags(PF_TRANSLUCENT, true);
						}

                        /*if (flags & PF_SpecialPoly){
                            poly->setFlags(PF_FOGVOLUME | PF_NONBLOCKING, true);
                        }*/

                        

                        poly->setFlags(pflags, true);

                        //origo -= (panU * (textureU / length(textureU)) + panV * (textureV / length(textureV)));
                        //origo += ((-panU - 2) * textureU * 2 + panV * textureV * 2);

                        origo -= (panU * textureU) / lengthSqr(textureU);
                        origo -= (panV * textureV) / lengthSqr(textureV);

                        textureU = fix(textureU);
                        textureV = fix(textureV);

						poly->setTexCoordSystem(fix(origo), textureU / (float) sizeU, textureV / (float) sizeV);
                        poly->setTextures(baseTexture,bumpMap);
						poly->finalize();
						//poly->setNormal(normal);
						polygons.add(poly);

					}
				}
				break;
			}

			state = states.pop();
		} else {


			switch(state){
			case POLYGON:
				if (usePolygons){
					if (stricmp(str, "Flags") == 0){
						tok.next(NULL); // =
						tok.next(str);  // flags
						flags = atoi(str);
												
					} else if ((flags & PF_Invisible) == 0){
						if (stricmp(str, "Vertex") == 0){
							Vertex v = readVertex();
							vertices.add(fix(v + location));
						} else if (stricmp(str, "Normal") == 0){
							normal = fix(readVertex());
						} else if (stricmp(str, "Texture") == 0){
    						tok.next(NULL); // =
	    					tok.next(str);  // texture name
                            textureTable.getTexturesFromName(str, sizeU, sizeV, baseTexture, bumpMap, pflags);
						} else if (stricmp(str, "TextureU") == 0){
							textureU = readVertex();
						} else if (stricmp(str, "TextureV") == 0){
							textureV = readVertex();
						} else if (stricmp(str, "Origin") == 0){
							origo = readVertex();
						} else if (stricmp(str, "Pan") == 0){
    						tok.next(NULL); // U
    						tok.next(NULL); // =
	    					tok.next(str);  // U pan or -
                            if (str[0] == '-'){
    	    					tok.next(str);  // U pan
		    				    panU = -(float) atof(str);
                            } else panU = (float) atof(str);

    						tok.next(NULL); // V
    						tok.next(NULL); // =
	    					tok.next(str);  // V pan or -
                            if (str[0] == '-'){
    	    					tok.next(str);  // V pan
		    				    panV = -(float) atof(str);
                            } else panV = (float) atof(str);
						}
					}
				}
				break;
			case ACTOR:
				if (stricmp(str, "Group") == 0){
					usePolygons = false;
				} else if (stricmp(str, "Location") == 0){
					float value;
					tok.next(NULL); // =
					tok.next(NULL); // (
					do {
						tok.next(str);
						vertexComponent = (*str - 'X');
						tok.next(NULL); // =
						tok.next(str);  // sign/value
						if (*str == '-'){
							tok.next(str);  // number
							value = -(float) atof(str);
						} else {
							value = (float) atof(str);
						}
						location[vertexComponent] = value;

						tok.next(str);
					} while (*str == ',');
				}
				break;
			}
		}
	}
	
	return true;
}

/* ########################################################################################################### */

#endif // _T3DLOADER_H_
