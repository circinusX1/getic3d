// z_ed2Doc.cpp : implementation of the CZ_ed2Doc class
//

#include "stdafx.h"
#include "TexRef.h"
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
//---------------------------------------------------------------------------------------

#define DOC_VERSION2     2
#define DOC_VERSION3     3

//---------------------------------------------------------------------------------------
#define SECT_EDITOR     -1
#define SECT_TEXTURES   -2
#define SECT_BRUSHES    -3
#define SECT_ITEMS      -4
#define SECT_MOTION     -5
#define SECT_SCENINF    -6
#define SECT_SCRIPTS    -7
#define SECT_SOUNDS     -8
#define SECT_CATFILE	-9
#define SECT_COMPIESET  -10
#define SECT_EOF        -0xFF

#ifdef _FREE
#pragma message("----------------------------------BUILDING REE VERSION--------------------------*")
int __Get1000()
{
    int k = 10;
    for(int i=0;i<50;i++)
    {
        k+=10;
    }
    for(int j=0;j<50;j++)
    {
        k+=10;
    }

    return k;
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::TestFreeVersion(int polys)
{
    if(polys >  __Get1000())
    {
        _lasterror = "Number of polygons exceeds Getic 1.30.02.Free Limit. Level Cannot be saved");
        throw (int)1;
    }

}
#else
#pragma message("**************************BUILDING FULL VERSION*********************")
//---------------------------------------------------------------------------------------
void CZ_ed2Doc::TestFreeVersion(int polys){}

#endif //

//---------------------------------------------------------------------------------------
void __SaveInFileGEMS5(LPCTSTR szName, vvector<CBspTree*>& btrees)
{
    
    FileWrap    fw;
    fw.Open(MKSTR("%s.brush",szName),"wb");

    // save the BSP faces in the file
    FOREACH(vvector<CBspTree*>, btrees, ppTree)
    {
        CBspTree* pT =  *ppTree;

        FOREACH(vvector<Poly>, pT->GetPolys(), ppoly)
        {
            Poly* pp = ppoly;

            int vxsize = pp->_vtci.size();
            fw.Write(vxsize);

            FOREACH(vvector<Vtx2>, pp->_vtci, pvtci)
            {
                Vtx& r = *pvtci;
                fw.Write(r);
            }
        }
    }

}

