//***************************************************************************************
// GeometryGenerator.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines a static class for procedurally generating the geometry of 
// common mathematical objects.
//***************************************************************************************


#pragma once
#ifndef _GEOMETRYGENERATOR_H
#define _GEOMETRYGENERATOR_H

#include <core\resources\MeshDataResource.h>

namespace fractal
{
	class GeometryGenerator
	{
	public:
		static fcore::MeshData LoadObjFromFile (FString filePath);
		static fcore::MeshData CreateBox (float width, float height, float depth);
		static fcore::MeshData CreateSphere (float radius, UINT sliceCount, UINT stackCount);
	};
}

#endif // !_GEOMETRYGENERATOR_H
