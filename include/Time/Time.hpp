#ifndef _TIME_HPP_
#define _TIME_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Time
	{
		typedef uint64_t TimeT;
		class Time
		{
		private:
			static Time* _Inst;
		public:
			Time();
			static Time* Get();
			static void ToHumanReadable(_In_ const TimeT& Value, _Out_ char HumanReadable[24]);

			virtual void Begin() = 0;
			virtual TimeT End() = 0;
			virtual TimeT GetTime() const = 0;
			virtual TimeT GetDeltaTime() const = 0;
			virtual void Update() = 0;
		};
	}
}

#endif