//---------------------------------------------------------------------------------------
static void __ReadItmSceItem(FileWrap& fw, 
                           SceItem* pItem, 
                           vvector<Texture >&  texNames,  
                           int fVersion)
{
    V3 t;
	int id=0;

	fw.Read(pItem->_id);
    pItem->_id+=SceItem::_GItemID;

	fw.Read(pItem->_props);
	fw.Read(pItem->_flags);
	fw.Read(pItem->_colorD);
	fw.Read(t);
	fw.Read(pItem->_s);		
    fw.Read(pItem->_dir);		
    fw.Read(pItem->_dirl);

    fw.Read(pItem->_name, 32);		
	fw.Read(pItem->_group);
    pItem->_group+=DOC()->_maxUsedGrp;

    
    MakeCurrent('3');       // select GL ctx

    int     tmpVal;
    DWORD    dwTmpVal;
    
    fw.Read(pItem->_combine);
    for(int k=0;k<4;k++)
    {
        fw.Read(tmpVal);
        fw.Read(dwTmpVal);

        if(tmpVal >=0 && (UINT)tmpVal < texNames.size())
        {
            pItem->_textures._texApply[k] = dwTmpVal;
            pItem->SetTex(texNames[tmpVal].filename, k, texNames[tmpVal].genMode);
        }
    }
  
    pItem->SetPos(t);
}
    

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::SaveInFile(LPCTSTR pSzName, BOOL bBrshes)
{
    char         sFileName[_MAX_PATH];
    map<int,int> texids;
	FileWrap     fw;
    int          fileVer   = DOC_VERSION3;
    int          cntTex=0;
    GUID         g;
	long         tmpVal;
    int          polys = 0;
    int          kPolys = 0;
    _lasterror = "";

    polys = DOC()->_polycount = _scene.GetPolyCnt();
    TestFreeVersion(polys);
    
    
    AfxGetApp()->DoWaitCursor(1);
	REDIR();
	BARDLG()->OnApply() ;

	fw.Open(pSzName,"wb");
    
    //
    //  version and guid
    //
    fw.Write(GeticGeometryRawFile,4);
    fw.Write(fileVer);                            
    fw.Write(g);
	//
	// scen info-----------------------------------------------------------------------
	//
    tmpVal = SECT_SCENINF;
	fw.Write(tmpVal);
    _scene._GCarving = GCarving;
	fw.Write(_scene._si);
    fw.Write(_scene._GCarving);
	fw.Write(_scene._dummy);

    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,0);

    fw.Write(pTX->_zoom);
    fw.Write(pTX->_shifty);
    fw.Write(pTX->_shiftx);

    fw.Write(pTY->_zoom);
    fw.Write(pTY->_shifty);
    fw.Write(pTY->_shiftx);

    fw.Write(pTZ->_zoom);
    fw.Write(pTZ->_shifty);
    fw.Write(pTZ->_shiftx);

	//
    //  camera position--------------------------------------------------------------------
    //
    tmpVal = SECT_EDITOR;
    fw.Write(tmpVal);

    fw.Write(_cam._wpos);
    fw.Write(_cam._euler); 

    //--------------------------------------------------------------------------------------
    RWCompSettings(fw, 1);


    tmpVal = SECT_TEXTURES;
    fw.Write(tmpVal);
    int textures = 0;

    TexSys::iterator bt = GTexSys.begin();
    TexSys::iterator et = GTexSys.end();
    for(;bt != et; bt++)
    {
        Texture& tex      = (*bt).second;
		if(!strncmp(tex.filename, "@L",2))
			continue;
		++textures;
    }

    fw.Write(textures);

    texids[0]           = -1;        // 0 invalid
    bt = GTexSys.begin();
    for(;bt != et; bt++)
    {
        Texture& tex      = (*bt).second;
		if(!strncmp(tex.filename, "@L",2))
			continue;
        texids[tex.hTex.hTex] = cntTex++;         // map index with texid
        fw.Write(tex);
    }

    tmpVal = SECT_BRUSHES;
    fw.Write(tmpVal);

    PBrushes* brshses = _scene.GetPrimitives();
    if(bBrshes)
        brshses = _scene.GetBrushes();      // savbe the brushes not the primitives

	int	brushes = brshses->size();
	fw.Write(brushes);
    
    
    FOREACH(PBrushes, (*brshses), ppb)
	{
        Brush* pB = (*ppb);

        pB->Recalc();
		fw.Write(pB->_unicID   );
		fw.Write(pB->_t        );
		fw.Write(pB->_r        );
		fw.Write(pB->_s        );
        fw.Write(pB->_flags    );
        fw.Write(pB->_props    );
        fw.Write(pB->_group    );

		int idxMotion = _scene.GetIndex(pB->_pMotion);
        fw.Write(idxMotion); 
        
		fw.Write(pB->_physicsA);
        fw.Write(pB->_rotSteps);
        fw.Write(pB->_rotSteps);
        fw.Write(pB->_physicsD);
        fw.Write(pB->_cf);              // creation flags
		fw.Write(pB->_polySec);
        fw.Write(pB->_thikness);
        
        pB->_name[15]=0;
        fw.Write(pB->_name, 16);
        
        ///////////////////////////////////////////
		int sz = pB->_polys.size();
		fw.Write(sz);
        kPolys+=sz;

        TestFreeVersion(kPolys);

        int texIndex=0;
		FOREACH(Polys, pB->_polys, pp)
		{
			fw.Write(pp->_combine);
            for(int k=0;k<4;k++)
            {
                map<int, int>::iterator fi = texids.find((int)pp->_textures._texts[k]);
                if(fi != texids.end())
                {
                    fw.Write((UINT&) texids[pp->_textures._texts[k]]);
                }
                else
                {
                    int it = -1;
                    fw.Write((int&) it);
                }
                fw.Write(        pp->_textures._texApply[k]);
            }

            fw.Write(pp->_flags    );
            fw.Write(pp->_props);
            fw.Write(pp->_texcoord );

            fw.Write(pp->_lmInfo);
            fw.Write(pp->_tanimA);
            fw.Write(pp->_tanimB);
            fw.Write(pp->_texV1);
            fw.Write(pp->_texV2);
            fw.Write(pp->_colorD); // obsolette

            fw.Write(pp->_flags2);
            fw.Write(pp->_colorS);
            fw.Write(pp->_colorD);
            fw.Write(pp->_colorE);
            fw.Write(pp->_shiness);

            fw.Write(pp->_bump);
            fw.Write(pp->_friction);
            fw.Write(pp->_use2splitidx);


			sz = (*pp)._vtci.size();
			fw.Write(sz);
			FOREACH(vvector<Vtx2>, pp->_vtci, pv)
			{
				fw.Write((*pv));
			}
		}
	}
    TestFreeVersion(kPolys);
    // items
    tmpVal = SECT_ITEMS;
    fw.Write(tmpVal);
    int sz = GetItems()->size();
    fw.Write(sz);


    FOREACH(vvector<SceItem*>, (*GetItems()), pItemPtr)
    {
        SceItem* pItem = *pItemPtr;

		fw.Write(pItem->_item);
        fw.Write((void*)pItem->_catname.c_str(),16);

		fw.Write(pItem->_id);
		fw.Write(pItem->_props);
		fw.Write(pItem->_flags);
		fw.Write(pItem->_colorD);   

		fw.Write(pItem->_t);
		fw.Write(pItem->_s);

        fw.Write(pItem->_dir);		
        fw.Write(pItem->_dirl);


        fw.Write(pItem->_name, 32);		
		fw.Write(pItem->_group);

        
        fw.Write(pItem->_combine);
        for(int k=0;k<4;k++)
        {
            map<int, int>::iterator fi = texids.find((int)pItem->_textures._texts[k]);
            if(fi != texids.end())
            {
                fw.Write((UINT&) texids[pItem->_textures._texts[k]]);
            }
            else
            {
                int it = -1;
                fw.Write((int&) it);
            }
            fw.Write(pItem->_textures._texApply[k]);
        }

        switch(pItem->_item)
        {
            case ITM_LIGTBULB:
                 fw.Write(((CLightBulb*)pItem)->_dir);
				 fw.Write(((CLightBulb*)pItem)->_specAngle);
				 fw.Write(((CLightBulb*)pItem)->_lmIntensity);
				 fw.Write(((CLightBulb*)pItem)->_radius);
				 fw.Write(((CLightBulb*)pItem)->_halloRadMin);
				 fw.Write(((CLightBulb*)pItem)->_halloRadMax);
                 fw.Write(((CLightBulb*)pItem)->_colorS);   //bsolette
                 fw.Write(((CLightBulb*)pItem)->_colorE);   //bsolette

                break;
            case ITM_TRIGER:
                {
                    DWORD ptr = -1;
                    fw.Write(((TriggerItem*)pItem)->_efectPoint);
                    V3 ex = ((TriggerItem*)pItem)->_drawBrush._box.GetExtends();
                    fw.Write(ex);

                    if(((TriggerItem*)pItem)->_pBrush)
                    {
                        fw.Write(((TriggerItem*)pItem)->_pBrush->_unicID);
                    }
                    else
                    {
                        fw.Write(ptr);
                    }

                }
                break;
            case ITM_GAME:
                {
                    GameItem* pGameItem = (GameItem*)pItem;
                    int       datas = pGameItem->_data.size();
                    fw.Write(datas);
                    FOREACH(vvector<NameTypeValue>, pGameItem->_data, pdata)
                    {
                        fw.Write(*pdata);
                    }
                }
                break;
        }    
	}//rof

    tmpVal = SECT_MOTION;
    fw.Write(tmpVal);
    sz = _scene.GetMotions()->size();
    fw.Write(sz);

    FOREACH(vvector<CMotion*>,  (*_scene.GetMotions()), ppMotion)
    {
        CMotion* pM = *ppMotion;

        fw.Write(pM->_bLoop);
        fw.Write(pM->_center);
        fw.Write(pM->_pBrush->_unicID);
        sz = pM->GetNodesCount();
        fw.Write(sz);
        for(int i=0;i<pM->GetNodesCount();i++)
        {
            MState* pS = pM->GetState(i);

            fw.Write(pS->_pos); 
            fw.Write(pS->_dir);
            fw.Write(pS->_trTimes[0]);
            fw.Write(pS->_trTimes[1]);
            fw.Write(pS->_trRots[0]);
            fw.Write(pS->_trRots[1]);
        }
    }

	// scripts
	{
        //section
	    tmpVal = SECT_SCRIPTS;//SECT_SOUNDS
		fw.Write(tmpVal);
        // no of scripts
		sz = _scene.GetScripts()->size();
		fw.Write(sz);

	    FOREACH(vvector<ScriptItem*>,  (*_scene.GetScripts()), ppScript)
		{
			ScriptItem*	pSi = *ppScript;
			
			tmpVal = pSi->_name.length();	
			fw.Write(tmpVal);	
			fw.Write((void*)pSi->_name.c_str(),pSi->_name.length());	

            
			// script text
			tmpVal = pSi->_script.length();	
			fw.Write(tmpVal);	
			fw.Write((void*)pSi->_script.c_str(),pSi->_script.length());	
		}

	}

    ///sounds
    {
	    tmpVal = SECT_SOUNDS;
		fw.Write(tmpVal);
		sz = _scene.GetSounds()->size();
		fw.Write(sz);

	    FOREACH(vvector<CSoundItem*>,  (*_scene.GetSounds()), ppSound)
		{
			CSoundItem*	pSi = *ppSound;

   			tmpVal = _scene.GetIndex(pSi);//dummy
			fw.Write(tmpVal); 
			
 			tmpVal = pSi->_filename.length();	
			fw.Write(tmpVal);	
			fw.Write((void*)pSi->_filename.c_str(),pSi->_filename.length());	

            fw.Write(pSi->_volume);	
            fw.Write(pSi->_radius);	
            tmpVal =pSi->GetRefs();
            fw.Write(tmpVal);	

            //
            // dummy for further dev
            //
        
		}

    }


	tmpVal = SECT_CATFILE;
	fw.Write(tmpVal);
	::memset(sFileName,0,sizeof(sFileName));
	if(theApp.XsdCatItmFile()[0])
		strcpy(sFileName, theApp.XsdCatItmFile());
	fw.Write((void*)sFileName,255);

    tmpVal = SECT_EOF;
	fw.Write(tmpVal);

	fw.Close();
	SetModifiedFlag(FALSE);     
 	AfxGetApp()->DoWaitCursor(0);



}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnFileOpen() 
{
    char szFilters[] = _T("Geometry raw (*.grf)|*.grf|"
                          "Geometry selection (*.grs)|*.grs|"  
                          "Geometry prefab (*.gpm)|*.gpm|"  
                          "All Files (*.*)|*.*||");
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

	DeleteContents();
    InitDocVars();
    ReadFromFile(dlg.m_ofn.lpstrFile);
    SetModifiedFlag(TRUE);  
    SetTitle(dlg.m_ofn.lpstrFile);
    TestBackupFile();

}

