
// variables
bool numSubs = 1;
bool renderQuit = FALSE;
char *outFile = "out.iv";
char *inFile = "scene.iv";
enum subdTypes { CATMULL_CLARK };
enum vertType { ORIGINAL, NEWEDGE, NEWFACE, MOVED };
subdTypes subdType = CATMULL_CLARK;
SoSeparator *scene = NULL;
SoXtExaminerViewer *myViewer;
SoSwitch *wireFrameSwitch;
SoSwitch *shapeSwitch;

struct SdFace;


// tclamp definition
// restricts a number to a range [min, max]
template <class T> inline
T tclamp(T x, T min, T max){
  if (x<min)
    return(min);
  else if (x>max)
    return(max);
  else
    return(x);
}

typedef struct SdEdge {
  int32_t vert;
  int32_t orig;   // debugging
  SdEdge *pair;
  SdFace *face;
  SdEdge *next;
} SdEdge;

typedef struct SdFace {
  SdEdge *edge;
} SdFace;


//WW''W''''''WWW''''WW'WWWWW'''WW'''''''''''WW''''WW''''''''WW''''''''''WWW''
//WW  W       WW   WW  WW      WW           WW              WW           WW  
//WW  W  WWW  WW  WWWW WWWW   WWW  WWW  WW  WW    WW  WWWW WWWW     WWW  WW  
//WWWWW W WW  WW   WW  WW    W WW W WW WWWW WW    WW WWW    WW     WW W  WW  
//WW  W W WW  WW   WW  WW    W WW W WW WW   WW    WW   WWW  WW     WW    WW  
//WW  W  WWW WWWW  WW  WWWWW  WWW  WWW  WWW WWWWW WW WWWW   WW      WWW WWWW 
//                                  WW                                       
//''''''''''''''''''''''''''''''''WWW''''''''''''''''''''''''''''''''''''''''
//
class HalfEdgeList {

  
private:
  list <SdEdge *> l_edges;
  list <SdFace *> l_faces;
  SoIndexedShape     *p_shape;
  SoIndexedFaceSet *p_shapeSet;
  SoMFVec3f *points, *v_points;
  vector <vertType> v_vtype;
  
public:

  //WW''''''WW''WW'
  //            WW 
  //WW WWW  WW WWWW
  //WW WW W WW  WW 
  //WW WW W WW  WW 
  //WW WW W WW  WW 
  //               
  //'''''''''''''''
  //
  void init(SoIndexedShape *shape_, SoMFVec3f *points_,
	    SoIndexedFaceSet *faceSet_, SoMFVec3f *points2_){
    p_shape = shape_;
    points = points_;
    p_shapeSet = faceSet_;
    v_points = points2_;

    // initialize vertex types
    //for (int i=0; i<v_points->getNum(); i++)
    //  v_vtype[i] = ORIGINAL;
    v_vtype.clear();
    v_vtype.resize(v_points->getNum(), ORIGINAL);
  }


