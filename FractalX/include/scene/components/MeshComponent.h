#pragma once
#ifndef _MESHCOMPONENT_H
#define _MESHCOMPONENT_H

#include <scene\components\Component.h>

namespace fractal
{
	// forward declarations
	namespace fcore
	{
		class MeshDataResource;
		class ShaderResource;
		class ImageResource;
	}

	namespace fscene
	{
		class MeshComponent : public Component
		{
		private:
			fcore::MeshDataResource*	m_meshData;
			fcore::ShaderResource*		m_shaderData;
			fcore::ImageResource*		m_imageData;

		public:
			MeshComponent (fcore::MeshDataResource* meshData, fcore::ShaderResource* shaderData, fcore::ImageResource* imageData);
			MeshComponent (const FString& meshResourceName, const FString& shaderResourceName, const FString& imageResourceName);
			~MeshComponent ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;
		};
	}
}

#endif // !_MESHCOMPONENT_H
