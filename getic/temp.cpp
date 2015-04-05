//---------------------------------------------------------------------------------------
void FindToucheingLeafs()
{
    for_each(leaf in leafs)
    {
        for_each(leaf2 in leafs)
        {
            if(leaf==leaf2)
                continue;
            if(leaf.box.Touches(leaf2.box))
            {
                Node node     = FindCommonParent(leaf, leaf2);
                Portal portal = CalculateInitialPortal(node);
                ClipWithLeafSides(leaf, leaf2, portal);

                portal.AddSideLeaf(leaf);
                portal.AddSideLeaf(leaf2);
                BSP_tree.Portals.Add(portal);
            }
        }
    }
}

Node FindCommonParent(Leaf& a, Leaf& b)
{
    return R_FindCommonParent(a, a.ParentNode(), b);
}

Node R_FindCommonParent(Node a, Node aUpNode, Node b)
{
    if(a == aUpNode.Front())
        a = aUpNode.BackNode();
    else
        a = aUpNode.FrontNode();

    if(R_SearchLeaf(a, b))
    {
        return a;
    }
    return R_FindCommonParent(aUpNode, aUpNode.ParentNode());
}


BOOL    R_SearchLeaf(Node a, Node b)
{
    if(a.IsLeaf())
    {
        return (a == b);
    }
    if(R_SearchLeaf(a.FrontNode(), b))
        return TRUE;
    if(R_SearchLeaf(a.BackNode(), b))
        return TRUE;
    return FALSE;
}



Portal CalculateInitialPortal(Node node)
{
    Plane&  plane  = node.GetPlane();
    V3&		normal = plane._n;
    Box	bb         = node.Box();

    //
    // project the node bbox center on splitting plane
    // that will be the center of out bogus polygon
    //
    V3		bbCenter = bb.GetCenter();			
    REAL	d2plan   = -plane.DistTo(bbCenter);	
    V3		portOrig = bbCenter + (normal * d2plan);
    V3		ax       = GetMajorAxes(plane._n);
    ax.norm();
    
    //
    // build a polygon from plane
    //
    V3		bbEx = (bb._max - bbCenter);				
    V3		uDr  = Vcp(ax, normal);						
    V3		vDr  = Vcp(uDr, normal);
    
    //
    // calc extends
    //
    uDr		*= bbEx.getmax() ;
    vDr		*= bbEx.getmax() ;
    REAL vdp = Vdp(normal, ax);

    //
    // reserve ans calc the corner vertexes of the portal
    //
    V3		corners[4];									
    corners[0] = portOrig + (uDr-vDr);
    corners[1] = portOrig + (uDr+vDr);
    corners[2] = portOrig - (uDr-vDr);					
    corners[3] = portOrig - (uDr+vDr);

    return portal << (corners);
}


void	ClipWithLeafSides(Leaf leaf, Portal& portal)
{
    Uniqueset    sides;
    Portal       temp(portal), backfragment;
    
    for_each(poly in leaf.polys)
    {
        if(sides.has(poly.GetPlane()))
            continue;           // we cut with this plane
        sides << poly.GetPlane();

        portal.Split(poly.GetPlane(), temp, backfragment);
        portal = temp;          // keep cutting
        temp.Clear();
    }
    // here the &portal is clean
}