  //WWW'''''''''''''''WW''WWW''WW'''''''''''''''''
  // WW               WW WW  W WW                 
  // WW   WW    WWW  WWW  WW   WWW   WWW WWW   WW 
  // WW  WW W  W WW W WW   WW  WW W W WW WW W WWWW
  // WW  WW W  W WW W WW W  WW WW W W WW WW W WW  
  //WWWW  WW    WWW  WWW  WWW  WW W  WWW WWW   WWW
  //                                     WW       
  //'''''''''''''''''''''''''''''''''''''WW'''''''
  //
  // Loads a single faceset into the data structure.
  // The strategy is to loop over l_faces, inserting half l_edges
  // for each edge, which are linked together, including a link to a new face.
  // For convenience, this new face is itself stored in a member list.
  // Additionally, the half l_edges are stored in lists
  // at each vertex. Then these new half l_edges are looped over again
  // to compare with the lists at each vertex to identify half-edge pairs.
  // Identified pairs are then removed from the vertex's edge list.
  // In this way each half edge is linked with it's next half edge,
  // a face, a pair, and of course a single vertex.
  //
  int loadShape(){

    int numV = v_points->getNum();
    int numI = p_shape->coordIndex.getNum();
    
    // create half-edge list for each vertex
    list <SdEdge *> vertEdges[numV];

    // state information
    int numPolys = 0;
    int numEdges = 0;
    SdEdge *firstEdge = NULL;
    SdEdge *prevEdge = NULL;

    // indexed face sets
    if (p_shape->isOfType(SoIndexedFaceSet::getClassTypeId())){

      // loop over polygons
      int j = 0;
      while (j<numI){
      
	if (p_shape->coordIndex[j] < 0){
	  j++;
	  continue;
	}

	// create new face
	SdFace *face = new SdFace;
	l_faces.push_back(face);
	numPolys++;
	firstEdge = NULL;

	// since polygon is closed, there is an edge for each vertex
	// loop over vertices, creating edge for each    
	while (1){

	  int v = p_shape->coordIndex[j];
	
	  // edge found
	  numEdges++;
	  if (numEdges%1000==0)
	    printf("\r%d%    ", numEdges);
	
	  // create half edge
	  SdEdge *edge = new SdEdge;
	  edge->vert = v;
	  //edge->type = ORIGINAL;
	  edge->pair = NULL;
	  edge->face = face;
	
	  // store first edge
	  if (firstEdge == NULL){
	    firstEdge = edge;
	    face->edge = edge;
	  }
	  else {
	    // point last edge to this one
	    prevEdge->next = edge;

	    // debugging
	    edge->orig = prevEdge->vert;
	  }

	  // save ptr to this edge
	  prevEdge = edge;
	
	  j++;

	  // check for end of polygon
	  if (j>=numI || p_shape->coordIndex[j]<0){

	    // close loop
	    edge->next = firstEdge;               // fill in next

	    // debugging
	    firstEdge->orig = edge->vert;

	    // make second pass to connect pairs
	    // this is done by looping over vertex pairs (found by successive
	    // pairs of half l_edges), and consuling the 2nd vertex's
	    // edge list for an edge going back to the 1st vertex

	    // loop over l_edges
	    edge = firstEdge;
	    do {

	      // identify the two vertices
	      int32_t v[2];
	      v[0] = edge->vert;
	      v[1] = edge->next->vert;

	      // search v[1]'s edge list for edge going to v[0]
	      // assign half edge as pair
	      list <SdEdge *>::iterator p;	    
	      for (p=vertEdges[v[1]].begin();
		   p != vertEdges[v[1]].end(); p++){
	      
		SdEdge *edge2 = *p;

		// found edge?
		if (edge2->vert == v[0]){

		  // link l_edges
		  edge->next->pair = edge2;
		  edge2->pair = edge->next;

		  // remove edge and stop
		  vertEdges[v[1]].erase(p);
		  break;
		}
	      }

	      // if not found, and not paired, add edge to vert's list
	      if (p == vertEdges[v[1]].end() && edge->next->pair == NULL)
		vertEdges[v[0]].push_back(edge->next);

	      // proceed to next edge
	      edge = edge->next;
	    
	    } while (edge != firstEdge);  // end edge loop
	  
	    break; // stop vert looping for this polygon
	  }

	}

      } // end poly loop

    }
    // indexed triangle strip sets
    else if (p_shape->isOfType(SoIndexedTriangleStripSet::getClassTypeId())){
      
    }
    else {
      cerr << "Unsupported indexed p_shape: "
	   << p_shape->getTypeId().getName().getString()
	   << "\n";
    }


    // check for unfound half-edge's
    int unpairedEdges = 0;
    int firstTime = 1;
    for (int i=0; i<numV; i++){

      unpairedEdges += vertEdges[i].size();

      if (unpairedEdges && firstTime){
	cerr << "Unpaired vertices: \n";
	firstTime = 0;
      }

      if (vertEdges[i].size()){
	
	cerr << "Vert " << i << ": ";

	// loop over unapred l_edges
	for (list <SdEdge *>::iterator p=vertEdges[i].begin();
	     p!=vertEdges[i].end(); ++p){

	  SdEdge *edge = *p;
	  
	  cerr << edge->vert << " ";

	  // debugging
	  show((*points)[edge->orig], (*points)[edge->vert]);
	  
	}
	cerr << "\n";
      }
    }


    //printFaces();
    
    // Euler's formula: F+V-E=2
    // E.g., a cube:    6+8-12=2 
    cerr << numPolys << " l_faces, "
	 << numV     << " verts, "
         << numEdges << " half l_edges ("
	 << numEdges/2 << " l_edges).\n";
    cerr << "Euler characteristic = "
	 << numPolys+numV - numEdges/2 << ".\n";
    cerr << unpairedEdges << " unpaired half-l_edges.\n";
    cerr << "Done loading face set.\n";

    return(1);
  }


