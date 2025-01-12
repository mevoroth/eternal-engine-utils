#pragma once

namespace Eternal
{
	namespace Types
	{
		template<typename EnumType>
		constexpr uint32_t ToUInt(EnumType InEnumValue)
		{
			return static_cast<uint32_t>(InEnumValue);
		}

		template<typename EnumType>
		constexpr int ToInt(EnumType InEnumValue)
		{
			return static_cast<int>(InEnumValue);
		}
	}
}
