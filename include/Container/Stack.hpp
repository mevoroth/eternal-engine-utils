#ifndef _STACK_HPP_
#define _STACK_HPP_

#include "Macros/Macros.hpp"

namespace Eternal
{
	namespace Container
	{
		template<typename TypeT, int SIZE>
		class Stack
		{
		private:
			TypeT _Elements[SIZE];
			int _Count = 0;
		public:
			Stack();
			~Stack();
			/**
			 * Is stack empty?
			 */
			inline bool Empty() const
			{
				return _Count == 0;
			}
			/**
			 * Is stack full?
			 */
			bool Full() const
			{
				return Count() == SIZE;
			}
			/**
			 * Elements count in stack
			 */
			inline int Count() const
			{
				return _Count;
			}
			/**
			 * Stack max size
			 */
			inline int Size() const
			{
				return SIZE;
			}
			/**
			 * Push element in stack
			 */
			void Push(const TypeT& element);
			/**
			 * Read stack head
			 */
			inline TypeT Head()
			{
				ETERNAL_ASSERT(!Empty());
				return _Elements[_Count - 1];
			}
			/**
			 * Pop stack head
			 */
			void Pop()
			{
				ETERNAL_ASSERT(_Count >= 0);
				--_Count;
			}
		};

		template<typename TypeT, int SIZE> Stack<TypeT, SIZE>::Stack()
			: _Count(0)
		{
		}
		template<typename TypeT, int SIZE> Stack<TypeT, SIZE>::~Stack()
		{
		}
		template<typename TypeT, int SIZE> void Stack<TypeT, SIZE>::Push(const TypeT& Element)
		{
			ETERNAL_ASSERT(!Full());
			_Elements[_Count++] = Element;
		}
	}
}

#endif