void CZ_ed2Doc::ReadFromFile(const TCHAR* szFilename)
{


    FileWrap            fw;
    int                 docuse2split = 0;
    long                tmpVal,polys,vxes,brushes;
    DWORD                dwTmpVal;
    vvector<Texture>    texNames;
    char                szTexName[128] = {0};
    map<int,Brush*>     dynaBrushes ;
    GUID                g;
    BYTE                buffer[_MAX_PATH];

    CWaitCursor cwc;
	//MakeCurrent(0,0);
    MakeCurrent('3');       // select GL ctx
    _TRY
    {

	    fw.Open(szFilename,"rb");

        //
        //  version and guid
        //
        fw.Read(szTexName,4);

        if(!strcmp(szTexName,GeticPrimitiveFile))
        {
            LoadPrefFile(fw);
            fw.Close();
            return;
        }
        
        if(strcmp(szTexName,GeticGeometryRawFile))
        {
            fw.Close();
            AfxMessageBox("Current File Is Not a Getic Geometry File",MB_OK|MB_ICONHAND);
            return;
        }

        fw.Read(i_geomfilever);

        if(i_geomfilever < DOC_VERSION2)
        {
            fw.Close();
            AfxMessageBox(MKSTR("Cannot Load File Version Under %d", DOC_VERSION2),MB_OK|MB_ICONHAND);
            return;
        }

        fw.Read(g);

		fw.Read(tmpVal);
		if(tmpVal != SECT_SCENINF)
		{
			fw.Close();
			AfxMessageBox(_T("Dammaged File or Invalid Version"));
            InitDocVars();
			return;
		}

        fw.Read(_scene._si);
        fw.Read(_scene._GCarving);
		fw.Read(_scene._dummy);
        GCarving=_scene._GCarving;


        CZ_ed2View* pTX = VIEW2D(1,1);
        CZ_ed2View* pTY = VIEW2D(0,1);
        CZ_ed2View* pTZ = VIEW2D(1,0);

        fw.Read(pTX->_zoom);
        fw.Read(pTX->_shifty);
        fw.Read(pTX->_shiftx);

        fw.Read(pTY->_zoom);
        fw.Read(pTY->_shifty);
        fw.Read(pTY->_shiftx);

        fw.Read(pTZ->_zoom);
        fw.Read(pTZ->_shifty);
        fw.Read(pTZ->_shiftx);

        //
        //  editor vars
        //
        fw.Read(tmpVal);
        if(tmpVal != SECT_EDITOR)
        {
            fw.Close();
            AfxMessageBox(_T("Dammaged File "));
            return;
        }


    
        START_WAIT("Loading...");

        assert(tmpVal == SECT_EDITOR);
        fw.Read(_cam._wpos);
        fw.Read(_cam._euler);


        _cam.Euler2Pos();

        RWCompSettings(fw, 0);

        fw.Read(tmpVal);
        assert(tmpVal == SECT_TEXTURES);

		Texture     tex ;

        fw.Read(tmpVal);
        for(int i=0;i<tmpVal;i++)
        {
            fw.Read(tex);
			if(tex.cx==0||tex.cy==0)
				continue;
            texNames.push_back(tex );
        }
    
        fw.Read(tmpVal);
        assert(tmpVal == SECT_BRUSHES);

	    fw.Read(brushes);
	    for(int b = 0; b< brushes; b++)
	    {
		    Brush* pB = new Brush;
		    
		    fw.Read(pB->_unicID);
            //pB->_unicID += Brush::GID;

		    fw.Read(pB->_t);
		    fw.Read(pB->_r);
		    fw.Read(pB->_s);
            fw.Read(pB->_flags);
			
			fw.Read(pB->_props);
            fw.Read(pB->_group);
            pB->_group+=_maxUsedGrp;

			// motion index saved	
			fw.Read(tmpVal);					// has motion index
            if(tmpVal>=0)						// for redoing the pointer in the motion object
                dynaBrushes[pB->_unicID] = pB;	// store this brush here
            pB->_pMotion  = 0;
            
			fw.Read(pB->_physicsA);
            fw.Read(pB->_rotSteps);
            fw.Read(pB->_rotSteps);
            fw.Read(pB->_physicsD);
            fw.Read(pB->_cf);
			fw.Read(pB->_polySec);
            fw.Read(pB->_thikness);


            fw.Read(pB->_name, 16);
			pB->_name[15]=0;

		    fw.Read(polys);
		    Poly	pl;
		    for(int p=0;p<polys;p++)
		    {
			    pl._vtci.clear();
                
                fw.Read(pl._combine);
                for(int k=0;k<4;k++)
                {
                    fw.Read(tmpVal);
                    fw.Read(dwTmpVal);

                    if(tmpVal >=0 && (UINT)tmpVal < texNames.size())
                    {
                        pl._textures._texApply[k] = dwTmpVal;

                        pl.SetTex(texNames[tmpVal].filename, k , texNames[tmpVal].genMode);
                    }
                }
                
                fw.Read(pl._flags);
                fw.Read(pl._props);
                fw.Read(pl._texcoord );
                fw.Read(pl._lmInfo);
                fw.Read(pl._tanimA);
                fw.Read(pl._tanimB);
                fw.Read(pl._texV1);
                fw.Read(pl._texV2);
                fw.Read(pl._colorD); //obsolette
                fw.Read(pl._flags2);
                fw.Read(pl._colorS);
                fw.Read(pl._colorD);
                if(pl._colorD.r==0 && pl._colorD.g==0  && pl._colorD.g==0 )
                {
                    pl._colorD = ZWHITE;
                }

                fw.Read(pl._colorE);
                fw.Read(pl._shiness);
                fw.Read(pl._bump);
                fw.Read(pl._friction);
                fw.Read(pl._use2splitidx);

                docuse2split = tmax(docuse2split, pl._use2splitidx);

			    pl._pBrush = pB;
                fw.Read(vxes); 
			    for(int v=0;v<vxes;v++)
			    {
				    Vtx2	v;
					fw.Read(&v,sizeof(Vtx2));
				    pl._vtci.push_back(v);
			    }
			    pl.Recalc();

                if(pl._props & FACE_BLACKMASK)
                    pl.ReApplyTexture(GEN_TEX_2D_MAP);

			    pB->_polys.push_back(pl);
			    
		    }
            pB->Recalc();

		    if(pB->IsSolid() || pB->IsProjector())
            {
                SelectBrush(pB);
            }

            _scene.AddPrimitive(pB);
		    Brush::GID = max(Brush::GID, pB->_unicID);
        
            KEEP_WAIT(MKSTR("Loading Brush %d ",pB->_unicID));

	    }

       // items
	    
        int		items;
        char    szitemcat[32];

        fw.Read(tmpVal);
        assert(tmpVal == SECT_ITEMS);
        fw.Read(items);

        for( i=0;i<items;i++)
        {
		    int item; 
		    fw.Read(item);
            
            memset(szitemcat,0,sizeof(szitemcat));
            fw.Read(szitemcat,16);
            
		    
		    switch(item)
            {
                case ITM_LIGTBULB:
                    {
				         CLightBulb* pItem = new CLightBulb();
				         pItem->_catname = szitemcat;
                         __ReadItmSceItem(fw,pItem, texNames, i_geomfilever);
                         fw.Read(pItem->_dir);
				         fw.Read(pItem->_specAngle);
				         fw.Read(pItem->_lmIntensity);
				         fw.Read(pItem->_radius);
				         fw.Read(pItem->_halloRadMin);
				         fw.Read(pItem->_halloRadMax);
                            
                          fw.Read(pItem->_colorS);   //bsolette
                          fw.Read(pItem->_colorE);   //bsolette


					    pItem->Commit();
					    pItem->_flags &= ~BRSH_NEW;
					    _scene.AddItem(pItem);
    				    BARDLG()->AddItem(pItem);
                    }
                    break;

                case ITM_TRIGER:
                    {
                        DWORD   dummy;
                        V3      ex;

                        TriggerItem* pItem = new TriggerItem();
                        
                        pItem->_catname = szitemcat;
                        __ReadItmSceItem(fw,pItem, texNames, i_geomfilever);
                        fw.Read(pItem->_efectPoint);
                        fw.Read(ex);
                        ex.x /= 2.0;
                        ex.y /= 2.0;
                        ex.z /= 2.0;
                        pItem->_drawBrush.SetSize(Box(-ex,ex));
						pItem->_drawBrush.Recalc();

                        fw.Read(dummy); // brush ID

                        pItem->_pBrush = _scene.GetBrushByID((int)dummy);
						if(pItem->_pBrush)
						{

							++pItem->_pBrush->_nTrigers;
							pItem->_pBrush->_flags |= BRSH_HASTRIG;
						}
                        
                        pItem->_flags &= ~BRSH_NEW;
                        pItem->Commit();

	    				_scene.AddItem(pItem);
    	    			BARDLG()->AddItem(pItem);
                    }
					break;
                case ITM_GAME:
                    {
                        GameItem* pItem = new GameItem();
                        pItem->_catname = szitemcat;
						__ReadItmSceItem(fw,pItem, texNames, i_geomfilever);

                        NameTypeValue   ntv;
                        int             datas;
                        fw.Read(datas);
                        
                        for(int i=0; i < datas; i++)
                        {
                            fw.Read(ntv);
                            pItem->_data.push_back(ntv);
                        }

						pItem->_flags &= ~BRSH_NEW;
						pItem->Commit();
	    				_scene.AddItem(pItem);
    	    			BARDLG()->AddItem(pItem);
                    }
                    break;
            }    
        }
    
		int		nIndex;
		fw.Read(tmpVal);
		assert(tmpVal == SECT_MOTION);
		fw.Read(items);

		for (i=0;i<items;i++)
		{
			CMotion* pM = new CMotion(0, 0, 0);

			fw.Read(pM->_bLoop);
			fw.Read(pM->_center);
			//
			// link them back
			//
			fw.Read(nIndex);					//uuid of brush
			pM->_pBrush = dynaBrushes[nIndex];	// take it from array
			pM->_pBrush->_pMotion = pM;			// link back

			fw.Read(pM->_iNodes);
			for(int i=0; i < pM->_iNodes; i++)
			{
				MState  state;

				fw.Read(state._pos);
				fw.Read(state._dir);
				fw.Read(state._trTimes[0]);
				fw.Read(state._trTimes[1]);
				fw.Read(state._trRots[0]);
				fw.Read(state._trRots[1]);

				pM->_states << state;

			}
			_scene.AddMotion(pM);
		}

		// scripts
		{
            TCHAR sz[_MAX_PATH];
			fw.Read(tmpVal);
			assert(tmpVal == SECT_SCRIPTS);

			int		nScripts;
			int		tLen;
			fw.Read(nScripts);
			for(int i=0;  i<nScripts; i++)
			{
				ScriptItem* pSci = new ScriptItem;
            				
                // rad script name
                fw.Read(tLen);	    
                memset(sz,0,sizeof(sz));
				fw.Read(sz, tLen);	
				pSci->_name = sz;


				//read tex lentgh
				fw.Read(tLen);	//associated item
                memset(sz,0,sizeof(sz));
				fw.Read(sz, tLen);	//associated item
				pSci->_script = sz;

				_scene.AddScript(pSci);
			}



		}

        {   
            TCHAR sz[_MAX_PATH];

            fw.Read(tmpVal);
			assert(tmpVal == SECT_SOUNDS);

			int		nScripts;
			int		tLen;
			fw.Read(nScripts);
			for(int i=0;  i<nScripts; i++)
			{
				CSoundItem* pSi = new CSoundItem;
            				
				fw.Read(tmpVal);                //dummy index of this
				fw.Read(tLen);	                // sound filename
                ::memset(sz,0,sizeof(sz));
				fw.Read(sz, tLen);	            
                pSi->_filename = sz;

                fw.Read(pSi->_volume);	
                fw.Read(pSi->_radius);	
                fw.Read(tmpVal);	
                pSi->SetRefs(tmpVal);

                //
                // dummy for further dev
                //
                
                _scene.AddSound(pSi);
			}
        }
		//cat file
		fw.Read(tmpVal);

		ASSERT(tmpVal == SECT_CATFILE);
		{
			tmpVal = SECT_CATFILE;

			::memset(buffer,0,sizeof(buffer));
			fw.Read((void*)buffer,255);
			if(buffer[0])
			{
				if(DOC()->_scene.SearchCats((TCHAR*)buffer))
				{
					theApp.XsdCatItmFile((TCHAR*)buffer);
				}
			}
		}
        //EOF
        {
            fw.Read(tmpVal);
            ASSERT(tmpVal == SECT_EOF);
        }

        fw.Close();
        Recut();	
	    DeselectAll();

	    _selmode    = SEL_NONE;
	    _brmode		= BR_VIEW;

        DOC()->_use2splitidx = tmax(docuse2split+1, DOC()->_use2splitidx);

        END_WAIT("");
        //++Brush::GID;
	    BARDLG()->RefreshItemsList();
    }
	_CATCHX()
    {
        AfxMessageBox("File is corupt. Check /temp Directory for backup files. Cannot Load.",MB_OK|MB_ICONHAND);
    }

	(VIEW3D(0,0))->SetFocus();

}