  //''''''''''WW'''''''WW''WWWWW'''''''''''''''''''''
  //                   WW  WW                        
  //WWW  W WW WW WWW  WWWW WWWW   WWW  WWW  WW   WWWW
  //WW W WW   WW WW W  WW  WW    W WW WW W WWWW WWW  
  //WW W WW   WW WW W  WW  WW    W WW WW   WW     WWW
  //WWW  WW   WW WW W  WW  WW     WWW  WWW  WWW WWWW 
  //WW                                               
  //WW'''''''''''''''''''''''''''''''''''''''''''''''
  //
  void printFaces(){

    cerr << "Faces: " << l_faces.size() << "\n";

    // loop over l_faces
    for (list <SdFace *>::iterator p=l_faces.begin(); p != l_faces.end(); p++){

      // loop over l_edges
      SdFace *face = *p;
      SdEdge *edge = face->edge;
      cerr << "Face: " << face->edge << "\n";
      do {

	cerr << "Edge: "
	     << edge->vert << " "
	     << edge->face << " "
	     << edge->pair << " "
	     << edge->next << "\n";

	edge = edge->next;
      } while (edge != face->edge);
      
    }
  }


  //'''''''''''WW'''''WW'WW''''''WW'''WW'''''
  //           WW     WW              WW     
  // WWWW W WW WWW   WWW WW WW W WW  WWW  WW 
  //WWW   W WW WW W W WW WW WW W WW W WW WWWW
  //  WWW W WW WW W W WW WW WW W WW W WW WW  
  //WWWW   WWW WWW   WWW WW  WW  WW  WWW  WWW
  //                                         
  //'''''''''''''''''''''''''''''''''''''''''
  //
  void subdivide(){

    // 1. For each face, split l_edges
    // 2.                create new l_edges to new face vert
    // 3.                connect sub face l_edges.
    
    // loop over l_faces
    list <SdFace *>::iterator firstFace=l_faces.begin();
    list <SdFace *>::iterator lastFace=l_faces.end();
    for (list <SdFace *>::iterator p=firstFace; p!=lastFace; p++){

      // loop over l_edges
      SdFace *face = *p;
      SdEdge *edge = face->edge;

      do {

	// split edge if not already split
	if (v_vtype[edge->vert] != NEWEDGE){

	  // create new edge point
	  int newEdgeVert = v_points->getNum();
	  SbVec3f newEdgePos = ((*v_points)[edge->vert]+(*v_points)[edge->pair->vert])/2.0;
	  v_points->set1Value(newEdgeVert, newEdgePos);
	  //v_vtype[newEdgeVert] = NEWEDGE;
	  v_vtype.push_back(NEWEDGE);

	  //
	  //     -------->0    pair
	  //    0<--------     edge
	  //
	  //         |
	  //         V
	  // 
	  //     old  new
	  //     --->O--->O    pair
	  //    0<---O<---     edge
	  //      new  old
	  //
	  
	  // point new edge at current edge vert, and current edge at new vert
	  SdEdge *newEdge = new SdEdge;
	  //newEdge->type = ORIGINAL;
	  newEdge->vert = edge->vert;
	  newEdge->face = face;
	  newEdge->pair = edge->pair;
	  newEdge->next = edge->next;

	  SdEdge *newEdgePair = new SdEdge;
	  //newEdgePair->type = ORIGINAL;
	  newEdgePair->vert = edge->pair->vert;
	  newEdgePair->face = edge->pair->face;
	  newEdgePair->pair = edge;
	  newEdgePair->next = edge->pair->next;

	  //edge->pair->type = NEWEDGE;
	  edge->pair->vert = newEdgeVert;
	  edge->pair->next = newEdgePair;
	  edge->pair->pair = newEdge;
	  
	  //edge->type = NEWEDGE;
	  edge->vert = newEdgeVert;
	  edge->next = newEdge;
	  edge->pair = newEdgePair;

	}

	// advance edge pointer to next side
	edge = edge->next->next;
	
      } while (edge != face->edge);

      
      // create new face point
      int newFaceVert = v_points->getNum();
      int numFaceVerts = 0;
      SbVec3f newFacePos(0.0, 0.0, 0.0);
      v_points->set1Value(newFaceVert, newFacePos);
      //v_vtype[newFaceVert] = NEWFACE;
      v_vtype.push_back(NEWFACE);
      
      // loop around face again, creating new l_faces
      edge = face->edge;
      do {

	SdEdge *nextEdge = edge->next;
	
	if (v_vtype[edge->vert] == NEWEDGE){

	  SdFace *newFace = new SdFace;

	  newFacePos += (*v_points)[edge->next->vert];
	  numFaceVerts++;

          //       edge
	  //  O<--O<--O
	  //  |out^|in^
	  //  v   |v  |
	  //  O   ()  O
	  //  |       ^
	  //  v       |
	  //  O-->O-->O
	  
	  
	  // create two new l_edges
	  SdEdge *newEdgeIn = new SdEdge;
	  SdEdge *newEdgeOut = new SdEdge;
	  
	  //newEdgeIn->type = NEWFACE;
	  newEdgeIn->vert = newFaceVert;
	  newEdgeIn->face = edge->next->face;
	  newEdgeIn->pair = newEdgeOut;
	  newEdgeIn->next = NULL;    // filled-in in next pass

	  //newEdgeOut->type = NEWEDGE;
	  newEdgeOut->vert = edge->vert;
	  newEdgeOut->face = newFace;
	  newEdgeOut->pair = newEdgeIn;
	  newEdgeOut->next = edge->next;

	  // configure face
	  newFace->edge = newEdgeOut;
	  l_faces.push_front(newFace);

	  newEdgeOut->next->face = newFace;
	  newEdgeOut->next->next->face = newFace;
	  newEdgeOut->next->next->next->face = newFace;

	  // splice new l_edges into edge/edge->next
	  edge->next = newEdgeIn;

	}
	
	edge = nextEdge;
      } while (edge != face->edge);

      // loop around face again, filling-in newEdgeIn's next field
      edge = face->edge;
      do {

	edge->next->pair->next->next->next->next = edge->next->pair;
	
	edge = edge->next->pair->next->next;
      } while (edge != face->edge);

      // remove old face
      delete face;

      // set face position
      newFacePos /= numFaceVerts;
      v_points->set1Value(newFaceVert, newFacePos);
    }

    // remove l_faces
    l_faces.erase(firstFace, lastFace);
    
  }


