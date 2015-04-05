// PortalPRC1.cpp: implementation of the CPortalPRC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "PortalPRC1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/*
void	CBspTree::CalculatePvs()
{
	vvector<Portal>	duplist;

	BuildPortals();

	_bytesPerSet = (_nleafs + 7) / 8;
	_bytesPerSet = (_bytesPerSet * 3 + 3) & 0xFFFFFFFC;
	_pPvsData = new BYTE[(_nleafs+1) * _bytesPerSet] ;
	
	//DuplicatePortals(duplist);
	//InitialPortalVis(duplist);
	//CalculatePortalVis(duplist);

}

void    CBspTree::RenderPortals(z_ed3View* pV)
{
    Ta.Disable();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
    glColor4ubv(CLR_PORTALS);
    glDisable(GL_CULL_FACE);
    
    int ii=0;
	FOREACH(vvector<Portal>, _portals, pp)
	{
        Portal &poly = *pp;        
        {
            glColor4ubv(CLR_PORTALS);
		    glBegin(GL_POLYGON);
	    	FOREACH(vvector<Vtx2>, poly._vtci, pv)
		    {
                glVertex3f(pv->_xyz.x,pv->_xyz.y,pv->_xyz.z);

		    }
    		glEnd();
        }

        {
            glColor4ubv(CLR_PORTALSLINE);
		    glBegin(GL_LINE_LOOP);
            FOREACH(vvector<Vtx2>, poly._vtci, pv2)
		    {
                glVertex3f(pv2->_xyz.x,pv2->_xyz.y,pv2->_xyz.z);
		    }
		    glEnd();
        }


        ii++;
	}

	glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
    glDisable(GL_BLEND);
}


BOOL CBspTree::BuildPortals()
{
	int	iNode = 0;

	FOREACH(vvector<CNode*>,	_nodes, ppnode)
	{
		CNode*	pn = (*ppnode);

		if(pn->_flags & NODE_LEAF)
			continue;

       // if(iNode==5)
        {
		    Portal	            p;							// first portal
		    vvector<Portal>	    plist;				// list op portals
            _stprintf(p._name,"node %d", iNode);
            CalculateInitialPortal(p, pn);		// calc portal on node pn
		    ClipPortal(0, p, plist);
            CleanObsolettePortals(plist);
            append(_portals, plist);
        }


		iNode++;
        
	}
    int ports = _portals.size();
    CleanObsolettePortals(_portals);
    ports = _portals.size();

    
	return TRUE;
}


void	CBspTree::CleanObsolettePortals(vvector<Portal>& plist)
{

	int sz = plist.size();

	FOREACH(vvector<Portal>, plist, pport)
	{
		Portal& portal = *pport;

		int sz = portal._leafsLst.size();

		if(portal._flags & POLY_DELETED){
			pport = plist.erase(pport)-1;
			continue;
		}

		if(portal._leafsLst.size() != 2){
			pport = plist.erase(pport)-1;
			continue;
		}
	}

	FOREACH(vvector<Portal>, plist, pport2)
	{
		if(PortalHasDuplicate(pport2, plist))
		{
			pport2 = plist.erase(pport2)-1;
		}
	}


}

BOOL	CBspTree::PortalHasDuplicate(vvector<Portal>::iterator pi, vvector<Portal>& plist)
{
	Portal& pa  = *pi;
	FOREACH(vvector<Portal>, plist, pip)
	{
		if(pi == pip)
			continue;
		Portal& pb  = *pip;

		//assert(pa._leafsLst.size()==2 && pb._leafsLst.size()==2);
		if(pa._leafsLst.size()!=2 ||pb._leafsLst.size()!=2)
			continue;
	
		int pla1 = pa._leafsLst[0];
		int pla2 = pa._leafsLst[1];
		int plb1 = pb._leafsLst[0];
		int plb2 = pb._leafsLst[1];
		
		// same leafs
		if((pla1 == plb1 && pla2 == plb2) || 
		   (pla1 == plb2 && pla2 == plb1))
		{
			Box bpa = GetVxesBbox(pa._vtci);
			Box bpb = GetVxesBbox(pb._vtci);

			REAL da = RABS(bpa.GetExtends().len());
			REAL db = RABS(bpb.GetExtends().len());

			if(da <= db)
				return TRUE;
		}
	}
	return FALSE;
}
///////////////

