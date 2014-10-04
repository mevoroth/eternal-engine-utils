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
		public:
			/**
			 * Position
			 */
			XMVECTOR Position;
			/**
			 * Rotation
			 */
			XMVECTOR Rotation;
			/**
			 * Scaling
			 */
			XMVECTOR Scale;
			Transform();
		};
	}
}

#endif
