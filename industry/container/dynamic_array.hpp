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

		template<typename Type, class Alloc = dynamic_array_default_allocation_policy<Type> >
		class dynamic_array {
		public:
			typedef typename Alloc::size_type size_type;
			typedef typename Alloc::difference_type difference_type;
			typedef typename Alloc::pointer pointer;
			typedef typename Alloc::reference reference;
			typedef typename Alloc::value_type value_type;
			typedef typename Alloc::const_pointer const_pointer;
			typedef typename Alloc::const_reference const_reference;
			typedef pointer iterator;
			typedef const_pointer const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

			dynamic_array(Alloc const& alloc = Alloc()) : alloc_(alloc) { }
			explicit dynamic_array(size_type count, value_type const& value = value_type(), Alloc const& alloc = Alloc()) : alloc_(alloc) { }
			~dynamic_array() { }

			iterator begin() { return iterator(first_); }
			iterator end() { return iterator(last_); }

			const_iterator begin() const { return iterator(first_); }
			const_iterator end() const { return iterator(last_); }

			reverse_iterator rbegin() { return std::reverse_iterator<iterator>(end()); }
			reverse_iterator rend() { return std::reverse_iterator<iterator>(begin()); }

			const_reverse_iterator rbegin() const { return std::reverse_iterator<const_iterator>(end()); }
			const_reverse_iterator rend() const { return std::reverse_iterator<const_iterator>(begin()); }

			reference front() { return *begin(); }
			const_reference front() const { return *begin(); }

			reference back() { return *(end() - 1); }
			const_reference back() const { return *(end() - 1); }

			size_type max_size() const { return alloc_.max_size(); }
			size_type size() const { return static_cast<size_type>(last_ - first_); }
			size_type capacity() const { return static_cast<size_type>(end_ - first_); }
			bool empty() const { return first_ == last_; }
			Alloc get_allocator() const { return alloc_; }

			reference operator[](size_type position) { return *(begin() + position); }
			const_reference operator[](size_type position) const { return *(begin() + position); }

			reference at(size_type position) {
				if(position >= size())
					throw std::out_of_range("Attempted access out of dynamic_array bounds.");

				return *(begin() + position);
			}

			const_reference at(size_type position) const {
				if(position >= size())
					throw std::out_of_range("Attempted access out of dynamic_array bounds.");

				return *(begin() + position);
			}

			void assign(size_type count, value_type const& value) {
				erase(begin(), end());
				insert(begin(), count, value);
			}

			template<typename InputIterator>
			void assign(InputIterator first, InputIterator last) {
				erase(begin(), end());
				insert(begin(), first, last);
			}

			void clear() { //TODO: Implement clear()
				erase(begin(), end());
			}

			iterator erase(iterator position) { //TODO: Implement erase(iterator)
				return ++position;
			}

			iterator erase(iterator first, iterator last) { //TODO: Implement erase(iterator, iterator)
				return ++last;
			}

			iterator insert(iterator position, const value_type& value) { //TODO: Implement insert(iterator, const_reference)
				return position;
			}

			void insert(iterator position, size_type count, const value_type& value) { //TODO: Implement insert(iterator, size_type, const_reference)
			}

			template<typename InputIterator>
			void insert(iterator position, InputIterator first, InputIterator last) { //TODO: Implement insert(iterator, InputIterator, InputIterator)
			}

			void reserve(size_type capacity) { //TODO: Implement reserve(size_type)
			}

			void resize(size_type length, value_type value = value_type()) {
				if(length > size()) {
					insert(end(), length - size(), value);
				} else if(length < size()) {
					erase(begin() + length, end());
				}
			}
		private:
			pointer first_;
			pointer last_;
			pointer end_;
			Alloc alloc_;
		};
	}
}

#endif //IG_INDUSTRY_CONTAINER_DYNAMIC_ARRAY
