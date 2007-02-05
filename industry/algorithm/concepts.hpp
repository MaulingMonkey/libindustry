// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ALGORITHM_CONCEPTS
#define IG_INDUSTRY_ALGORITHM_CONCEPTS

#include <industry/traits/processor.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/static_assert.hpp>

namespace industry {
	namespace algorithm {
		template < typename Processor > struct coerce_to_processor {
			typedef typename boost::remove_const< Processor >::type type;
		};

		template < typename LHS , typename RHS > class   bound_processor_chain;
		template < typename LHS , typename RHS > class unbound_processor_chain;
		template < typename LHS , typename RHS , typename LHSType = typename LHS::processor_type , typename RHSType = typename RHS::processor_type >
		struct coerce_to_processor_chain {
			static const bool LHS_is_unbound_processor = boost::is_same< typename LHS::processor_type , unbound_processor_tag >::value;
			static const bool LHS_is_bound_processor   = boost::is_same< typename LHS::processor_type ,   bound_processor_tag >::value;
			BOOST_STATIC_ASSERT(( LHS_is_unbound_processor || LHS_is_bound_processor ));

			static const bool RHS_is_unbound_processor = boost::is_same< typename RHS::processor_type , unbound_processor_tag >::value;
			BOOST_STATIC_ASSERT(( RHS_is_unbound_processor ));

			BOOST_STATIC_ASSERT(( false )); //This class should never be instantiated... implementation for diagnostic errors only (one of the above should trip)
		};

		template < typename LHS , typename RHS > struct coerce_to_processor_chain< LHS , RHS , unbound_processor_tag , unbound_processor_tag > {
			BOOST_STATIC_ASSERT(( boost::is_same< typename LHS::processor_type , unbound_processor_tag >::value ));
			BOOST_STATIC_ASSERT(( boost::is_same< typename RHS::processor_type , unbound_processor_tag >::value ));

			typedef unbound_processor_chain< LHS , RHS > type;
		};

		template < typename LHS , typename RHS > struct coerce_to_processor_chain< LHS , RHS ,   bound_processor_tag , unbound_processor_tag > {
			BOOST_STATIC_ASSERT(( boost::is_same< typename LHS::processor_type ,   bound_processor_tag >::value ));
			BOOST_STATIC_ASSERT(( boost::is_same< typename RHS::processor_type , unbound_processor_tag >::value ));

			typedef bound_processor_chain< LHS , RHS > type;
		};

		template < typename Self >
		class unbound_processor {
		public:
			typedef unbound_processor_tag processor_type;

			template < typename PreceedingProcessor >
			friend typename coerce_to_processor_chain< PreceedingProcessor , Self >::type
				operator|( const PreceedingProcessor & preceeding , const Self & self )
			{
				return typename coerce_to_processor_chain< typename coerce_to_processor<PreceedingProcessor>::type , Self >
					::type( typename coerce_to_processor<PreceedingProcessor>::type(lhs) , rhs );
			}
		};

		template < typename Self >
		class bound_processor {
		public:
			typedef bound_processor_tag processor_type;
		};

		template < typename LHS , typename RHS >
		class unbound_processor_chain : public unbound_processor< unbound_processor_chain< LHS , RHS > > {
			BOOST_STATIC_ASSERT(( boost::is_same< typename LHS::processor_type , unbound_processor_tag >::value ));
			BOOST_STATIC_ASSERT(( boost::is_same< typename RHS::processor_type , unbound_processor_tag >::value ));

			LHS lhs;
			RHS rhs;
		public:
			unbound_processor_chain( const LHS& lhs , const RHS& rhs ): lhs(lhs), rhs(rhs) {}

			template < typename PREActiveProcessor > class activate;
			template < typename PREActiveProcessor > friend class activate;

