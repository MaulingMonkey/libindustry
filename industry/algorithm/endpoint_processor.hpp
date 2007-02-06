// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ALGORITHM_ENDPOINT_PROCESSOR
#define IG_INDUSTRY_ALGORITHM_ENDPOINT_PROCESSOR

#include <industry/algorithm/coerce_processor.hpp>

namespace industry {
	namespace algorithm {
		template < typename Self > class endpoint_processor {
		public:
			template < typename BoundProcessorCoerceable >
			friend void operator|( const BoundProcessorCoerceable & lhs , const Self & rhs ) {
				typedef typename coerce_to_processor< const BoundProcessorCoerceable& >::type BoundProcessor;
				BOOST_STATIC_ASSERT(( boost::is_same< typename BoundProcessor::processor_type , bound_processor_tag >::value ));
				BoundProcessor bp(lhs);

				typedef typename BoundProcessor::activate ActiveProcessor;
				BOOST_STATIC_ASSERT(( boost::is_same< typename AcitveProcessor::processor_type , active_processor_tag >::value ));
				ActiveProcessor ap(bp);

				rhs.act(ap);
			}
			template < typename BoundProcessorCoerceable >
			friend void operator|(       BoundProcessorCoerceable & lhs , const Self & rhs ) {
				typedef typename coerce_to_processor< BoundProcessorCoerceable& >::type BoundProcessor;
				BOOST_STATIC_ASSERT(( boost::is_same< typename BoundProcessor::processor_type , bound_processor_tag >::value ));
				BoundProcessor bp(lhs);

				typedef typename BoundProcessor::activate ActiveProcessor;
				BOOST_STATIC_ASSERT(( boost::is_same< typename AcitveProcessor::processor_type , active_processor_tag >::value ));
				ActiveProcessor ap(bp);

				rhs.act(ap);
			}
		};
	}
}

#endif //ndef IG_INDUSTRY_ALGORITHM_ENDPOINT_PROCESSOR
