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
			static Time* _inst;
		public:
			Time();
			static Time* Get();
			virtual void Begin() = 0;
			virtual TimeT End() = 0;
			virtual TimeT GetTime() const = 0;
		};
	}
}

#endif
