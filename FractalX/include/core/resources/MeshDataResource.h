#pragma once
#ifndef _MESHDATARESOURCE_H
#define _MESHDATARESOURCE_H

#include "Resource.h"

namespace fractal
{
	namespace fcore
	{

		struct VertexPosColor
		{
			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT3 Color;
		};

		class MeshDataResource : public Resource
		{
			MeshDataResource (const FString& path);
			MeshDataResource (const FString& name, VertexPosColor* vertices, int* indices);

			MeshDataResource ();
		};
	}
}
#endif // !_MESHDATARESOURCE_H
