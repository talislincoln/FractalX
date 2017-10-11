#pragma once
#ifndef _MESHCOMPONENT_H
#define _MESHCOMPONENT_H

#include <scene\components\Component.h>

namespace fractal
{
	namespace fcore
	{
		class MeshDataResource;
		class ShaderResource;
	}

	namespace fscene
	{
		class MeshComponent : public Component
		{
		private:
			fcore::MeshDataResource* m_meshData;
			fcore::ShaderResource* m_shaderData;

		public:
			MeshComponent (fcore::MeshDataResource* meshData, fcore::ShaderResource* shaderData);
			~MeshComponent ();

			bool Init () override;
			void Update () override;
			void Draw () const override;
			bool Shutdown () override;
		};
	}
}

#endif // !_MESHCOMPONENT_H
