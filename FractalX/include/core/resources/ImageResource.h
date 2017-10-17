#pragma once
#ifndef _IMAGERESOURCE_H
#define _IMAGERESOURCE_H

#include "Resource.h"

namespace fractal
{
	namespace fcore
	{
		class ImageResource : public Resource
		{
		private:
			ID3D11ShaderResourceView*           m_textureResourceView;
			ID3D11SamplerState*                 m_samplerLinear;
		public:
			ImageResource (const FString& name, const FString& path);
			~ImageResource ();

			virtual bool Init ();
			virtual bool Shutdown ();
			
			inline ID3D11ShaderResourceView* GetResourceView () const { return m_textureResourceView; }
			inline ID3D11SamplerState* GetSampler () const { return m_samplerLinear; }
		};
	}
}

#endif // !_IMAGERESOURCE_H
