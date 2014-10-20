#ifndef _STACK_HPP_
#define _STACK_HPP_

#include <cassert>

namespace Eternal
{
	namespace Container
	{
		template<typename TypeT>
		class Stack
		{
		private:
			TypeT* _elements;
			int _size;
			int _count;
		public:
			/**
			 * @param[in] Stack max size
			 */
			Stack(int size);
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
				return Count() == _size;
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
				return _size;
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

		template<typename TypeT> Stack<TypeT>::Stack(int size)
			: _size(size)
			, _count(-1)
		{
			_elements = new TypeT[_size];
		}
		template<typename TypeT> Stack<TypeT>::~Stack()
		{
			delete [] _elements;
		}
		template<typename TypeT> void Stack<TypeT>::Push(const TypeT& element)
		{
			assert(!Full());
			_elements[_count++] = element;
		}
	}
}

#endif