void CZ_ed2Doc::SerializeScene(FileWrap& fw, bool direction)
{
}

void CZ_ed2Doc::SerializeBrush(FileWrap& fw, bool direction)
{
}

void CZ_ed2Doc::SerializePolygon(FileWrap& fw, bool direction)
{
}

void CZ_ed2Doc::SerializeTextures(FileWrap& fw, bool direction)
{
}

void CZ_ed2Doc::SerializeItems(FileWrap& fw, bool direction)
{
}

void CZ_ed2Doc::SerializeScripts(FileWrap& fw, bool direction)
{
}

void CZ_ed2Doc::SerializeMotions(FileWrap& fw, bool direction)
{
}

void CZ_ed2Doc::SerializeGameData(FileWrap& fw, bool direction)
{
}


BOOL FixBadName(TCHAR* pszname)
{
	TCHAR		invalids[] =_T("!@#$%^&^*()_+~|}{():\"\'.,<>?/-=\\][");


	char* pinvalid = invalids;

	while(*pinvalid)
	{
        StripChar(pszname,*(pinvalid++));
	}
	return 0;

}


void      CZ_ed2Doc::SavePrefFile()
{
    
	int          fVersion   = DOC_VERSION3;
    map<int,int> texids;
    int          cntTex = 0;
    REDIR();

    DlgCpxPrimnames d;
    if(d.DoModal() == IDOK)
    {
        FileWrap    fw;

		REDIR();
        CDirChange  cdd(d._curPath.c_str());

        PBrushes* pBrushes = _scene.GetBrushes();
        cdd.UseObj();      
        FOREACH(PBrushes, (*pBrushes), ppCpxBrush)
        {
            Brush brush = *(*ppCpxBrush);

            brush.Recalc();
            
            V3  center = brush._box.GetCenter();
            brush.Move(-center);

            TCHAR locName[64];
            _tcscpy(locName, brush._name);
			if(FixBadName(locName))
			{
				return;
			}

            if(fw.Open(MKSTR("%s.gpm",locName),"wb"))
            {
                fw.Write(GeticPrimitiveFile,4);
                fw.Write(fVersion);                  // version


				int textures = 0;

				TexSys::iterator bt = GTexSys.begin();
				TexSys::iterator et = GTexSys.end();
				for(;bt != et; bt++)
				{
					Texture& tex      = (*bt).second;
					if(!strncmp(tex.filename, "@L",2))
						continue;
					++textures;
				}

				fw.Write(textures);

				texids[_dumptex] = -1;        // 0 invalid
				bt = GTexSys.begin();
				for(;bt != et; bt++)
				{
					Texture& tex      = (*bt).second;
					if(!strncmp(tex.filename, "@L",2))
						continue;
					texids[tex.hTex.hTex] = cntTex++;         // map index with texid
					fw.Write(tex);
				}


		        fw.Write(brush._unicID   );
		        fw.Write(brush._t        );
		        fw.Write(brush._r        );
		        fw.Write(brush._s        );
                fw.Write(brush._flags    );
                fw.Write(brush._props    );
                fw.Write(brush._group    );

                fw.Write(brush._pMotion ); 

                fw.Write(brush._physicsA);
                fw.Write(brush._rotSteps);
                fw.Write(brush._rotSteps);
                fw.Write(brush._physicsD);
                fw.Write(brush._cf);              // creation flags
                fw.Write(brush._name, 16);

                ////////////////////////////////////////////////////////////
            
		        int sz = brush._polys.size();
		        fw.Write(sz);

		        FOREACH(Polys, brush._polys, pp)
		        {

                    fw.Write(pp->_combine);
                    for(int k=0;k<4;k++)
                    {
                        map<int, int>::iterator fi = texids.find((int)pp->_textures._texts[k]);
                        if(fi != texids.end())
                        {
                            fw.Write((UINT&) texids[pp->_textures._texts[k]]);
                        }
                        else
                        {
                            int it = -1;
                            fw.Write((int&) it);
                        }
                        fw.Write(        pp->_textures._texApply[k]);
                    }

                    fw.Write(pp->_flags    );
                    fw.Write(pp->_props);
                    fw.Write(pp->_texcoord );
                    fw.Write(pp->_lmInfo);
                    fw.Write(pp->_tanimA);
                    fw.Write(pp->_tanimB);
                    fw.Write(pp->_texV1);
                    fw.Write(pp->_texV2);
                    fw.Write(pp->_colorD);

                    fw.Write(pp->_flags2);
                    fw.Write(pp->_colorS);
                    fw.Write(pp->_colorD);
                    fw.Write(pp->_colorE);
                    fw.Write(pp->_shiness);

                    fw.Write(pp->_bump);
                    fw.Write(pp->_friction);
                    fw.Write(pp->_use2splitidx);

                    ////////////////////////////////////////////////////////////
                    

			        sz = (*pp)._vtci.size();
			        fw.Write(sz);
			        FOREACH(vvector<Vtx2>, pp->_vtci, pv)
			        {
				        fw.Write((*pv));
			        }
		        }
                fw.Close();
            }
        }
    }
    (VIEW3D(0,0))->SetFocus();
	_bShowCsg=0;
}

