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
		static fcore::MeshData LoadObjFromFile (const std::string& filePath);
		static fcore::MeshData CreateBox (float width, float height, float depth);
		static fcore::MeshData CreateSphere (float radius, UINT sliceCount, UINT stackCount);
	private:
		static bool ReadFileCounts (const std::string& filePath, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
	};
}

#endif // !_GEOMETRYGENERATOR_H