void	CBspTree::CalculateInitialPortal(Portal& p, CNode* pn)
{
	Plane&  plane  = _planes[pn->_planeIdx];			// splitting plane
	V3&		normal = plane._n;
	Box&	bb     = pn->_bbox;							// bbox
	V3		bbCenter = bb.GetCenter();					// bbox center
	REAL	d2plan = -plane.DistTo(bbCenter);			// dist to plane


	V3		planeCenter = bbCenter + (normal * d2plan);	// plane center id bb center projeccted on plane
														// starting from this center we build the far edges
	V3		ax = GetMajorAxes(plane._n);			// get to see what wax we are using for rotation cp
    ax.norm();

	
	V3		bbEx = (bb._max - bbCenter);				// bbox extends
	V3		u = Vcp(ax, normal);						// to corner vectors;
	V3		v = Vcp(u, normal);
	u  *= bbEx;
	v  *= bbEx;
//	V3		corners[4];									// the 4 corners
	//
	// corners
	//
	V3		corners[4];									// the 4 corners

	corners[0] = planeCenter + (u-v);
	corners[1] = planeCenter + (u+v);
	corners[2] = planeCenter - (u-v);					// switch this to be able to rotate it
	corners[3] = planeCenter - (u+v);

	//
	// populate the portal structure
	//
	p._c    = plane._c;
	p._n   = normal;
	p._flags     |= POLY_PORTAL;
	p._planeIdx = pn->_planeIdx;

	Vtx2 vtinc;
	for(int i=0; i < 4; i++)
	{
		vtinc._xyz = corners[i];
		p._vtci.push_back(vtinc);
	}
}


V3	CBspTree::GetMajorAxes(V3& normal)
{
	V3	rv;
	if( RABS(normal.y) > RABS(normal.z) ) 
	{
		if( RABS(normal.z) < RABS(normal.x) ) 
		{
			rv.z = 1.f;
		} 
		else  
		{
			rv.x = 1.f;
		} 
	} 
	else 
	{
		if (RABS(normal.y )<= RABS(normal.x) ) 
		{
			rv.y = 1;
		} 
		else 
		{
			rv.x = 1;
		} 
	}
	return rv;
}


void	CBspTree::SplitPortal(Portal& portal,Plane& plan,Portal& fp,Portal& bp)
{
	Poly*	p2split = reinterpret_cast<Poly*>(&portal);
	Poly*	pf		= reinterpret_cast<Poly*>(&fp);
	Poly*	pb		= reinterpret_cast<Poly*>(&bp);

	fp.CopyProps(portal);
	bp.CopyProps(portal);
	p2split->Split(plan, *pf, *pb);
}


void	CBspTree::DuplicatePortals(vvector<Portal>& duplist)
{
	duplist.reserve(_portals.size()*2);

	FOREACH(vvector<Portal>,	_portals, pp)
	{
		// get a copy of the portal
		Portal& p = *pp;

		assert(p._leafsLst.size() == 2);
		int l0 = -p._leafsLst[0];
		int l1 = -p._leafsLst[1];
		assert(l0 > 0 && l1 > 0);

		CNode* lf0 = _nodes[l0];
		CNode* lf1 = _nodes[l1];
		
		V3	l0c = lf0->_bbox.GetCenter();
		V3	l1c = lf1->_bbox.GetCenter();

		REL_POS	rp0 = ClassifyPoint(l0c, (Plane&)p);
		REL_POS	rp1 = ClassifyPoint(l1c, (Plane&)p);

		Duplicate(duplist, p, p, lf0, lf1, rp0, rp1);

	}
	int szo = _portals.size();
	int szd = duplist.size();

	pgPortalVis = new BYTE[szd+1];
	memset(pgPortalVis, 0, szd+1);


}


