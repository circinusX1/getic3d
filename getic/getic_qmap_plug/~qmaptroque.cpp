void EditGeometry::exportToMap(const char * fileName, U32 detailNum)
{
   FileStream file;
   file.open(fileName, FileStream::Write);

   char buf[1024];
   writeString(file, "{\n");
   writeString(file, "\"classname\" \"worldspawn\"\n");
   writeString(file, "\"mapversion\" \"220\"\n");

   // wadname
   dSprintf(buf, sizeof(buf), "\"wad\" \"%s\"\n", gMapProperties.mWadName);
   writeString(file, buf);

   // geometry scale
   dSprintf(buf, sizeof(buf), "\"geometry_scale\" \"%f\"\n", gMapProperties.mGeometryScale);
   writeString(file, buf);

   // lighting scale
   dSprintf(buf, sizeof(buf), "\"light_geometry_scale\" \"%f\"\n", gMapProperties.mLightingScale);
   writeString(file, buf);

   // ambient color
   dSprintf(buf, sizeof(buf), "\"ambient_color\", \"%d %d %d\"\n", 
      gMapProperties.mAmbientColor.red, gMapProperties.mAmbientColor.green, gMapProperties.mAmbientColor.blue);
   writeString(file, buf);

   // emergency lighting color
   dSprintf(buf, sizeof(buf), "\"emergency_ambient_color\", \"%d %d %d\"\n",
      gMapProperties.mEmergencyAmbientColor.red, gMapProperties.mEmergencyAmbientColor.green, gMapProperties.mEmergencyAmbientColor.blue);
   writeString(file, buf);

   // minpixels
   dSprintf(buf, sizeof(buf), "\"min_pixels\" \"%d\"\n", mMinPixels);
   writeString(file, buf);

   // detailNum
   dSprintf(buf, sizeof(buf), "\"detail_number\" \"%d\"\n", detailNum);
   writeString(file, buf);

   // create a brush per face..
   for(U32 i = 0; i < mSurfaces.size(); i++)
   {
      Surface & surface = mSurfaces[i];

      writeString(file, "{\n");

      char buf[1024];
      
      // surface plane..
      Point3F pnts[3];
      for(U32 j = 0; j < 3; j++)
         pnts[j] = getPoint(surface.winding.indices[j]);

      // unscale texgens
      surface.texGenX.x *= mMaterialSizes[surface.materialIndex].x;
      surface.texGenX.y *= mMaterialSizes[surface.materialIndex].x;
      surface.texGenX.z *= mMaterialSizes[surface.materialIndex].x;
      surface.texGenX.d *= mMaterialSizes[surface.materialIndex].x;

      surface.texGenY.x *= mMaterialSizes[surface.materialIndex].y;
      surface.texGenY.y *= mMaterialSizes[surface.materialIndex].y;
      surface.texGenY.z *= mMaterialSizes[surface.materialIndex].y;
      surface.texGenY.d *= mMaterialSizes[surface.materialIndex].y;

      // write out this plane
      dSprintf(buf, sizeof(buf), "( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %s [ %f %f %f %f ] [ %f %f %f %f ] 0 1 1\n",
         (S32)pnts[2].x, (S32)pnts[2].y, (S32)pnts[2].z,
         (S32)pnts[1].x, (S32)pnts[1].y, (S32)pnts[1].z,
         (S32)pnts[0].x, (S32)pnts[0].y, (S32)pnts[0].z,
         getMaterial(surface.materialIndex),
         surface.texGenX.x, surface.texGenX.y, surface.texGenX.z, surface.texGenX.d,
         surface.texGenY.x, surface.texGenY.y, surface.texGenY.z, surface.texGenY.d);
      writeString(file, buf);

      PlaneF plane = getPlane(surface.planeIndex);
      
      // get the surface area
      Point3F areaNorm(0.f,0.f,0.f);
      Point3F tmp;

      mCross(p.vXes[0]._xyz, p.vXes[1]._xyz);  areaNorm += tmp;
      mCross(p.vXes[1]._xyz, p.vXes[2]._xyz);  areaNorm += tmp;
      mCross(p.vXes[2]._xyz, p.vXes[0]._xyz);  areaNorm += tmp;

      F32 len = mSqrt(mFabs(mDot(plane, areaNorm) * 0.5f));
      
      // null bounding planes
      Point3F center = getSurfaceCenter(surface);
      center -= (plane * mClampF(len * gcCenterOffsetScale, gcMinCenterOffset, gcMaxCenterOffset));

      // write out the null planes
      for(U32 k = 0; k < surface.winding.numIndices; k++)
      {
         U32 l = (k+1) % surface.winding.numIndices;

         pnts[0] = getPoint(surface.winding.indices[l]);
         pnts[1] = getPoint(surface.winding.indices[k]);
         pnts[2] = center;

         // bunk texture info (null surfaces)
         dSprintf(buf, sizeof(buf), "( %d %d %d ) ( %d %d %d ) ( %d %d %d ) NULL [ 1 0 0 0 ] [ 0 1 0 0 ] 0 1 1\n",
            (S32)pnts[2].x, (S32)pnts[2].y, (S32)pnts[2].z,
            (S32)pnts[1].x, (S32)pnts[1].y, (S32)pnts[1].z,
            (S32)pnts[0].x, (S32)pnts[0].y, (S32)pnts[0].z);
         writeString(file, buf);
      }

      writeString(file, "}\n");
   }

   // wad file
   writeString(file, "}\n");

   // the collision info is dumped on highest detail
   if(!detailNum)
   {
      gCollisionDetail->processConvexHullObjects();
      gCollisionDetail->export(file);
   }
      
   file.close();
}