  //'''''''''''WW'''''''''''''''WWW''WWW'''WWW''WWW''''''''''''WW'''
  //           WW                WW   WW  WW  W  WW            WW   
  // WWW  WWW WWWW WWWW W  W WW  WW   WW  WW     WW   WWW W WW WW W 
  //WW W W WW  WW  WW WW W W WW  WW   WW  WW     WW  W WW WW   WWW  
  //WW   W WW  WW  WW WW W W WW  WW   WW  WW  W  WW  W WW WW   WW W 
  // WWW  WWW  WW  WW WW W  WWW WWWW WWWW  WWW  WWWW  WWW WW   WW  W
  //                                                                
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  //
  void catmullClark(){

    // copy v_points
    SoMFVec3f *points3 = new SoMFVec3f();
    points3->setValues(0, v_points->getNum(), v_points->getValues(0));    
    
    // loop over l_faces
    for (list <SdFace *>::iterator p=l_faces.begin(); p!=l_faces.end(); p++){

      // loop over l_edges
      SdFace *face = *p;
      SdEdge *edge = face->edge;

      do {

	if (v_vtype[edge->vert] == ORIGINAL){

	  // calculate edge and face averages
	  SbVec3f faceAvgPos(0.0, 0.0, 0.0);
	  SbVec3f edgeAvgPos(0.0, 0.0, 0.0);
	  int l_edges = 0;

	  // loop over l_edges around original vert
	  SdEdge *newEdge = edge->pair;
	  do {

	    if (v_vtype[newEdge->vert] == NEWEDGE){
	      SbVec3f newPos =
		(*points3)[newEdge->pair->vert] +
		(*points3)[newEdge->next->vert] +
		(*points3)[newEdge->next->pair->next->vert] +
		(*points3)[newEdge->next->pair->next->pair->next->vert];
	      newPos /= 4.0;
	      
	      v_points->set1Value(newEdge->vert, newPos);	      

	      // mark as moved in all half l_edges pointing to it
	      v_vtype[newEdge->vert] = MOVED;
	    }


	    // track edge position average
	    edgeAvgPos += (*points3)[newEdge->vert];
	    
	    // track face position average
	    faceAvgPos += (*points3)[newEdge->next->vert];

	    l_edges++;
	    
	    newEdge = newEdge->pair->next;
	  } while (newEdge != edge->pair);

	  edgeAvgPos /= l_edges;
	  faceAvgPos /= l_edges;
	  
	  // move original vert
	  SbVec3f newOrigPos = (faceAvgPos + 2.0*edgeAvgPos +
				(l_edges-3)*(*v_points)[edge->vert])/l_edges;
	  v_points->set1Value(edge->vert, newOrigPos);
	  v_vtype[edge->vert] = MOVED;
	}

	edge = edge->next;
      } while (edge != face->edge);
    
    }
    

    // free memory
    delete points3;
    
  }