void	CBspTree::Duplicate(vvector<Portal>& duplst, 
							Portal& pa, Portal pb, 
							CNode* pleaf0, CNode* pleaf1,
							REL_POS pos0, REL_POS pos1)
{
	pb.Reverse();
	//
	// fill duplicate list and retreive a reference ot these portals
	//
	int idx = duplst.size();
	duplst.push_back(pa);
	Portal& po0 = duplst.back();
	po0._idx = idx;

	idx = duplst.size();
	duplst.push_back(pb);
	Portal& po1 = duplst.back();
	po1._idx = idx;
	
	
	//
	// leaf 0 of curent portal position
	//
	switch(pos0){
		//
		// leaf 0 on fromt so lef   [leaf1] |->[leaf0] asuume . l2 owns portal
		//
		case PON_FRONT:
			// add the po to leaf1 and make portal owner pleaf1	
			// l1->potral[l0]
			pleaf1->_pports.push_back(&po0);
			po0._pNeigbourLeaf = pleaf0;
			// add the portal 1 to leaf 0 and make neighbor lef0
			// l0->portal[l1]
			pleaf0->_pports.push_back(&po1);
			po1._pNeigbourLeaf = pleaf1;
			break;

		case PON_BACK:
			pleaf0->_pports.push_back(&po0);
			po0._pNeigbourLeaf = pleaf1;

			pleaf1->_pports.push_back(&po1);
			po1._pNeigbourLeaf = pleaf0;

			break;
		case ON_PLANE:
			// check the other leaf
			switch(pos1)
			{
				case PON_FRONT:
					pleaf0->_pports.push_back(&po0);
					po0._pNeigbourLeaf = pleaf1;

					pleaf1->_pports.push_back(&po1);
					po1._pNeigbourLeaf = pleaf0;
					break;
				case PON_BACK:
					pleaf1->_pports.push_back(&po0);
					po0._pNeigbourLeaf = pleaf0;

					pleaf0->_pports.push_back(&po1);
					po1._pNeigbourLeaf = pleaf1;
					break;
				case ON_PLANE:
					assert(0);
					break;
			}
			break;
	}
}


void CBspTree::InitialPortalVis(vvector<Portal>& duplst)
{

	FOREACH(vvector<Portal>, duplst, pp1)
	{
		Portal& p1 = *pp1;

		p1._ppvs = new BYTE[_bytesPerSet];
		memset(p1._ppvs, 0, _bytesPerSet);

		_nVisCount = 0;
		memset(pgPortalVis, 0, duplst.size());
		
		FOREACH(vvector<Portal>, duplst, pp2)
		{
			if(pp2 == pp1)
			{
				continue;
			}
			Portal& p2 = *pp2;
			
			//
			// classify p2 against p1
			//
			FOREACH(vvector<Vtx2>, p2._vtci, ppct)
			{
				V3&    v2    = ppct->_xyz;
				REL_POS pos  = ClassifyPoint(v2, (Plane&)p1);
				if(PON_FRONT == pos)
					break;
			}

			//
			// all pints have been tested. p2 totally on front of p1
			//
			if(ppct == p2._vtci.end())
				continue;

			//
			// test if any p1 are behind p2 plane
			//
			FOREACH(vvector<Vtx2>, p1._vtci, ppc)
			{
				V3&    v2    = ppct->_xyz;
				REL_POS pos  = ClassifyPoint(v2, (Plane&)p2);
				if(PON_BACK == pos)
					break;
			}

			//
			// all pints have been tested. p2 totally on front of p1
			//
			if(ppct == p2._vtci.end())
				continue;

			//
			// fill out temporary visible array
			//
			pgPortalVis[pp2-duplst.begin()] = 1;
			_nVisCount ++;
		}//for p2

		gLeafVisCnt = 0;
		R_PortalFlood(p1, p1._pNeigbourLeaf);
		p1._numPossible = gLeafVisCnt;
	}//for p1
}

