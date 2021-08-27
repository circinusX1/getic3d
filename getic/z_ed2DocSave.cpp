// z_ed2Doc.cpp : implementation of the CZ_ed2Doc class
//

#include "stdafx.h"
#include "Texref.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "mainfrm.h"
#include "Z_ed2View.h"
#include "Z_ed3View.h"
#include <algorithm>
#include "DlgNewBrush.h"
#include "LightBulb.h"
#include "DlgCpxPrimnames.h"
#include "TriggerItem.h"
#include "SoundItem.h"
#include "GameItem.h"
#include "versionmacro.h"


#define     CHECK_COUNT(c_,max_,_expsion)    if(c_>max_ ||c_<0)throw _expsion;
int         Version = 0;
//---------------------------------------------------------------------------------------
BOOL __FixBadName(char* pszname)
{
	char		invalids[] =_T("!@#$%^&^*()_+~|}{():\"\'.,<>?/-=\\][");
	char* pinvalid = invalids;
	while(*pinvalid)
	{
        StripChar(pszname,*(pinvalid++));
	}
	return 0;
}

//---------------------------------------------------------------------------------------
class CDocSaver : public FileWrap
{
public:
    CDocSaver(const char* szFile, BOOL bsave, BOOL pref=0):_pref(pref),_version(DOC_VERSION8),_filename(szFile),_bsave(bsave),_maxspliterIdx(0),_selection(0),_pTerBrush(0){}
    BOOL    Open(){
        return this->FileWrap::Open(_filename.c_str(),_bsave ? "wb" : "rb");
    }
    void    SerializeDoc(BOOL selection); 
    void    SerializeHeader();
    void    SerializeScene();
    void    SerializeSettings();
    void    SerializeCompSettings();
    void    SerializeTextures();
    void    SerializeBrushes();
    void    SerializeItems();
    void    SerializeMotions();
    void    SerializeScripts();
    void    SerializeCatFiles();
    void    SerializeTerrain();
    void    SerializeDescriptions();

    BOOL    IsPerf(){return _pref;}

    void SerializeBrush(Brush* pB);
    void SerializePolygon(Brush* pB, Poly* pp);
    SceItem* SerializeItem(SceItem* pi);

private:
    size_t            _version;
    int              _maxspliterIdx;   
    map<int,int>     _texassoc;
    vvector<Texture> _texNames;
    map<int,Brush*>  _dynaBrushes;
    BOOL             _bsave;
    tstring          _filename; 
    BOOL             _pref;
    BOOL             _selection;
    Brush*           _pTerBrush;   
};

//---------------------------------------------------------------------------------------
void CDocSaver::SerializeDoc(BOOL selection)
{
    REDIR();

    theApp._errors.clear();

    if(!this->FileWrap::Open(_filename.c_str(),  _bsave ? "wb": "rb"))
    {
        return;
    }

    CWaitCursor cwc;

    _pTerBrush = 0;
    _selection = (_filename.find(".grs") != -1);
    _pref      = (_filename.find(".gpm") != -1);
    
    SerializeHeader();

    if(_selection)
    {
        SerializeTextures();
        SerializeBrushes();
        return;
    }

    if(!_pref)
    {
        SerializeScene();
        SerializeSettings();
        SerializeCompSettings();
    }
    SerializeTextures();
    SerializeBrushes();
    SerializeTerrain();

    if(!_pref)
    {
        SerializeItems();
        SerializeMotions();
        SerializeScripts();
        SerializeCatFiles();
        SerializeDescriptions();

        if(!_bsave)
        {
            DOC()->_use2splitidx = tmax(_maxspliterIdx+1, DOC()->_use2splitidx);
        }
    }

    DOC()->RecreateAmbLmap();

    if(!theApp._errors.empty())
    {
        string all;
        int lines = 0;
        all = "Cannot Find: \r\n\r\n";
        FOREACH(vvector<string>, theApp._errors, pps)
        {
            all += *pps;
            all += "\r\n";
            if(++lines>32)
            {
                all += "...";
                break;
            }
        }
        theApp._errors.clear();
        AfxMessageBox(all.c_str(), MB_OK|MB_ICONWARNING);
    }
}
 
//----------------------------------------------------------------------------------------
void CDocSaver::SerializeHeader()
{
    char        szSig[5];

    if(_pref)
        ::strcpy(szSig, GeticPrimitiveFile);
    else
        ::strcpy(szSig, GeticGeometryRawFile);

    this->Serialize(szSig,4);
    if(!_bsave)
    {
        if(!strcmp(szSig,GeticPrimitiveFile))
        {
            _pref=1;
        }
        else
        if(strcmp(szSig,GeticGeometryRawFile))
        {
            throw "Invalid File Signature";
        }
    }
    this->Serialize(_version);   
    Version = _version;
    if(_version < DOC_VERSION4)
    {
        throw "Invalid File Version. ";
    }
}

