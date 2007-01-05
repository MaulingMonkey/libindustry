// Copyright (c) 2006 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_CONTAINER_DYNAMIC_ARRAY
#define IG_INDUSTRY_CONTAINER_DYNAMIC_ARRAY

#include <memory>
#include <limits>
#include <stdexcept>

namespace industry {
	namespace container {
		template<typename Type>
		class dynamic_array_default_allocation_policy : public std::allocator<Type> {
			typedef std::allocator<Type> base_allocator;
		public:
			typedef typename base_allocator::size_type size_type;
			typedef typename base_allocator::difference_type difference_type;
			typedef typename base_allocator::pointer pointer;
			typedef typename base_allocator::reference reference;
			typedef typename base_allocator::value_type value_type;
			typedef typename base_allocator::const_pointer const_pointer;
			typedef typename base_allocator::const_reference const_reference;

			std::pair<pointer, size_type> grow(size_type size, pointer old) {
				return std::make_pair(this->allocate(size + size/2, old), size + size/2);
			}
		};
	}
}

#endif //IG_INDUSTRY_CONTAINER_DYNAMIC_ARRAY
