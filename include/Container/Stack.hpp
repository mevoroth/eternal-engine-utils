#ifndef _STACK_HPP_
#define _STACK_HPP_

#include <cassert>

namespace Eternal
{
	namespace Container
	{
		template<typename TypeT, int SIZE>
		class Stack
		{
		private:
			TypeT _elements[SIZE];
			int _count;
		public:
			Stack();
			~Stack();
			/**
			 * Is stack empty?
			 */
			inline bool Empty() const
			{
				return _count < 0;
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
				return _count + 1;
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
				assert(_count >= 0);
				return _elements[_count];
			}
			/**
			 * Pop stack head
			 */
			void Pop()
			{
				assert(_count >= 0);
				--_count;
			}
		};

		template<typename TypeT> Stack<TypeT>::Stack()
			: _count(-1)
		{
		}
		template<typename TypeT> Stack<TypeT>::~Stack()
		{
		}
		template<typename TypeT> void Stack<TypeT>::Push(const TypeT& element)
		{
			assert(!Full());
			_elements[_count++] = element;
		}
	}
}

#endif