  SoIndexedFaceSet* getFaceSet(){

    // read half l_edges into face set
    p_shapeSet->coordIndex.setNum(0);

    // loop over l_faces
    int i = 0;
    for (list <SdFace *>::iterator p=l_faces.begin(); p != l_faces.end(); p++){

      // loop over l_edges
      SdEdge *edge = (*p)->edge;
      do {

	p_shapeSet->coordIndex.set1Value(i++, edge->vert);
	v_vtype[edge->vert] = ORIGINAL;
	edge = edge->next;
	
      } while (edge != (*p)->edge);

      p_shapeSet->coordIndex.set1Value(i++, -1);
    }
    
    return(p_shapeSet);
  }

  // display edge
  void show(SbVec3f v0, SbVec3f v1){

    int clear = 0;
    SbColor color = SbColor(1.0, 0.0, 0.0);
    SbVec3f dir = v1-v0;
    float height = dir.normalize();
    
    static double hue = 0.0;
    double red, green, blue;

    // height stuff
    double cb_rat = 0.05;
    double cone_height = cb_rat*height;
    double bar_height = height-cone_height;
    double bar_radius = bar_height/100.0;
    double cone_radius = bar_height/30.0;

    // add self to scene graph
    //pthread_mutex_lock(&lock_show);
    static SoSeparator *showRaySep = NULL;
    if (showRaySep == NULL){
      // add cube sep to scene
      showRaySep = new SoSeparator();
      showRaySep->addChild(new SoResetTransform());
      SoComplexity *comp = new SoComplexity();
      comp->value.setValue(0.3);
      showRaySep->addChild(comp);
      scene->addChild(showRaySep);
    }
    else if (clear){
      showRaySep->removeAllChildren();
      SoComplexity *comp = new SoComplexity();
      comp->value.setValue(0.3);
      showRaySep->addChild(comp);

      //pthread_mutex_unlock(&lock_show);
      return;
    }
    //pthread_mutex_unlock(&lock_show);
  
    // Ray debugging
    SoSeparator *sep = new SoSeparator();
    SoTransform *trn;
    SoMaterial *mat = new SoMaterial();
    mat->ambientColor.setValue(0.0, 0.0, 0.0);
    mat->diffuseColor.setValue(0.0, 0.0, 0.0);
    mat->specularColor.setValue(1.0, 1.0, 1.0);
    mat->shininess.setValue(0.3);
    mat->transparency.setValue(0.5);

    if (color[0]<0.0){
      //printCol(color);

      //pthread_mutex_lock(&lock_show);
      hue += (1.0-hue)*0.05;
      mat->emissiveColor.setHSVValue(hue, 1.0, 0.8);
      //pthread_mutex_unlock(&lock_show);
    }
    else {
      for (int i=0; i<3; i++)
//	color[i] = 1.0-exp(-2.0*color[i]/10.0);
      color[i] = tclamp(color[i], 0.0f, 1.0f);
      //mat->diffuseColor.setValue(color);
      mat->emissiveColor.setValue(color);
    }
    sep->addChild(mat);
    
    // Translate base
    trn = new SoTransform();
    trn->translation.setValue(v0);
    sep->addChild(trn);

  
    // Scale
    trn = new SoTransform();

    // Rotate
    SbRotation sbr;
    sbr.setValue(SbVec3f(0.0, 1.0, 0.0), dir);
    trn->rotation.setValue(sbr);
    sep->addChild(trn);

    // Pole
    SoCylinder *cyl = new SoCylinder();
    cyl->radius.setValue(bar_radius);
    cyl->height.setValue(bar_height);
    trn = new SoTransform();
    trn->translation.setValue(SbVec3f(0.0, bar_height/2.0, 0.0));
    sep->addChild(trn);
    sep->addChild(cyl);
  
    // Cone
    SoCone *con = new SoCone();
    con->bottomRadius.setValue(cone_radius);
    con->height.setValue(cone_height);
    trn = new SoTransform();
    trn->translation.setValue(SbVec3f(0.0, height/2.0, 0.0));
    sep->addChild(trn);
    sep->addChild(con);
  
    //pthread_mutex_lock(&lock_show);
    showRaySep->addChild(sep);
    //pthread_mutex_unlock(&lock_show);
  }

  
};

