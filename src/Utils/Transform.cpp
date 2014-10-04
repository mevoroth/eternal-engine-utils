#include "Utils/Transform.hpp"

using namespace Eternal::Utils;

Transform::Transform()
	: Position(XMVectorSet(0.f, 0.f, 0.f, 1.f))
	, Rotation(XMVectorSet(0.f, 0.f, 0.f, 1.f))
	, Scale(XMVectorSet(1.f, 1.f, 1.f, 1.f))
{
}
