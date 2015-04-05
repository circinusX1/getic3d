//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------
void Bsp::Recurs_Clip(int node, list<Polygon>& polys2Clip,
//--| finalList|------------------------------------------------------------------------
list<Polygon>& finalList)
{
    REAL        fTemp;
    BspNode  node = GetNode(node);
    
    if(node->IsLeaf())
    {
        if(node->IsSolid())
        {
            if(m_csgUnion)
            {
                return;
            }
            finalList << polys2Clip;
        }
        else
        {
            if(!m_csgUnion)
            {
                return;
            }
            finalList << polys2Clip;
        }
        return;
    }
    
    for_each(pSpPoly in polys2Clip)
    {
        Where_Is rp1   = pSpPoly.Classify(node->GetPlane());
        
        switch (rp1)
        {
            case ON_PLANE:
                if(SameFacing(pSpPoly,node.GetPlane()))
            {
                backPolys.push_back(pSpPoly);
            }
            else
            {
                if(m_csgUnion == FALSE)
                  frontPolys.push_back(pSpPoly);
                else
                    backPolys.push_back(pSpPoly);
            }
            break;
            case ON_FRONT:
                frontPolys.push_back(pSpPoly);
            break;
            case ON_BACK:
                backPolys.push_back(pSpPoly);
            break;
            case ON_SPLIT:
            {
                Polygon fp, bp;
                pSpPoly.Split(node->GetPlane(),fp,bp);
                frontPolys.push_back(fp);
                backPolys.push_back(bp);
            }
            break;
        }
    }
    if(backPolys.size())
      Recurs_Clip(node->BackNodeIndex(), backPolys, finalList);
    if(frontPolys.size())
      Recurs_Clip(node->FrontNodeIndex(), frontPolys, finalList);
}