HalfEdgeList HE_list;


//WW''''''''''''''WWWWW'''''''''''''''''WW''WW''W'''''''''''''WW'WWW'''''''
//WW              WW                    WW  WW  W             WW  WW       
//WW W   WW  W WW WWWW  WW W  WW  WWW  WWWW WW  W  WWW WWW   WWW  WW   WW  
//WWW   WWWW W WW WW    WW W WWWW WW W  WW  WWWWW W WW WW W W WW  WW  WWWW 
//WW W  WW   W WW WW    WW W WW   WW W  WW  WW  W W WW WW W W WW  WW  WW   
//WW  W  WWW  WWW WWWWW  WW   WWW WW W  WW  WW  W  WWW WW W  WWW WWWW  WWW 
//             WW                                                          
//'''''''''''WWW'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//
void keyEventHandler(void *gvp, SoEventCallback *callBack){
  const SoEvent *event = callBack->getEvent();

  // hide wireframe
  if (SO_KEY_PRESS_EVENT(event, H)){
    if (shapeSwitch->whichChild.getValue() == 0){
      cerr << "Hiding p_shape.\n";
      shapeSwitch->whichChild.setValue(-1);
    }
    else {
      cerr << "Showing p_shape.\n";
      shapeSwitch->whichChild.setValue(0);
    }
  }
  
  // subdivide
  if (SO_KEY_PRESS_EVENT(event, S)){
    cerr << "Subdividing.\n";
    wireFrameSwitch->whichChild.setValue(0);
    HE_list.subdivide();
    //HE_list.printFaces();
    HE_list.getFaceSet();
  }

  // refine
  if (SO_KEY_PRESS_EVENT(event, R)){
    cerr << "Refining.\n";
    wireFrameSwitch->whichChild.setValue(0);
    HE_list.subdivide();
    HE_list.catmullClark();
    HE_list.getFaceSet();
  }

  // Save scene to file
  if (SO_KEY_PRESS_EVENT(event, W)){

    SoOutput *out = new SoOutput();
    out->openFile(outFile);
    SoWriteAction *wa = new SoWriteAction(out);
    wa->apply(scene);
    out->closeFile();
    cerr << "Dumped scene graph to " << outFile << ".\n";
    delete out;
    delete wa;
  }
  
}

