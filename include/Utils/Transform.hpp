#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include <DirectXMath.h>

using namespace DirectX;

namespace Eternal
{
	namespace Utils
	{
		class Vector3;
		class Transform
		{
			virtual void Translate(const Vector3& d) = 0;
			//virtual void 
		};
	}
}

#endif
