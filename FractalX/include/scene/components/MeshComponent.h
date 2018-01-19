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
		class MaterialResource;
	}

	namespace fscene
	{
		class MeshComponent : public Component
		{
		private:
			fcore::MeshDataResource*	m_meshData;
			fcore::ShaderResource*		m_shaderData;
			fcore::ImageResource*		m_imageData;
			fcore::MaterialResource*	m_materialData;

		public:
			MeshComponent (fcore::MeshDataResource* meshData, fcore::ShaderResource* shaderData, fcore::ImageResource* imageData, fcore::MaterialResource* materialData);
			MeshComponent (const FString& meshResourceName, const FString& shaderResourceName, const FString& imageResourceName, fcore::MaterialResource* materialData);
			~MeshComponent ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;

			class fcore::ShaderResource* GetShader () const;
		};
	}
}

#endif // !_MESHCOMPONENT_H
