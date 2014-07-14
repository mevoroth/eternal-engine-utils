#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include <DirectXMath.h>

using namespace DirectX;

namespace Eternal
{
	namespace Utils
	{
		class Transform
		{
		private:
			/**
			 * Position
			 */
			XMVECTOR _p;
			/**
			 * Rotation
			 */
			XMVECTOR _r;
			/**
			 * Scaling
			 */
			XMVECTOR _s;
		public:
			Transform();
		};
	}
}

#endif
