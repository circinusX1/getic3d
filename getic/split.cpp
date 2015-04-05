void    Face::Split(Plane& plane, Face& frontFace, 
					Face& backFace)
{
	Vertex vertex1	= m_points.first();
	Vertex vertex2	= m_points.back();
	float  fB;
	float  fA		= plane.DistTo(vertex2._xyz);
	
	for_each(vertex in m_poins)
	{
		vertex1 = *vertex;
		fB      = plane.DistTo(vertex1._xyz);
		if(fB > EPSILON)
		{
			if(fA < -EPSILON)
			{
				float   t = -fA /(fB - fA);
				Vertex midvertex = vertex1 + 
								   (vertex2-vertex1)*t;
				frontFace << midvertex;
				backFace << midvertex;
			}
			frontFace<<vertex1;
		}
		else if(fB < -EPSILON)
		{
			if(fA > EPSILON)
			{
				float t = -fA /(fB - fA); 
				Vertex midvertex = vertex1 + 
								   (vertex2-vertex1)*t;
				frontFace<<midvertex;
				backFace <<midvertex;
			}
			backFace <<vertex1;
		}
		else
		{
			frontFace << vertex1;
			backFace << vertex1;
		}
		vertex2 = vertex1;
		fA   = fB;
	}
}