BOOL CZ_ed2Doc::LoadPrefFile(FileWrap& fw, Brush** pBRet, vvector<Texture >* pTexNames)
{
    int               docuse2split=0;
    int               tmpVal;
    int               uniqueIdDUmmy;
    int               fVersion;
    DWORD              dwTmpVal;
    vvector<Texture > texNames;
	Brush*			  pB = 0;// = new Brush();

    CDirChange  cdd("");

    cdd.UseObj();
    REDIR();
    _TRY
    {

        fw.Read(fVersion);                  // version
		if(fVersion < DOC_VERSION2 )
		{
            AfxMessageBox(MKSTR("Cannot Model Load File Version Under %d", DOC_VERSION2),MB_OK|MB_ICONHAND);
			return FALSE;
		}
    
		Texture tex ;

		fw.Read(tmpVal);
        for(int i=0;i<tmpVal;i++)
        {
            fw.Read(tex);
            texNames.push_back(tex );
        }

		pB = new Brush();

		// as a new brush so It's unique ID is the generated one
		fw.Read(uniqueIdDUmmy);

		fw.Read(pB->_t);
		fw.Read(pB->_r);
		fw.Read(pB->_s);
		fw.Read(pB->_flags);
		fw.Read(pB->_props);
		fw.Read(pB->_group);


		fw.Read(pB->_pMotion );
		pB->_pMotion=0;
		fw.Read(pB->_physicsA);
		fw.Read(pB->_rotSteps);
		fw.Read(pB->_rotSteps);
		fw.Read(pB->_physicsD);
		fw.Read(pB->_cf);
		fw.Read(pB->_name, 16);
		pB->_name[15]=0;


		int polys;
		fw.Read(polys);
		Poly	pl;
		for(int p=0;p<polys;p++)
		{
			pl._vtci.clear();

            fw.Read(pl._combine);

            for(int k=0;k<4;k++)
            {
                fw.Read(tmpVal);
                fw.Read(dwTmpVal);

                if(tmpVal >=0 && (UINT)tmpVal < texNames.size())
                {
                    pl._textures._texApply[k] = dwTmpVal;
                    pl.SetTex(texNames[tmpVal].filename, k, texNames[tmpVal].genMode);
                }
            }


            fw.Read(pl._flags);
			fw.Read(pl._props);
			fw.Read(pl._texcoord );
			fw.Read(pl._lmInfo);
			fw.Read(pl._tanimA);
			fw.Read(pl._tanimB);
			fw.Read(pl._texV1);
			fw.Read(pl._texV2);
			fw.Read(pl._colorD);
            if(pl._colorD.r==0 && pl._colorD.g==0  && pl._colorD.g==0)
            {
                pl._colorD = ZWHITE;
            }


            fw.Read(pl._flags2);
            fw.Read(pl._colorS);
            fw.Read(pl._colorD);
            fw.Read(pl._colorE);
            fw.Read(pl._shiness);
            fw.Read(pl._bump);
            fw.Read(pl._friction);
            fw.Read(pl._use2splitidx);

            docuse2split = max(docuse2split, pl._use2splitidx);


			pl._pBrush = pB;
			int vxes;
			fw.Read(vxes); 
			for(int v=0;v<vxes;v++)
			{
				Vtx2	v;
				fw.Read(&v,sizeof(Vtx2));
				pl._vtci.push_back(v);
			}
			pl.Recalc();

            if(pl._props & FACE_BLACKMASK)
                pl.ReApplyTexture(GEN_TEX_2D_MAP);

			pB->_polys.push_back(pl);
			
		}
		pB->Recalc(1);

        DOC()->_use2splitidx = tmax(docuse2split+1, DOC()->_use2splitidx);

	}
	_CATCHX()
	{
		delete pB;
		if(pTexNames)
			pTexNames->clear();

		AfxMessageBox("File Is Corrupt",MB_OK|MB_ICONWARNING);
		return 0;
	}

    return ResolveTexturesOnHRC(pB, pBRet, texNames, pTexNames);
}