//----------------------------------------------------------------------------------------
void    CDocSaver::SerializeScene()
{
	this->Serialize(SCENE()._si);
    if(!_bsave)
    {
        if(_version < DOC_VERSION5)
        {
            SCENE()._si.fogNear     = 0;
            SCENE()._si.fogFar      = 0;
            SCENE()._si.fogDens     = 0;
            SCENE()._si.fogColor    = ZWHITE;
        }
    }
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeSettings()
{
    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,0);

    this->Serialize(GCarving);
    this->Serialize(pTX->_zoom);
    this->Serialize(pTX->_shifty);
    this->Serialize(pTX->_shiftx);
    this->Serialize(pTY->_zoom);
    this->Serialize(pTY->_shifty);
    this->Serialize(pTY->_shiftx);
    this->Serialize(pTZ->_zoom);
    this->Serialize(pTZ->_shifty);
    this->Serialize(pTZ->_shiftx);
    this->Serialize(DOC()->_cam._pos);
    this->Serialize(DOC()->_cam._euler); 

}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeCompSettings()
{
    this->Serialize(COMP()->_bSaveFinalBSP);
    this->Serialize(COMP()->_szipit);
    this->Serialize(COMP()->_bSaveImages);
    this->Serialize(COMP()->_maxFacesLeaf);
    this->Serialize(COMP()->_bSaveCats);
    this->Serialize(COMP()->_bSavePortals);
    this->Serialize(COMP()->_bPolySortCrit);
    this->Serialize(COMP()->_bMergeNarrowLeafs);
    this->Serialize(COMP()->_bSaveSkyDom);
    this->Serialize(COMP()->_bLightMaps);
    this->Serialize(COMP()->_b3SampleLM);
    this->Serialize(COMP()->_bAvoidNonOrt);
    this->Serialize(COMP()->_bUseUserPriority);
    this->Serialize(COMP()->_bPvsMethod);
    this->Serialize(COMP()->_bZones);
    this->Serialize(COMP()->_bPvsMethod); // dup
    this->Serialize(COMP()->_removeOnePolyLeaf);
    this->Serialize(COMP()->_portalCutFlag);
    this->Serialize(COMP()->_balance);
    this->Serialize(COMP()->_lmapMaxSize);
    this->Serialize(COMP()->_lMapLumelSz);
    this->Serialize(COMP()->_bLmPatch);
    this->Serialize(COMP()->_bExportFile);
    if(_version >= DOC_VERSION5)
    {
        this->Serialize(COMP()->_bSaveBigTerrain);
        this->Serialize(COMP()->_bSaveBigTerrainBSP);
    }
    if(_version >= DOC_VERSION7)
    {
        this->Serialize(COMP()->_bConvexEarly);
        // additional data added
        this->Serialize(COMP()->_bCompressPvs);
        this->Serialize(COMP()->_bAABBTreeCsgTer);
        this->Serialize(COMP()->_lmpowof2);     
        this->Serialize(COMP()->_lmConst);   
        this->Serialize(COMP()->_bGeticBSP );       
        this->Serialize(COMP()->_bPolySortCrit); // dup
        this->Serialize(COMP()->_bConvexEarly);  //here next used var
        this->Serialize(COMP()->_bConvexEarly);
    }
    if(_version >= DOC_VERSION8)
    {
        int dummy;
        this->Serialize(COMP()->_lmapIntens);
        this->Serialize(dummy); // dummy
        this->Serialize(dummy);
        this->Serialize(dummy);
    }
}