void	CBspTree::R_PortalFlood(Portal& sourceP, Leaf* pLeaf)
{
	if(GetPvsBit(sourceP._ppvs, -(pLeaf->_idxThis)))
		return;								// already is set
	SetPvsBit(sourceP._ppvs, -(pLeaf->_idxThis));
	gLeafVisCnt++;
	
	//
	// for each leaf's portal
	//
	FOREACH(vvector<Portal*>, pLeaf->_pports, pport)
	{
		Portal* pp =*pport;

		if(pgPortalVis[pp->_idx])
			continue;
		R_PortalFlood(sourceP, (*pport)->_pNeigbourLeaf);
	}
}


void CBspTree::CalculatePortalVis(vvector<Portal>& duplst)
{
	//_pactualpvs = p._pactualpvs;
	//_numActual
	if(_fastPVSCompile)
	{
		FOREACH(vvector<Portal>, duplst, pp)
		{
			Portal& p = *pp;
			p._pactualpvs = p._ppvs;
			p._numActual  = p._numPossible;
			p._ppvs = 0;
		}
		return;
	}

//	PVSRECURSEDATA	data;
}


void	CBspTree::ClipPortal(int nodeIdx, Portal&  portal, vvector<Portal>& portals)
{
	CNode*  pNode   = _nodes[nodeIdx];
	CNode*  pFront  = _nodes[pNode->_iNodes[N_FRONT]];
	CNode*  pBack   = _nodes[pNode->_iNodes[N_BACK]];
    Plane&  plan    = _planes[pNode->_planeIdx];
    REL_POS rp1     = portal.Classify(plan);

    switch (rp1)
	{
		case ON_PLANE:
            {
                vvector<Portal>   fps; //temp

                if(pFront->IsLeaf()) // send on front to be clipped 
                {
                    portal._leafsLst.push_back(pFront->_idxThis);
                    fps.push_back(portal);
                }
                else
                {
                    ClipPortal(pNode->_iNodes[N_FRONT], portal, fps);
                }
                    
                if(fps.size())  // what have survived on front send on back
                {
                    if(pBack->IsLeaf())
                    {
                        append(portals, fps);
                        return;
                    }

                    FOREACH(vvector<Portal>, fps, rp)
				    {	
					    Portal& port = *rp;
					    ClipPortal(pNode->_iNodes[N_BACK], *rp, portals);
				    }
                }
            }
			break;
        case PON_FRONT:
            if(pFront->IsNode())
            {
                ClipPortal(pNode->_iNodes[N_FRONT], portal, portals);
            }
            else
            {
                portal._leafsLst.push_back(pFront->_idxThis);
                portals.push_back(portal);
            }
			break;
        case PON_BACK:
            if(pBack->IsNode())
            {
			    ClipPortal(pNode->_iNodes[N_BACK], portal, portals);
            }
			break;
        case ON_SPLIT:
            {
                Portal fp;
                Portal bp;

                fp.CopyProps(portal);
                bp.CopyProps(portal);
                portal.Split(plan,fp,bp);
                portal._flags |= POLY_DELETED;

                if(pFront->IsNode())
                {
                    ClipPortal(pNode->_iNodes[N_FRONT], fp, portals);
                }
                else
                {
                    fp._leafsLst.push_back(pFront->_idxThis);
					portals.push_back(fp);
                }
                if(pBack->IsNode())
                {
                    ClipPortal(pNode->_iNodes[N_BACK],  bp, portals);
                }
            }
			break;
    } 
    return;
    
}

*/