			template < typename PREActiveProcessor > class activate {
				typedef typename LHS::template activate< PREActiveProcessor > LHSActiveProcessor;
				typedef typename RHS::template activate< LHSActiveProcessor > RHSActiveProcessor;

				LHSActiveProcessor lhs;
				RHSActiveProcessor rhs;
			public:
				activate( const PREActiveProcessor & preceeding , const unbound_processor_chain & self ): lhs(preceeding,self.lhs), rhs(lhs,self.rhs) {}
				typedef typename RHSActiveProcessor::result_type result_type;

				void        advance()   { rhs.advance(); }
				bool        end() const { return rhs.end(); }
				result_type get() const { return rhs.get(); }	
			};
		};
		template < typename LHS , typename RHS >
		class bound_processor_chain {
			BOOST_STATIC_ASSERT(( boost::is_same< typename LHS::processor_type ,   bound_processor_tag >::value ));
			BOOST_STATIC_ASSERT(( boost::is_same< typename RHS::processor_type , unbound_processor_tag >::value ));

			LHS lhs;
			RHS rhs;
		public:
			bound_processor_chain( const LHS& lhs , const RHS& rhs ): lhs(lhs), rhs(rhs) {}

			class activate;
			friend class activate;

			class activate {
				typedef typename LHS::         activate /* original source */ LHSActiveProcessor;
				typedef typename RHS::template activate< LHSActiveProcessor > RHSActiveProcessor;

				LHSActiveProcessor lhs;
				RHSActiveProcessor rhs;
			public:
				activate( const bound_processor_chain & self ): lhs(self.lhs), rhs(lhs,self.rhs) {}
				typedef typename RHSActiveProcessor::result_type result_type;

				void        advance()   { rhs.advance(); }
				bool        end() const { return rhs.end(); }
				result_type get() const { return rhs.get(); }
			};
		};
	}
}

#endif //ndef IG_INDUSTRY_ALGORITHM_CONCEPTS





#if 0 //Old header -- for reference only:
#include <industry/range.hpp>

namespace industry {
	namespace algorithm {
		template < typename BoundProcessor > struct active_processor_of { typedef typename BoundProcessor::active_processor_type type; };
		template < typename Iterator > class active_range_processor {
			      Iterator i;
			const Iterator end_i;
		public:
			typedef typename industry::range<Iterator>::reference reference;
			active_range_processor( industry::range<Iterator> const& r ): i(r.begin()), end_i(r.end()) {}

			reference get() const { return *i;} 
			void advance() { ++i; }
			bool end() { return i == end_i; }
		};
		template < typename Iterator > struct active_processor_of< industry::range< Iterator > > { typedef active_range_processor<Iterator> type; };

		//template < typename ActiveProccessorT , typename NextUnboundProcessor = industry::nil > struct unbound_processor {
		//	friend ... activate( unbound_processor const& p ) {
		//};

		//template < typename BoundProcessorConvertable > struct bound_processor_of {};
		/* Primary concepts:
		 *
		 * - UnboundProcessor
		 *   A processor not bound to an input range -- used for secondary processor inputs
		 *   UnboundProcessor | UnboundProcessor results in another UnboundProcessor (a "chained" processor)
		 *   Example:  Result of "filter", or "transform"
		 *
		 * - BoundProcessor
		 *   A processor bound to an input range
		 *   Example:  Result of "each"
		 *
		 * - ActiveProcessor
		 *   A processor with iteration state data -- invokation of a BoundProcessor chain -- actual processor logic here.
		 *   Interface:
		 *      reference get() const; //current element
		 *      void advance(); //advance to next position
		 *      bool end(); //get() no longer valid
		 *
		 * - EndpointProcessor
		 *   A processor which when concatonated to a BoundProcessor chain invokes an operation upon the iterated over elements.
		 *   Takes a BoundProcessor, converts to an ActiveProcessor, and uses said chain.
		 *
		 *   Examples:  Result of "call", or "push_back"
		 *
		 * Other concepts that map to algorithm concepts:
		 *
		 * - Range      is-a        BoundProcessor
		 * - Container  converts-to Range             is-a BoundProcessor
		 *
		 * Other concepts that map to specific algorithm items:
		 *
		 * - Function Pointers         convert-to UnboundProcessor[ transform_fun ]
		 * - Member Function Pointers  convert-to UnboundProcessor[ transform_mem_fun ]
		 * - Unary Functors            convert-to UnboundProcessor[ transform_functor ]
		 * - 
		 */
	}
}
#endif