//----------------------------------------------------------------------------------------    
void CDocSaver::SerializeTextures()
{
    int nCount = 0;
    if(_bsave)
    {
        //
        // count textures (excluding lightmaps). we dont save lmaps at all
        //

        TexSys::iterator bt = GTexSys.begin();
        TexSys::iterator et = GTexSys.end();
        for(;bt != et; bt++)
        {
            Texture& tex      = (*bt).second;
		    if(!strncmp(tex.filename, "@L",2))
			    continue;
		    ++nCount;
        }
     
        this->Write(nCount);

        int cntTex = 0;

        _texassoc[_dumptex.hTex]  =  -1; 
        bt = GTexSys.begin();
        for(;bt != et; bt++)
        {
            Texture& tex      = (*bt).second;
		    if(!strncmp(tex.filename, "@L",2))
            {
			    continue;
            }
            _texassoc[tex.hTex.hTex] = cntTex++;      // map index with texid
            this->Write(tex);
        }
        return;
    }
    
    Texture     tex ;

    this->Read(nCount);
    for(int i=0; i < nCount; i++)
    {
        CHECK_COUNT(nCount,65535,"To many textures: Limit is 65535");

        this->Read(tex);
		if(tex.cx==0 ||
           tex.cy==0)
			continue;
		if(tex.cx>=1024 ||
           tex.cy>=1024)
			continue;

        if(_version < DOC_VERSION5)
        {
            tex.hTex.glTarget= 0;
            tex.hTex.envMode = 0;
            tex.hTex.genST   = 0;
        }

        _texNames.push_back(tex);
    }
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeBrushes()
{
    if(_bsave)
    {
        PBrushes* brshses = 0 ;
        if(_selection)
            brshses = &DOC()->_pSelBrushes;
        else
            brshses = SCENE().GetPrimitives();

    	int	nBrushes = brshses->size();
	    this->Write(nBrushes);
        FOREACH(PBrushes, (*brshses), ppb)
	    {
            Brush* pB = (*ppb);
            pB->Recalc();
            SerializeBrush(pB);
        }
    }
    else
    {
        int	nBrushes;
        if(_pref)
            nBrushes=1;
        else
        {
           this->Read(nBrushes);
        }

        CHECK_COUNT(nBrushes,32768 ,"To many brushes: Limit is 32768");

        for(int ib=0; ib<nBrushes;ib++)
        {
		    Brush* pB = new Brush;
	        SerializeBrush(pB);

            DOC()->AddBrush(pB);

		    if(!pB->IsSolid() || pB->IsProjector())
            {
                DOC()->SelectBrush(pB);
            }
        }
        ++Brush::GID;
    }
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeBrush(Brush* pB)
{
    //hack
    int off = this->mOffset;
    
    this->Serialize(pB->_unicID   );
    this->Serialize(pB->_t        );
    this->Serialize(pB->_r        );
    this->Serialize(pB->_s        );
    this->Serialize(pB->_brushflags    );
    this->Serialize(pB->_brushprops    );
	this->Serialize(pB->_physicsA);
    this->Serialize(pB->_rotSteps);
    this->Serialize(pB->_rotSteps);
    this->Serialize(pB->_physicsD);
    this->Serialize(pB->_cf);              // creation flags
	this->Serialize(pB->_polySec);
    this->Serialize(pB->_thikness);
    off = this->mOffset;
    pB->_name[15] = 0; 
    this->Serialize(pB->_name, 16); 
    pB->_name[15] = 0;
    off = this->mOffset;
    this->Serialize(pB->_group    );
    
    off = this->mOffset;
    if(_bsave)
    {
        off = this->mOffset;
        int motionIdx = SCENE().GetIndex(pB->_pMotion);
        this->Write(motionIdx);

        int nCountPolys = pB->_polys.size();
        this->Write(nCountPolys);

        FOREACH(Polys, pB->_polys, pp)
        {
            SerializePolygon(pB, &(*pp));
        }
    }
    else
    {
        off = this->mOffset;
        pB->_group += DOC()->_maxUsedGrp;
        int motionIdx ;
        this->Read(motionIdx);
        if(motionIdx>=0)	// for redoing the pointer in the motion object
        {
            _dynaBrushes[pB->_unicID] = pB;	// store this brush here, process later
        }
        pB->_pMotion = 0;

        int     nCountVx ;

        this->Read(nCountVx );
        if(nCountVx==-1){
            AfxMessageBox("File was not saved as 'Save Model' Option", MB_OK|MB_ICONWARNING);
            return;
        }
        CHECK_COUNT(nCountVx,4096, MKSTR("To many polygons per brush %s: Limit is 4096",pB->_name));
        for(int vx=0; vx < nCountVx ; vx++)        
        {
            do{
                Poly	pl;
                SerializePolygon(pB, &pl);
                pB->_polys << pl;
                pl.Clear();
            }while(0);
        }
        if(pB->_brushflags & BRSH_BIGTERRAIN)
        {   
            _pTerBrush = pB;
        }
        pB->Dirty(1);
        pB->Recalc(1);
        Brush::GID = tmax(Brush::GID, pB->_unicID);
    }
    KEEP_WAIT(MKSTR("Processing Brush %d ",pB->_unicID));
}

//----------------------------------------------------------------------------------------

void CDocSaver::SerializePolygon(Brush* pB, Poly* pp)
{
    for(int k=0;k<4;k++)
    {
        if(_bsave)
        {
            map<int, int>::iterator fi = _texassoc.find((int)pp->GetHtex(k));
            if(fi != _texassoc.end())
            {
                int fisec = fi->second;
                this->Write(fisec);
            }
            else
            {
                int it = -1;
                this->Write(it);
            }
            this->Write(pp->Apply(k));
        }
        else
        {
            int texID, applyTex;
            this->Read(texID);
            this->Read(applyTex);

            if(texID >=0 && (size_t)texID < _texNames.size())
            {
                pp->SetApply(applyTex,k);
                pp->SetTex(_texNames[texID].filename, k, _texNames[texID].hTex.glTarget);

                if(TexHandler::GDefaultHtexture == pp->GetHtex(k))
                {
                    string serr = _texNames[texID].filename;
                    theApp._errors.push_unique(serr);
                }
            }
        }
    }
    size_t utempval = pp->Combine();
    this->Serialize(utempval);
    this->Serialize(pp->_polyflags);
    this->Serialize(pp->_polyflags2);
    this->Serialize(pp->_polyprops);
    this->Serialize(pp->_use2splitidx);
    this->Serialize(pp->_texcoord );
    this->Serialize(pp->_lmInfo);
    this->Serialize(pp->_colorD);
    this->Serialize(pp->_colorE);
    this->Serialize(pp->_colorS);
    this->Serialize(pp->_shiness);
    this->Serialize(pp->_tanimA);
    this->Serialize(pp->_tanimB);
    this->Serialize(pp->_texV1);
    this->Serialize(pp->_texV2);

    this->Serialize(pp->_bump);
    this->Serialize(pp->_friction);

    if(_bsave)
    {
        int  nCount = (*pp)._vtci.size();
		this->Write(nCount);
		FOREACH(vvector<Vtx2>, pp->_vtci, pv)
		{
			this->Write((*pv));
		}
    }
    else
    {
        _maxspliterIdx = tmax(_maxspliterIdx, pp->_use2splitidx);
        pp->_pBrush = pB;

        Vtx2 vtx2;
        int  nCount = 0;
        this->Read(nCount); 

        CHECK_COUNT(nCount,256, MKSTR("Too many vertexes per polygon: %s",pB->_name));
        
        for(int v=0; v < nCount; v++)
		{
    		this->Read(&vtx2,sizeof(Vtx2));
			pp->_vtci << vtx2;
        }
		pp->Recalc();
    }
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeItems()
{
    int nItemsCount;
    if(_bsave)
    {
        nItemsCount = DOC()->GetItems()->size();
        this->Serialize(nItemsCount);
        FOREACH(vvector<SceItem*>, (*DOC()->GetItems()), ppi)
        {
            SceItem* pi = *ppi;
            SerializeItem(pi);
        }
    }
    else
    {
         this->Serialize(nItemsCount);
         CHECK_COUNT(nItemsCount,2048,"Too many items per scene: Limit is 2048");

         for(int vx=0; vx < nItemsCount ; vx++)        
         {
             SceItem* pi = SerializeItem(0);

             SceItem::_GItemID = tmax(SceItem::_GItemID, (long)pi->_id);

             pi->_flags &= ~BRSH_NEW;
	         pi->Commit();
             SCENE().AddItem(pi);
             BARDLG()->AddItem(pi); 
         }
         ++SceItem::_GItemID;
    }
}

//----------------------------------------------------------------------------------------
SceItem* CDocSaver::SerializeItem(SceItem* pi)
{
    if(!pi) //load
    {
        int itemItem;        

        this->Read(itemItem);
        pi = SceItem::Create(itemItem);
    }
    else
    {
        this->Write(pi->_item);
    }
    pi->Serialize(this, _texNames, _texassoc);
    return pi;
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeMotions()
{
    if(_bsave)
    {
        int nCount = SCENE().GetMotions()->size();
        this->Write(nCount);

        FOREACH(vvector<CMotion*>,  (*SCENE().GetMotions()), ppMotion)
        {
            CMotion* pM = *ppMotion;

            this->Write(pM->_bLoop);
            this->Write(pM->_center);
            this->Write(pM->_pBrush->_unicID);
            
            int nCount = pM->GetNodesCount();
            this->Write(nCount);

            for(int i=0; i<nCount; i++)
            {
                MState* pS = pM->GetState(i);

                this->Write(pS->_pos); 
                this->Write(pS->_euler);
                this->Write(pS->_trTimes[0]);
                this->Write(pS->_trTimes[1]);
                this->Write(pS->_trRots[0]);
                this->Write(pS->_trRots[1]);
            }
        }
    }
    else
    {
        int nCount = 0,nIndex=0;
		this->Read(nCount);

        CHECK_COUNT(nCount,256,"Too many motions: Limit is 128");

		for (int i=0;i<nCount;i++)
		{
			CMotion* pM = new CMotion(0, 0, 0);

			this->Read(pM->_bLoop);
			this->Read(pM->_center);
			//
			// link them back
			//
			this->Read(nIndex);					// uuid of brush
			pM->_pBrush = _dynaBrushes[nIndex];	// take it from array
            if(pM->_pBrush)
            {
			    pM->_pBrush->_pMotion = pM;			// link back
            }
			this->Read(pM->_iNodes);
			for(int i=0; i < pM->_iNodes; i++)
			{
				MState  state;

				this->Read(state._pos);
				this->Read(state._euler);
				this->Read(state._trTimes[0]);
				this->Read(state._trTimes[1]);
				this->Read(state._trRots[0]);
				this->Read(state._trRots[1]);
				pM->_states << state;
			}
            if(pM->_pBrush)
			    SCENE().AddMotion(pM);
            else
                delete pM;

        }
    }
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeScripts()
{
    if(_bsave)
    {
        int tmpVal = 0;
	    int nCount = SCENE().GetScripts()->size();

	    this->Write(nCount);
	    FOREACH(vvector<ScriptItem*>,  (*SCENE().GetScripts()), ppScript)
	    {
		    ScriptItem*	pSi = *ppScript;
		    
		    tmpVal = pSi->_name.length();	
		    this->Write(tmpVal);	
		    this->Write((void*)pSi->_name.c_str(),pSi->_name.length());	
		    
		    tmpVal = pSi->_script.length();	
		    this->Write(tmpVal);	
		    this->Write((void*)pSi->_script.c_str(), pSi->_script.length());	
	    }
    }
    else
    {
        int     nCount = 0;
        int     nLength = 0;
        char   szBuff[PATH_MAX];
        char*   pszScr;    

        this->Read(nCount);
        CHECK_COUNT(nCount,512, "Too many scipt items: Limit is 256");
        for(int i=0;  i<nCount; i++)
        {
            ScriptItem* pSci = new ScriptItem();
            
            // script file name
            this->Read(nLength);	    
            memset(szBuff,0,sizeof(szBuff));
            this->Read(szBuff, nLength);	
            pSci->_name = szBuff;

            // script content
            this->Read(nLength);	        
            pszScr = new char[nLength];
            if(pszScr)
            {
                memset(pszScr,0,sizeof(nLength));
                this->Read(pszScr, nLength);	
                pSci->_script = pszScr;
                SCENE().AddScript(pSci);
                delete[] pszScr;
            }
            else
            {
                delete pSci;
            }
        }
    }
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeCatFiles()
{
    char sFileName[PATH_MAX] = {0};
    if(_bsave)
    {
	    if(theApp.XsdCatItmFile()[0])
		    strcpy(sFileName, theApp.XsdCatItmFile());
	    this->Write((void*)sFileName,PATH_MAX);
    }
    else
    {
		this->Read((void*)sFileName,PATH_MAX);
		if(sFileName[0])
		{
			if(SCENE().SearchCats((char*)sFileName))
			{
				theApp.XsdCatItmFile((char*)sFileName);
			}
		}
    }
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeTerrain()
{
    CBigTerrain& terr = SCENE().Terrain();

    if(_bsave)
    {
        terr.Serialize(this, _texNames, _texassoc);
    }
    else if(_version >= DOC_VERSION5) // load these sections
    {
        terr.Serialize(this, _texNames, _texassoc);
        terr.p_dummyBrush = _pTerBrush;
        if(_pTerBrush)
            _pTerBrush->_pUsrData = &terr;
    }
}

//----------------------------------------------------------------------------------------
void CDocSaver::SerializeDescriptions()
{
    BTF_Details             detcell;
    int                     ncount  = 0;
    map<string ,string>&    descs   = SCENE().Descriptions();

    if(_bsave)
    {
        ncount = descs.size();
        this->Write(ncount);
        for(map<string ,string>::iterator b = descs.begin(); b != descs.end(); b++)
        {
            SCopy(detcell.name, (char*)(*b).first.c_str(), sizeof(detcell.name));
            detcell.length = (*b).second.length();
            this->Write(detcell);
            this->WriteTextAsIs((*b).second.c_str());
        }
    }
    else if(_version >= DOC_VERSION5)// load these sections
    {
        char  tmp[16384];
        this->Read(ncount);
        CHECK_COUNT(ncount,2048, "Too many detail descriptors. Limit is 1024");
        for(int i=0; i < ncount; i++)
        {
            this->Read(detcell);
            this->Read(tmp, detcell.length);
            tmp[detcell.length] = 0;
            descs[detcell.name] = tmp;
        }
    }
}


//----------------------------------------------------------------------------------------
void      CZ_ed2Doc::SavePrefFile()
{
    
    REDIR();
    DlgCpxPrimnames d;
    if(d.DoModal() == IDOK)
    {
		REDIR();
        CDirChange  cdd(d._curPath.c_str());
        cdd.UseObj();      

        PBrushes* pBrushes = _scene.GetBrushes();
        
        FOREACH(PBrushes, (*pBrushes), ppCpxBrush)
        {
            do{
                Brush brush = *(*ppCpxBrush);
                brush.Recalc();

                V3  center = brush._box.GetCenter();
                brush.Move(-center);

                char locName[64];
                strcpy(locName, brush._name);
			    if(__FixBadName(locName))
				    continue;
            
                CDocSaver   ds(MKSTR("%s.gpm",locName), TRUE, TRUE);

                if(ds.Open())
                {
                    ds.SerializeHeader();
                    ds.SerializeTextures();
                    ds.SerializeBrush(&brush);
                    ds.Close();
                }

            }while(0);
        }
    }
    (VIEW3D(0,0))->SetFocus();
}

//----------------------------------------------------------------------------------------
BOOL CZ_ed2Doc::LoadPrefFile(const char* szFileName, Brush** pBRet)
{
	Brush*			  pB = 0;

    CDirChange  cdd("");
    cdd.UseObj();

    CDocSaver   ds(szFileName, FALSE); // load, pref

    try
    {
        if(ds.Open())
        {
            ds.SerializeHeader();
            if(ds.IsPerf())
            {
                ds.SerializeTextures();

                if(pB = new Brush())
                {
                    do{
                        CDirChange cdc("");
                        REDIR();
                        ds.SerializeBrush(pB);
                        pB->_unicID = Brush::GID++;
                    }while(0);

                    pB->_pMotion=0;

                    if(!(pB->_brushflags & BRSH_SOLID) && 
                       !pB->IsProjector())
                    {
                        SelectBrush(pB);
                        Recut();	
                    }
                }
            }
            ds.Close();
        }
    }
    catch(const char* ex)
    {
        AfxMessageBox(ex,MB_OK|MB_ICONHAND);
    }
    catch(...)
    {
        AfxMessageBox("File Is Malformated or file version is too old.",MB_OK|MB_ICONHAND);
        delete pB;
        pB = 0;
    }
    if(pBRet)
    {
        *pBRet = pB;
    }
    return pB!=0;
}

//----------------------------------------------------------------------------------------
void CZ_ed2Doc::OnFileOpen() 
{
    char szFilters[] = _T("Geometry raw (*.grf)|*.grf|"
                          "Geometry selection (*.grs)|*.grs|"  
                          "Geometry prefab (*.gpm)|*.gpm|"
                          "Getic BSP(*.gbt)|*.gbt||");  



	LPTSTR          lpszPathName = NULL;
    _closing        = FALSE;

	CFileDialog dlg(TRUE, _T("grf"), 0, OFN_HIDEREADONLY, szFilters);
	DeselectAll();

    //
    // Prompt the user for a filename
    //
	dlg.m_ofn.lpstrTitle = _T("Select  File");
    dlg.m_ofn.lpstrInitialDir = theApp.DocsDir();

	if (theApp.DoFileDialog(dlg) != IDOK) 
	{
		REDIR();
        return;
	}

    REDIR();
    PathHandler ph(dlg.m_ofn.lpstrFile);
	theApp.DocsDir(ph.Path());

	DeleteContents();
    InitDocVars();

    ////AfxMessageBox(MKSTR("%s%s_res",ph.Path(),ph.JustFile()));
    
    TexHandler::SetSearchPath(MKSTR("%s%s_res",ph.Path(),ph.JustFile()));    

    char szupper[PATH_MAX];
    ::strcpy(szupper, dlg.m_ofn.lpstrFile);
    CharUpper(szupper);
    if(_tcsstr(szupper,".GBT"))
    {
        ReadFromBSP(dlg.m_ofn.lpstrFile);
    }
    else
    {
        CopyResources(dlg.m_ofn.lpstrFile);
        ReadFromFile(dlg.m_ofn.lpstrFile);
    }

    SetModifiedFlag(TRUE);  
    SetTitle(dlg.m_ofn.lpstrFile);
    TestBackupFile();
    
    Invalidate(1);
    _docDirty=0;

}

//----------------------------------------------------------------------------------------
void CZ_ed2Doc::OnFileMerge() 
{

    char szFilters[] = _T("Geometry raw (*.grf)|*.grf|"
                          "Geometry selection (*.grs)|*.grs|"  
                          "Geometry prefab (*.gpm)|*.gpm||");  

	LPTSTR          lpszPathName = NULL;
    _closing        = FALSE;

	CFileDialog dlg(TRUE, _T("grf"), 0, OFN_HIDEREADONLY, szFilters);
	DeselectAll();
	// Prompt the user for a filename
	dlg.m_ofn.lpstrTitle = _T("Select  File");
	{
        dlg.m_ofn.lpstrInitialDir = theApp.DocsDir();

		if (theApp.DoFileDialog(dlg) != IDOK) 
		{
			REDIR();
            return;
		}
		REDIR();
        PathHandler ph(dlg.m_ofn.lpstrFile);
	    theApp.DocsDir(ph.Path());
	}
    ++DOC()->_maxUsedGrp;
     ReadFromFile(dlg.m_ofn.lpstrFile);
    SetModifiedFlag(TRUE);  
    AfxMessageBox(MKSTR("Merged Scene Is On Group: '%d' And Up", _maxUsedGrp), MB_OK|MB_ICONINFORMATION);
    	
}

//----------------------------------------------------------------------------------------
class NO_VT VRMLWritter
{
public:
    VRMLWritter(FileWrap* pfw):_pfw(pfw),_tabs(0){}
   	void Write(const char* p,...)
    {
		va_list args;
		va_start(args, p);
        ::_vsnprintf(_s, sizeof(_s) / sizeof(char), p, args);
		va_end(args);

        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");
        _pfw->WriteTextAsIs(_s);
        _pfw->WriteTextAsIs("\r\n");
	}

    void Tab(){
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");
    }
   	void WriteValNoCR(const char* p,...)
    {
        va_list args;
		va_start(args, p);
        ::_vsnprintf(_s, sizeof(_s) / sizeof(char), p, args);
		va_end(args);
        _pfw->WriteTextAsIs(_s);
	}

 	
    void WriteValue(const char* sect, char* p)
    {
      for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("%s\t",sect));
        _pfw->WriteTextAsIs(p);
        _pfw->WriteTextAsIs("\r\n");
    }

    void WriteValue(const char* sect)
    {
      for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("%s\r\n",sect));
    }


   	void WriteValue(const char* sect, UV& f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("%s\t",sect));
        _pfw->WriteTextAsIs(MKSTR("%f %f", f.u,f.v));
        _pfw->WriteTextAsIs("\r\n");
    }

    void WriteValue(const char* sect, CLR& f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("%s\t",sect));
        _pfw->WriteTextAsIs(MKSTR("%d ", f.r));
        _pfw->WriteTextAsIs(MKSTR("%d ", f.g));
        _pfw->WriteTextAsIs(MKSTR("%d ", f.b));
        _pfw->WriteTextAsIs(MKSTR("%d",  f.a));
        _pfw->WriteTextAsIs("\r\n");

    }
   	void WriteValue(const char* sect, V3& f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("%s\t",sect));
        _pfw->WriteTextAsIs(MKSTR("%f ", f.x));
        _pfw->WriteTextAsIs(MKSTR("%f ", f.y));
        _pfw->WriteTextAsIs(MKSTR("%f",  f.z));
        _pfw->WriteTextAsIs("\r\n");
    }


    void WriteValue(const char* sect, long f)
    {
        WriteValue(sect, (int) f);
    }
    void WriteValue(const char* sect, size_t f)
    {
        WriteValue(sect, (int) f);
    }

    void WriteValue(const char* sect, int f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");
        _pfw->WriteTextAsIs(MKSTR("%s\t",sect));
        _pfw->WriteTextAsIs(MKSTR("%d", f));
        _pfw->WriteTextAsIs("\r\n");
	}


    void WriteValue(const char* sect, float f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("%s\t",sect));
        _pfw->WriteTextAsIs(MKSTR("%f", (float)f));
        _pfw->WriteTextAsIs("\r\n");
	}


    void Open(char* p)
    {
        int i;
        for( i=0; i<_tabs; i++)
           _pfw->WriteTextAsIs("   ");
        _pfw->WriteTextAsIs(p); 
        _pfw->WriteTextAsIs("\r\n"); 
        for(i=0; i<_tabs; i++)
           _pfw->WriteTextAsIs("   ");
        _pfw->WriteTextAsIs("{\r\n");
        ++_tabs;
    }
    void Close()
    {
        --_tabs;
        _pfw->WriteTextAsIs("\r\n");
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");
        _pfw->WriteTextAsIs("}\r\n");
    }
private:
    char       _s[512];
    FileWrap*   _pfw;
    int         _tabs;
};


struct XmlFace{
    XmlFace():vxi(4),tc1i(4),tc2i(4),tc3i(4){}
    vvector<int>     vxi;
    vvector<int>     tc1i;
    vvector<int>     tc2i;
    vvector<int>     tc3i;
    int              clri[3]; //asde
    Poly*            pPoly;
};

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::SaveInXML(const char* pSzName, BOOL bBrshes)
{
    map<int,int>    texids;
    int             idx = 0;
	FileWrap        fw;
    int             fileVer   = DOC_VERSION5;
    int             cntTex=0;
    GUID            g = {0};
	

	CWaitCursor cwc;
	REDIR();
	BARDLG()->OnApply() ;
	fw.Open(pSzName,"wb");

    VRMLWritter  w(&fw);
    
    w.Open((char*)MKSTR("getic #%d",DOC_VERSION7));
    w.Open("level");
        if(_scene._si.sceneSpash>=0)
            w.WriteValue(MKSTR("splash '%s'",_scene._si.sceneSpash));        
        w.WriteValue("author", _scene._si.author);  
        w.WriteValue("comments",_scene._si.comments);     
        w.WriteValue("leveltype",_scene._si.levelType);         
        w.WriteValue("actors",_scene._si.maxActors);         
        w.WriteValue("fargs",_scene._si.maxFrags);          
        w.WriteValue("time",_scene._si.maxTime);           
    w.Close();
    w.Open("scene");
        w.WriteValue("coordsystem","left");          
        
        if(GMeters	== 1)
            w.WriteValue("measureunit", "centimeter radians");          
        else if(GMeters	== 100)
            w.WriteValue("measureunit", "meter radians");          
        else
            w.WriteValue("measureunit", "feet radians");     
        w.WriteValue("globallight",_scene._si.defLumin);          
        w.WriteValue("gravitation",-(int)_scene._si.gravAcc);           
        if(_scene._si.fogNear ||_scene._si.fogFar)
        {
            w.Open("fog");
                w.WriteValue("near",_scene._si.fogNear);           
                w.WriteValue("far",_scene._si.fogFar);
                w.WriteValue("color",_scene._si.fogColor);
                w.WriteValue("density",_scene._si.fogDens);
                w.WriteValue("falloff",_scene._si.fogFallOff);
            w.Close();
        }
        w.Open("camera");
            w.WriteValue("position", _cam._pos);
            w.WriteValue("orientation", _cam._euler);
            w.WriteValue("focalDistance", 8.0f);
            w.WriteValue("heightAngle", _scene._si.camFov);
        w.Close();
        
        w.Open("textures");
            texids[0] = 0;         // map index with texid
            TexSys::iterator bt = GTexSys.begin();
            TexSys::iterator et = GTexSys.end();
            for(;bt != et; bt++)
            {
                Texture& tex      = (*bt).second;
                texids[tex.hTex.hTex] = cntTex++;
                if(! strncmp(tex.filename, "@L", 2))
                {
                    continue;
                }
                w.Write("filename '%s' %d %d %d %x", tex.filename, tex.cx, tex.cy, tex.bpp, tex.hTex.glTarget);
            }
        w.Close();

        vvector<XmlFace>    faces(4096);  
        vvector<CLR>        colors(4096);    
        vvector<V3>         vxes(16384);
        vvector<UV>         tcs(8912);
        size_t                k;

        PBrushes* brshses = _scene.GetPrimitives();
        FOREACH(PBrushes, (*brshses), ppb)
        {
            Brush* pB = (*ppb)->_pResult;
            FOREACH(Polys, pB->_polys, pp)
            {
                XmlFace xf;
                xf.pPoly = (Poly*)&(*pp);

                k = colors.push_unique(pp->_colorD);
                xf.clri[0] =  k;               
                k = colors.push_unique(pp->_colorS);
                xf.clri[1] =  k;              
                k = colors.push_unique(pp->_colorE);
                xf.clri[2] =  k;
                FOREACH(vvector<Vtx2>, pp->_vtci, pv)
                {
                    vxes.push_unique((pv)->_xyz,&k);
                    xf.vxi  << k;
                    tcs.push_unique((pv)->_uv[0],&k);
                    xf.tc1i <<  k;
                    tcs.push_unique((pv)->_uv[1],&k);
                    xf.tc2i << k;
                    tcs.push_unique((pv)->_uv[2],&k);
                    xf.tc3i <<  k;
                }
                faces << xf;
            }
        }

    w.Open("points3Df");
    FOREACH(vvector<V3>, vxes, pv3)
    {
        w.WriteValue("", *pv3); // index
    }
    w.Close();

    w.Open("points2Df");
        FOREACH(vvector<UV>, tcs, puv)
        {
            w.WriteValue("", *puv); // index
        }
    w.Close();

    w.Open("colrs4ub");
        FOREACH(vvector<CLR>, colors, pcrl)
        {
            w.WriteValue("", *pcrl); // index
        }
    w.Close();

    w.Open("faces");
    int index=0;
    FOREACH(vvector<XmlFace>, faces, pxmlf)
    {
        w.Open((char*)MKSTR("%d", index++));

            if(pxmlf->pPoly->_polyflags || pxmlf->pPoly->_polyflags2)
                w.Write("flags %x %x %x ", pxmlf->pPoly->_polyprops, pxmlf->pPoly->_polyflags, pxmlf->pPoly->_polyflags2);
            w.Write("textureindex %d", texids[pxmlf->pPoly->GetHtex(0)]);
            if(pxmlf->pPoly->Apply(0))
                w.Write("texapplymodes %x", pxmlf->pPoly->Apply(0));
            w.Write("colorindex %d %d %d",pxmlf->clri[0], pxmlf->clri[1], pxmlf->clri[2]); 

            if(pxmlf->pPoly->_shiness)
                w.Write("shiness %d",pxmlf->pPoly->_shiness);

            if(pxmlf->pPoly->_bump!=1)
                w.Write("physics %f %f",pxmlf->pPoly->_bump, pxmlf->pPoly->_friction);
            if(pxmlf->pPoly->_use2splitidx!=-1)
                w.WriteValue("splitter",pxmlf->pPoly->_use2splitidx);
            
    
            w.Open("vertexes");
                vvector<int>::iterator ivx   = pxmlf->vxi.begin();
                vvector<int>::iterator itc1i = pxmlf->tc1i.begin();
//                vvector<int>::iterator itc2i = pxmlf->tc2i.begin();
//                vvector<int>::iterator itc3i = pxmlf->tc3i.begin();
                w.Tab();    
                for(;ivx != pxmlf->vxi.end();ivx++, itc1i++)//, itc3i++, itc2i++)
                {
                    w.WriteValNoCR("[%d %d] ",*ivx, *itc1i);//, *itc2i, *itc3i);
                }
            w.Close();
        
        w.Close();
    }
    w.Close();

    w.Open("brushes");
    vvector<XmlFace>::iterator xmlfi = faces.begin();
    int fIdx=0;
    FOREACH(PBrushes, (*brshses), ppb)
    {
        
        Brush* pB = (*ppb)->_pResult;

        pB->_name[15] = 0;
        w.Open(pB->_name);

        w.Write("flags %x %x",pB->_brushflags, pB->_brushprops);
        if(pB->_group)
            w.Write("group %d",pB->_group);
        int idxMotion = _scene.GetIndex(pB->_pMotion);
        if(idxMotion!=-1)
            w.WriteValue("motionsindex",idxMotion);
            w.Open("faceindexes");
                idx = 0;
                w.Tab();
                FOREACH(Polys, pB->_polys, pp)
                {
                    w.WriteValNoCR("%d ",fIdx++); 
                }
            w.Close();
        w.Close();
    }
    w.Close();

    w.Open("items");
    {
        FOREACH(vvector<SceItem*>, (*GetItems()), pItemPtr)
        {
            SceItem* pItem = *pItemPtr;
            pItem->_name[24] = 0;
            
            w.Open(pItem->_name);
            {

		        w.WriteValue("type",pItem->_item);
                w.WriteValue("group",pItem->_group);
                w.WriteValue("category",(char*)pItem->_catname);
		        w.Write("flags %x %x",pItem->_props, pItem->_flags);
		        w.WriteValue("color4ub",pItem->_colorD);
		        w.WriteValue("position",pItem->_t);
		        w.WriteValue("scalation",pItem->_s);
                w.WriteValue("orientation",pItem->_euler);
                w.WriteValue("texture", (char*)MKSTR("%d %d",texids[pItem->GetHtex(0)],pItem->Apply(0)));
                switch(pItem->_item)
                {
                    case ITM_LIGTBULB:

				         w.WriteValue("specularangle",((CLightBulb*)pItem)->_specAngle);
				         w.WriteValue("intensity",((CLightBulb*)pItem)->_lmIntensity);
				         w.WriteValue("maximradius",((CLightBulb*)pItem)->_radius);
				         w.WriteValue("minimhallo",((CLightBulb*)pItem)->_halloRadMin);
				         w.WriteValue("maximhallo",((CLightBulb*)pItem)->_halloRadMax);
                        break;
                    case ITM_TRIGER:
                        {
                            size_t   ptr = -1;
                            V3 ex = ((TriggerItem*)pItem)->_drawBrush._box.GetExtends();
                            w.WriteValue("effectpoint",((TriggerItem*)pItem)->_efectPoint);
                            w.WriteValue("extends",ex);
                        }
                        break;
                }    
                w.Close();
            }
	    }
        w.Close();    
    }

    w.Open("motions");    
    {
        FOREACH(vvector<CMotion*>,  (*_scene.GetMotions()), ppMotion)
        {
            CMotion* pM = *ppMotion;

            w.WriteValue("closed",  pM->_bLoop);
            w.WriteValue("center",pM->_center);
            w.WriteValue("brushindex", pM->_pBrush->_unicID);
            w.Open("path");    
            {
                for(int i=0;i<pM->GetNodesCount();i++)
                {
                    MState* pS = pM->GetState(i);

                    w.WriteValue("position", pS->_pos); 
                    w.WriteValue("orientation", pS->_euler);
                }
                w.Close();    
            }
        }
        w.Close();    
    }

    w.Open("scripts");    
    FOREACH(vvector<ScriptItem*>,  (*_scene.GetScripts()), ppScript)
    {
        ScriptItem*	pSi = *ppScript;
        w.Open("script");    
        {
            fw.Write((void*)pSi->_script.c_str(),pSi->_script.length());	
            w.Close();    
        }
    }
    w.Close();    

}

//---------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void CZ_ed2Doc::SaveInFile(const char* pSzName, BOOL selection)
{
    CDocSaver cfv(pSzName, TRUE);
    
    try
    {
        cfv.SerializeDoc(selection);
    }
    catch(const char* ex)
    {
        AfxMessageBox(ex,MB_OK|MB_ICONHAND);
    }
    catch(...)
    {
        AfxMessageBox("Unknkown Exception. Load/Save File Failure",MB_OK|MB_ICONHAND);
    }
    (VIEW3D(0,0))->SetFocus();
}



//----------------------------------------------------------------------------------------
void CZ_ed2Doc::ReadFromFile(const char* szFilename)
{
    Invalidate();
    theApp.PumpMessage(WM_PAINT,10);
    
    char        sigFile[5] = {0};
    FileWrap    fw;

    if(!fw.Open(szFilename,"rb"))
    {
        AfxMessageBox(MKSTR("Cannot Open The File: '%s'. Error: %d ", szFilename, GetLastError()),MB_OK|MB_ICONHAND);
    }

    fw.Read(sigFile,4);
    if(strcmp(sigFile,"GGRF") && strcmp(sigFile,"GPGF"))
    {
        fw.Close();
        AfxMessageBox("Current File Is Not a Getic Geometry File",MB_OK|MB_ICONHAND);
        return;
    }
    fw.Read(i_geomfilever);
    fw.Close();        
    
    if(i_geomfilever == DOC_VERSION3 || i_geomfilever == DOC_VERSION2)
    {
        AfxMessageBox("Very old files are not longer suported...",MB_OK|MB_ICONHAND);
        return ;
    }
    else if(i_geomfilever >= DOC_VERSION4)
    {
        START_WAIT("Please Wait...");

        CDocSaver cfv(szFilename, FALSE);

        try{
            MakeCurrent('3');       
            cfv.SerializeDoc(0);
            Recut();	
	        DeselectAll();
	        (VIEW3D(0,0))->SetFocus();
            END_WAIT("");
            AfxGetMainWnd()->SendMessage(WM_COMMAND, PB_REFR);
            DOC()->Invalidate();
            return;
        }catch(const char* ex)
        {
            END_WAIT("");
            AfxMessageBox(ex,MB_OK|MB_ICONHAND);
        }
        catch(...)
        {
            END_WAIT("");
            AfxMessageBox("Unknkown Exception. File is malformated",MB_OK|MB_ICONHAND);
        }
    }
    else
    {
        AfxMessageBox("Error Reading File. Version Not Longer Supported",MB_OK|MB_ICONHAND);
    }
    InitDocVars();
    (VIEW3D(0,0))->SetFocus();
    AfxGetMainWnd()->SendMessage(WM_COMMAND, PB_REFR);
    DOC()->Invalidate();
}

void CZ_ed2Doc::ReadFromBSP(const char* szFilename)
{

}