BOOL CZ_ed2Doc::ResolveTexturesOnHRC(Brush* pB, Brush** pBRet, 
                                vvector<Texture >&  texNames,
                                vvector<Texture >*  pTexNames)
{
 
    if(pBRet)
    {
        *pBRet = pB;
        return 1;
    }
    // MAIN HRC
    
    // place cuts in selection list to updated the cuts CSG
    if(!(pB->_flags & BRSH_SOLID) && !pB->IsProjector())
        SelectBrush(pB);

    _scene.AddPrimitive(pB);
    SelectBrush(pB);
    Recut();	
    return TRUE;

}

void CZ_ed2Doc::OnFileMerge() 
{

    char szFilters[] = _T("Geometry raw (*.grf)|*.grf|"
                          "Geometry selection (*.grs)|*.grs|"  
                          "Geometry prefab (*.gpm)|*.gpm|"  
                          "All Files (*.*)|*.*||");
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

class NO_VT XmlWritter
{
public:
    XmlWritter(FileWrap* pfw):_pfw(pfw),_tabs(0){}
   	void Write(const TCHAR* p,...)
    {
		va_list args;
		va_start(args, p);
        ::_vsnprintf(_s, sizeof(_s) / sizeof(TCHAR), p, args);
		va_end(args);

        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");
        _pfw->WriteTextAsIs(_s);
        _pfw->WriteTextAsIs("\r\n");
	}
 	
    void WriteTag(const TCHAR* sect, TCHAR* p)
    {
      for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("<%s>",sect));
        _pfw->WriteTextAsIs(p);
        _pfw->WriteTextAsIs(MKSTR("</%s>",sect));
        
        _pfw->WriteTextAsIs("\r\n");
    }
    void WriteTag(const TCHAR* sect, CLR& f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("<%s>",sect));
        _pfw->WriteTextAsIs(MKSTR("%d,", f.r));
        _pfw->WriteTextAsIs(MKSTR("%d,", f.g));
        _pfw->WriteTextAsIs(MKSTR("%d", f.b));
        _pfw->WriteTextAsIs(MKSTR("</%s>",sect));
        _pfw->WriteTextAsIs("\r\n");

    }
   	void WriteTag(const TCHAR* sect, V3& f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("<%s>",sect));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f.x));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f.y));
        _pfw->WriteTextAsIs(MKSTR("%6.4f",  f.z));

        _pfw->WriteTextAsIs(MKSTR("</%s>",sect));
        _pfw->WriteTextAsIs("\r\n");
    }

   	void WriteTag(const TCHAR* sect, Vtx2& f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("<%s>",sect));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._xyz.x));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._xyz.y));
        _pfw->WriteTextAsIs(MKSTR("%6.4f",  f._xyz.z));

        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._nrm.x));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._nrm.y));
        _pfw->WriteTextAsIs(MKSTR("%6.4f",  f._nrm.z));

        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._uv[0].u));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._uv[0].v));

        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._uv[1].u));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._uv[1].v));

        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._uv[2].u));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._uv[2].v));

        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._uv[3].u));
        _pfw->WriteTextAsIs(MKSTR("%6.4f,", f._uv[3].v));

        _pfw->WriteTextAsIs(MKSTR("%d,", f._rgb.r));
        _pfw->WriteTextAsIs(MKSTR("%d,", f._rgb.g));
        _pfw->WriteTextAsIs(MKSTR("%d", f._rgb.b));

        _pfw->WriteTextAsIs(MKSTR("</%s>",sect));
        _pfw->WriteTextAsIs("\r\n");
	}

    void WriteTag(const TCHAR* sect, long f)
    {
        WriteTag(sect, (int) f);
    }
    void WriteTag(const TCHAR* sect, DWORD f)
    {
        WriteTag(sect, (int) f);
    }

    void WriteTag(const TCHAR* sect, int f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("<%s>",sect));
        _pfw->WriteTextAsIs(MKSTR("%d", f));
        _pfw->WriteTextAsIs(MKSTR("</%s>",sect));
        
        _pfw->WriteTextAsIs("\r\n");
	}


    void WriteTag(const TCHAR* sect, float f)
    {
        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");

        _pfw->WriteTextAsIs(MKSTR("<%s>",sect));
        _pfw->WriteTextAsIs(MKSTR("%06.6f", (float)f));
        _pfw->WriteTextAsIs(MKSTR("</%s>",sect));
        
        _pfw->WriteTextAsIs("\r\n");
	}


    void Open(TCHAR* p)
    {
        _open.push_back(p);

        for(int i=0; i<_tabs; i++)
           _pfw-> WriteTextAsIs("   ");
        _pfw->WriteTextAsIs(MKSTR("<%s>\r\n", p));
        _tabs++;
    }
    void Close()
    {
        _tabs--;
        string& s = _open.back();

        for(int i=0; i<_tabs; i++)
            _pfw->WriteTextAsIs("   ");
        _pfw->WriteTextAsIs(MKSTR("</%s>\r\n", s.c_str()));
        _open.pop_back();

    }
