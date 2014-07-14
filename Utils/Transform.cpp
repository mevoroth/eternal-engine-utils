#include "Transform.hpp"

using namespace Eternal::Utils;

Transform::Transform()
{
	_p = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_s = XMVectorSet(1.f, 1.f, 1.f, 1.f);
}