//'''''''''''''WW'''''
//                    
//WWWW W   WWW WW WWW 
//WW WW W W WW WW WW W
//WW WW W W WW WW WW W
//WW WW W  WWW WW WW W
//                    
//''''''''''''''''''''
//
int main(int argc, char *argv[]){

  // read options
  while (1){
    char c = getopt(argc, argv, "c:hld:s:o:r");

    if (c == -1)
      break;

    switch (c){

    case 'h':
       cerr << "Usage: " << argv[0]
	 << " [-h] [-o out.iv] [-nN] [-r] [-t] [scene.iv]\n"
	 << "\n"
	 << "Loads .iv scene file and subdivides it.\n"
         << "Quads in SoIndexedShapes only! for right now.\n"
	 << ""
	 << "\n"
	 << " -h              this help message\n"
	 << " -o file         name of output rgb file (out.iv)\n"
	 << " -nN             use N subdivisions (1)\n"
	 << " -r              render straight to file without viewer.\n"
	 << " -t TYPE         use TYPE subdivision method (catmullClark)\n";
	 
      exit(1);
      break;

    case 'o':
      outFile = optarg;
      break;
      
    case 'n':
      numSubs = atoi(optarg);
      break;

     case 'r':
       renderQuit = TRUE;
       break;

    case 't':
      if (strcmp(optarg, "catmullClark"))
	subdType = CATMULL_CLARK;
      break;
       
     default:
       break;
    }
  }

  // read input filename
  if (optind<argc){
    inFile = argv[optind++];
  }
  
  while (optind<argc){
    cerr << "Unknown option: " << argv[optind++] << "\n";
  }
  
  // initialize inventor
  if (renderQuit)
    SoDB::init();
  else
    SoXt::init(argv[0]);

  // open scene
  SoInput *sceneIn = new SoInput();
  if (!sceneIn->openFile(inFile)){
    cerr << "Unable to open scene file: " << inFile << endl;
    exit(1);
  }

  // read scene
  scene = SoDB::readAll(sceneIn);
  if (!scene){
    cerr << "Error: scene separator is null!" << endl;
    exit(1);
  }
  scene->ref();   // ref count is initially zero (why I dunno...)
  delete sceneIn;

  // Search for indexed shapes in scene graph
  SoSearchAction sa;
  sa.setType(SoIndexedShape::getClassTypeId());
  sa.setInterest(SoSearchAction::ALL);
  sa.apply(scene);
  SoPathList plist = sa.getPaths();
  int len = plist.getLength();

  if (len<=0){
    cerr << "Could not find SoIndexedShape in " << inFile << "\n";
    exit(1);
  }


  // find face set
  SoPath *path = (SoPath*) plist[0];
  SoIndexedShape *p_shape = (SoIndexedShape *)path->getTail();
  SoGroup *shapeParent = (SoGroup *)path->getNodeFromTail(1);

  // put in group
  shapeSwitch = new SoSwitch();
  shapeSwitch->addChild(p_shape);
  shapeSwitch->whichChild.setValue(0);
  shapeParent->replaceChild(p_shape, shapeSwitch);
  
  // insert wireframe style and base light model
  wireFrameSwitch = new SoSwitch();
  SoGroup *wireFrameGroup = new SoGroup();
  SoLightModel *baseModel = new SoLightModel();
  baseModel->model.setValue(SoLightModel::BASE_COLOR);
  wireFrameGroup->addChild(baseModel);
  SoDrawStyle *wireFrameStyle = new SoDrawStyle();
  wireFrameStyle->style.setValue(SoDrawStyle::LINES);
  wireFrameGroup->addChild(wireFrameStyle);
  wireFrameSwitch->addChild(wireFrameGroup);
  int shapeIdx = shapeParent->findChild(shapeSwitch);
  shapeParent->insertChild(wireFrameSwitch, shapeIdx++);

  // insert extra light model and draw style
  shapeParent->insertChild(new SoLightModel(), shapeIdx+1);
  shapeParent->insertChild(new SoDrawStyle(), shapeIdx+2);

  
  // find vertex list
  SoMFVec3f *points = NULL;
  
  // check for vertex property
  if (p_shape->vertexProperty.getValue() == NULL){

    // search for Coordinate3 node
    cerr << "Searching for Coordinate3 node...";
    SoSearchAction sa2;
    sa2.setType(SoCoordinate3::getClassTypeId());
    sa2.setSearchingAll(FALSE); // don't look under off switches
    sa2.setInterest(SoSearchAction::LAST);
    sa2.apply(path);

    SoPath *coordPath = sa2.getPath();
    if (coordPath == NULL){
      cerr << "Could not find Coordinate3 node!\n";
      exit(2);
    }
    else
      cerr << "found.\n";
      
    SoCoordinate3 *coord = (SoCoordinate3 *)coordPath->getTail();
    points = &(coord->point);
    
  }
  else {
    SoVertexProperty *vProp = (SoVertexProperty *)p_shape->vertexProperty.getValue();
    points = &(vProp->vertex);
  }

  // insert secondary face set
  SoIndexedFaceSet *p_shapeSet = new SoIndexedFaceSet();
  SoVertexProperty *vProp2 = new SoVertexProperty();
  SoMFVec3f *v_points = &(vProp2->vertex);
  v_points->setValues(0, points->getNum(), points->getValues(0));
  p_shapeSet->vertexProperty.setValue(vProp2);
  shapeParent->insertChild(p_shapeSet, shapeIdx+3);
  
  // load into half-edge data structure
  HE_list.init(p_shape, points, p_shapeSet, v_points);
  HE_list.loadShape();

  if (!renderQuit){
    // Set up keyboard
    SoEventCallback *callBack = new SoEventCallback();
    callBack->addEventCallback(SoKeyboardEvent::getClassTypeId(),
			       keyEventHandler);
    scene->addChild(callBack);
    cerr << "\nPress ESC, then R to save to file.\n" << endl;

    // This returns a main window to use.
    // If unsuccessful, exit.
    Widget myWindow = SoXt::init(argv[0]); // pass the app name
    if (myWindow == NULL) exit(1);

    // Set up viewer
    myViewer = new SoXtExaminerViewer(myWindow);
    myViewer->setSceneGraph(scene);
    myViewer->setTitle("Subdivider");
    myViewer->show();
    
    SoXt::show(myWindow); // Display main window
    SoXt::mainLoop();	// Main Inventor event loop
  }
  else
    1;
  
  return(0);
}