private:
    TCHAR       _s[512];
    FileWrap*   _pfw;
    int         _tabs;
    vvector<string>       _open;
};


  
//---------------------------------------------------------------------------------------
void CZ_ed2Doc::SaveInXML(LPCTSTR pSzName, BOOL bBrshes)
{
    map<int,int>    texids;
    int             idx = 0;
	FileWrap        fw;
    int             fileVer   = DOC_VERSION3;
    int             cntTex=0;
    GUID            g = {0};
    char            nospacestexfile[_MAX_PATH];
	

	AfxGetApp()->DoWaitCursor(1);
	REDIR();
	BARDLG()->OnApply() ;
	fw.Open(pSzName,"wb");

    XmlWritter  xmlw(&fw);
    
    xmlw.Open("XML");
    {
        xmlw.Write(MKSTR("<FILE SIGNATURE=\"GGRF\" NAME=\"%s\" VERSION=\"%x\">%x-%x-%x-%x%x%x%x%x%x%x%x</FILE>",
                      pSzName,fileVer,g.Data1,g.Data2,g.Data3,g.Data4[0],g.Data4[1],g.Data4[2],g.Data4[3],g.Data4[4],g.Data4[5],g.Data4[6],g.Data4[7]));    

        xmlw.Open("SCENE");
        {
            xmlw.Open("EDITOR");
            {
                CZ_ed2View* pTX = VIEW2D(1,1);
                CZ_ed2View* pTY = VIEW2D(0,1);
                CZ_ed2View* pTZ = VIEW2D(1,0);

	            //
                //  camera position
                //

                xmlw.WriteTag("CAM-POS",_cam._wpos);
                xmlw.WriteTag("CAM-ANG",_cam._euler); 

                xmlw.WriteTag("X-ZOOM",pTX->_zoom);
                xmlw.WriteTag("X-VSHIFT",pTX->_shifty);
                xmlw.WriteTag("X-HSHIFT",pTX->_shiftx);

                xmlw.WriteTag("Y-ZOOM",pTY->_zoom);
                xmlw.WriteTag("Y-VSHIFT",pTY->_shifty);
                xmlw.WriteTag("Y-HSHIFT",pTY->_shiftx);

                xmlw.WriteTag("Z-ZOOM",pTZ->_zoom);
                xmlw.WriteTag("Z-VSHIFT",pTZ->_shifty);
                xmlw.WriteTag("Z-HSHIFT",pTZ->_shiftx);
	            
                xmlw.Close();
            }

            xmlw.Open("INFO");
            {
                xmlw.WriteTag("AUTHOR", _scene._si.author);  
                xmlw.WriteTag("COMMENTS",_scene._si.comments);     
                xmlw.WriteTag("LEVEL",_scene._si.levelType);         
                xmlw.WriteTag("ACTORS",_scene._si.maxActors);         
                xmlw.WriteTag("FRAGS",_scene._si.maxFrags);          
                xmlw.WriteTag("TIME",_scene._si.maxTime);           
                xmlw.WriteTag("ILUM",_scene._si.defLumin);          
                xmlw.WriteTag("GRAV",_scene._si.gravAcc);           
                xmlw.WriteTag("FAR",_scene._si.camFar);            
                xmlw.WriteTag("FOV",_scene._si.camFov);            
                xmlw.WriteTag("SPEED",_scene._si.sceSpeed);          
                xmlw.WriteTag("FLAGS",_scene._si.sceFlags);          
                xmlw.WriteTag("FOG0",_scene._si.fogNear);           
                xmlw.WriteTag("FOG1",_scene._si.fogFar);
                xmlw.WriteTag("FOGCLR",_scene._si.fogColor);
                xmlw.WriteTag("FOGDENS",_scene._si.fogDens);
                xmlw.WriteTag("FOGFALL",_scene._si.fogFallOff);
                xmlw.WriteTag("SPLASH",_scene._si.sceneSpash);        
                xmlw.Close();
            }
            


            xmlw.Open("TEXTURES");
            {
                idx = 0;
                TexSys::iterator bt = GTexSys.begin();
                TexSys::iterator et = GTexSys.end();
                for(;bt != et; bt++)
                {
                    Texture& tex      = (*bt).second;
                        
                    if(! strncmp(tex.filename, "@L", 2))
                        continue;
                    texids[tex.hTex.hTex] = cntTex++;         // map index with texid

                    xmlw.Open("TEX");
                    {
                        ::strcpy(nospacestexfile, tex.filename);
                        Replace(nospacestexfile, ' ', '_');
                        xmlw.Open(tex.filename);
                        {
                            xmlw.WriteTag("BPP-X-Y-GL",(TCHAR*)MKSTR("%d,%d,%d,%d,%d",idx, tex.bpp, tex.cx,tex.cy,tex.genMode));
                            xmlw.Close();
                        }
                        xmlw.Close();
                        ++idx;
                    }//TEXTURES
                }
                xmlw.Close();
            }
        
            xmlw.Open("BRUSHES");
            {
                PBrushes* brshses = _scene.GetPrimitives();
	            FOREACH(PBrushes, (*brshses), ppb)
	            {
                    Brush* pB = (*ppb);
                    
                    pB->_name[15]=0;
                    xmlw.Open(pB->_name);
                    {
                        xmlw.WriteTag("GROUP",pB->_group);
                        xmlw.WriteTag("FLAG",pB->_flags);
                        xmlw.WriteTag("PROP",pB->_props);
                        
                        int idxMotion = _scene.GetIndex(pB->_pMotion);
                        xmlw.WriteTag("MOTION",idxMotion);
                        xmlw.WriteTag("PHISICS0",pB->_physicsA);
                        xmlw.WriteTag("PHISICS1",pB->_physicsD);

                        xmlw.Open("FACES");
                        {
                            idx = 0;
		                    FOREACH(Polys, pB->_polys, pp)
		                    {
                                xmlw.Open("FACE");
                                {
                                    xmlw.WriteTag("TEX-ID_GL", (TCHAR*)MKSTR("%d,%d",texids[pp->_textures._texts[0]],pp->_textures._texApply[0]));
                                    xmlw.WriteTag("TEX-ID_GL", (TCHAR*)MKSTR("%d,%d",texids[pp->_textures._texts[1]],pp->_textures._texApply[1]));
                                    xmlw.WriteTag("TEX-ID_GL", (TCHAR*)MKSTR("%d,%d",texids[pp->_textures._texts[2]],pp->_textures._texApply[2]));
                                    xmlw.WriteTag("TEX-ID_GL", (TCHAR*)MKSTR("%d,%d",texids[pp->_textures._texts[3]],pp->_textures._texApply[3]));
                                    xmlw.WriteTag("FLAG0", pp->_flags);
                                    xmlw.WriteTag("FLAG1",pp->_flags2);
                                    xmlw.WriteTag("PROP", pp->_props);
                                    xmlw.WriteTag("COLORA",pp->_colorD);
                                    xmlw.WriteTag("COLORS",pp->_colorS);
                                    xmlw.WriteTag("COLORD",pp->_colorD);
                                    xmlw.WriteTag("COLORE",pp->_colorE);
                                    xmlw.WriteTag("SHINE",pp->_shiness);
                                    xmlw.WriteTag("BUMP",pp->_bump);
                                    xmlw.WriteTag("FRICTION",pp->_friction);
                                    xmlw.WriteTag("USE2SPLIT",pp->_use2splitidx);

                                    xmlw.Open("VERTEXEX");
                                    {
			                            FOREACH(vvector<Vtx2>, pp->_vtci, pv)
			                            {
                                            xmlw.WriteTag("VNTC",*pv);
			                            }
                                        xmlw.Close();
                                    }
                                    xmlw.Close();
                                }//FACE
		                    }//FOR
                            xmlw.Close();
                        }//FACES
                    
                    }//BRUSH
                }//FOR
                xmlw.Close();
            }//BRUSHES

            xmlw.Open("ITEMS");
            {
                FOREACH(vvector<SceItem*>, (*GetItems()), pItemPtr)
                {
                    SceItem* pItem = *pItemPtr;
                    pItem->_name[24] = 0;

                    xmlw.Open("pItem->_name");
                    {

		                xmlw.WriteTag("TYP",pItem->_item);
                        xmlw.WriteTag("GRP",pItem->_group);
                        xmlw.WriteTag("CAT",(TCHAR*)pItem->_catname.c_str());
		                xmlw.WriteTag("ID",pItem->_id);
		                xmlw.WriteTag("PROP",pItem->_props);
		                xmlw.WriteTag("FLAG",pItem->_flags);
		                xmlw.WriteTag("COLOR",pItem->_colorD);
		                xmlw.WriteTag("POS",pItem->_t);
		                xmlw.WriteTag("SIZE",pItem->_s);
                        xmlw.WriteTag("TEX-ID_GL", (TCHAR*)MKSTR("%d,%d",texids[pItem->_textures._texts[0]],pItem->_textures._texApply[0]));
                        xmlw.WriteTag("TEX-ID_GL", (TCHAR*)MKSTR("%d,%d",texids[pItem->_textures._texts[1]],pItem->_textures._texApply[1]));
                        xmlw.WriteTag("TEX-ID_GL", (TCHAR*)MKSTR("%d,%d",texids[pItem->_textures._texts[2]],pItem->_textures._texApply[2]));
                        xmlw.WriteTag("TEX-ID_GL", (TCHAR*)MKSTR("%d,%d",texids[pItem->_textures._texts[3]],pItem->_textures._texApply[3]));
                        switch(pItem->_item)
                        {
                            case ITM_LIGTBULB:
                                 xmlw.WriteTag("DIR",((CLightBulb*)pItem)->_dir);
				                 xmlw.WriteTag("ANGLE",((CLightBulb*)pItem)->_specAngle);
				                 xmlw.WriteTag("INTENS",((CLightBulb*)pItem)->_lmIntensity);
				                 xmlw.WriteTag("RADIUS",((CLightBulb*)pItem)->_radius);
				                 xmlw.WriteTag("HALLO0",((CLightBulb*)pItem)->_halloRadMin);
				                 xmlw.WriteTag("HALLO1",((CLightBulb*)pItem)->_halloRadMax);
                                break;
                            case ITM_TRIGER:
                                {
                                    DWORD   ptr = -1;
                                    V3 ex = ((TriggerItem*)pItem)->_drawBrush._box.GetExtends();
                                    xmlw.WriteTag("TARGETPOINT",((TriggerItem*)pItem)->_efectPoint);
                                    xmlw.WriteTag("DIMS",ex);
                                }
                                break;
                        }    
                        xmlw.Close();
                    }
	            }//FOR
                xmlw.Close();    
            }//ITEMS

            xmlw.Open("MOTIONS");    
            {
                FOREACH(vvector<CMotion*>,  (*_scene.GetMotions()), ppMotion)
                {
                    CMotion* pM = *ppMotion;

                    xmlw.WriteTag("LOOP",  pM->_bLoop);
                    xmlw.WriteTag("CENTER",pM->_center);
                    xmlw.WriteTag("BRUSH", pM->_pBrush->_unicID);
                    xmlw.Open("PATH");    
                    {
                        for(int i=0;i<pM->GetNodesCount();i++)
                        {
                            MState* pS = pM->GetState(i);
    
                            xmlw.WriteTag("POS", pS->_pos); 
                            xmlw.WriteTag("EULER", pS->_dir);
                        }
                        xmlw.Close();    
                    }
                }
                xmlw.Close();    
            }
            //MOTIONS

            xmlw.Open("SCRIPTS");    
            {
	            FOREACH(vvector<ScriptItem*>,  (*_scene.GetScripts()), ppScript)
		        {
			        ScriptItem*	pSi = *ppScript;
                    xmlw.Open("SCRIPT");    
                    {
                        xmlw.Open("CONTENT");    
                        {
                            fw.Write((void*)pSi->_script.c_str(),pSi->_script.length());	
                            xmlw.Close();    
                        }
                        xmlw.Close();    
                    }
		        }
                xmlw.Close();    
            }

            
            xmlw.Close();
        }//SENE
        xmlw.Close();
    }//XML

    fw.Close();
	AfxGetApp()->DoWaitCursor(0);
}


