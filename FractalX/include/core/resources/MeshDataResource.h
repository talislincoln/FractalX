#pragma once
#ifndef _MESHDATARESOURCE_H
#define _MESHDATARESOURCE_H

#include <core\resources\Resource.h>
#include <vector>

namespace fractal
{
	namespace fcore
	{
		class MeshDataResource : public Resource
		{
		private:
			ID3D11Buffer* m_d3dVertexBuffer;
			ID3D11Buffer* m_d3dIndexBuffer;

			unsigned __int32 m_indicesCount;

		public:
			MeshDataResource (const FString& path);
			MeshDataResource (const FString& name, std::vector<VertexPosColor> vertices, std::vector<WORD> indices);

			MeshDataResource () = delete;
			MeshDataResource (MeshDataResource&) = delete;
			MeshDataResource (MeshDataResource&&) = delete;

			bool Init () override;
			bool Shutdown () override;

			ID3D11Buffer* GetVertexBuffer () const;
			ID3D11Buffer* GetIndexBuffer () const;

			unsigned __int32 GetIndicesCount () const;
		};
	}
}
#endif // !_MESHDATARESOURCE_H