void CZ_ed2Doc::RWCompSettings(FileWrap &fw, BOOL save)
{
    int tmpVal = SECT_COMPIESET;

    if(save)
    {
        int tmpVal = SECT_COMPIESET;
        fw.Write(tmpVal);


        fw.Write(COMP()->_bSaveFinalBSP);
        fw.Write(COMP()->_szipit);
        fw.Write(COMP()->_bSaveImages);
        fw.Write(COMP()->_maxFacesLeaf);
		fw.Write(COMP()->_bSaveCats);
        fw.Write(COMP()->_bSavePortals);
        fw.Write(COMP()->_bSortByAreea);
        fw.Write(COMP()->_bMergeNarrowLeafs);
        fw.Write(COMP()->_bSaveSkyDom);
        fw.Write(COMP()->_bLightMaps);
        fw.Write(COMP()->_b3SampleLM);
        fw.Write(COMP()->_bAvoidNonOrt);
        fw.Write(COMP()->_bUseUserPriority);
        fw.Write(COMP()->_bPvsFull);
        fw.Write(COMP()->_bZones);
        fw.Write(COMP()->_bPvs);
        fw.Write(COMP()->_removeOnePolyLeaf);
        fw.Write(COMP()->_portalCutFlag);
        fw.Write(COMP()->_balance);
        fw.Write(COMP()->_lmapMaxSize);
        fw.Write(COMP()->_lMapLumelSz);
        fw.Write(COMP()->_bLmPatch);
		fw.Write(COMP()->_bExportFile);

        fw.Write(COMP()->_bSaveBigTerrain);
        fw.Write(COMP()->_bSaveBigTerrainBSP);
        fw.Write(COMP()->_bSnapMainBsp);
        fw.Write(COMP()->_lmpowof2);
        fw.Write(COMP()->_lmaspect);
        fw.Write(COMP()->_bGeticBSP); // last
        fw.Write(tmpVal);
        fw.Write(tmpVal);

    }
    else
    {
        fw.Read(tmpVal);
        assert(tmpVal == SECT_COMPIESET);

        fw.Read(COMP()->_bSaveFinalBSP);
        fw.Read(COMP()->_szipit);
        fw.Read(COMP()->_bSaveImages);
        fw.Read(COMP()->_maxFacesLeaf);
		fw.Read(COMP()->_bSaveCats);
        fw.Read(COMP()->_bSavePortals);
        fw.Read(COMP()->_bSortByAreea);
        fw.Read(COMP()->_bMergeNarrowLeafs);
        fw.Read(COMP()->_bSaveSkyDom);
        if(i_geomfilever == DOC_VERSION2)
        {
            fw.Read(tmpVal);
            fw.Read(tmpVal);
        }
        fw.Read(COMP()->_bLightMaps);
        fw.Read(COMP()->_b3SampleLM);
        if(i_geomfilever == DOC_VERSION2)
        {
            fw.Read(tmpVal);
        }
        fw.Read(COMP()->_bAvoidNonOrt);
        fw.Read(COMP()->_bUseUserPriority);
        fw.Read(COMP()->_bPvsFull);
        fw.Read(COMP()->_bZones);
        fw.Read(COMP()->_bPvs);
        fw.Read(COMP()->_removeOnePolyLeaf);
        fw.Read(COMP()->_portalCutFlag);
        fw.Read(COMP()->_balance);
        fw.Read(COMP()->_lmapMaxSize);
        fw.Read(COMP()->_lMapLumelSz);
        fw.Read(COMP()->_bLmPatch);
		fw.Read(COMP()->_bExportFile);

        fw.Read(COMP()->_bSaveBigTerrain);
        fw.Read(COMP()->_bSaveBigTerrainBSP);
        fw.Read(COMP()->_bSnapMainBsp);
        fw.Read(COMP()->_lmpowof2);
        fw.Read(COMP()->_lmaspect);
        fw.Read(COMP()->_bGeticBSP);
        fw.Read(tmpVal);
        fw.Read(tmpVal);
    }
}
