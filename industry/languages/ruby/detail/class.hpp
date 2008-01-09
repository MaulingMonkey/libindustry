#ifndef IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS
#define IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS

#include <industry/traits/function.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>
#include <stdarg.h>
#include <industry/languages/ruby/detail/do_call.hpp>

namespace industry { namespace languages { namespace ruby {
	template<class T>
	struct class_;

	namespace detail {
		template<class T, class Fn, unsigned int Arity>
		struct do_call_proxy {
			static VALUE do_call_o(boost::function<Fn> const& f, VALUE self, va_list args) {
				return do_call<T,
					Fn,
					boost::function<Fn>::arity,
					boost::is_same<T,
						typename boost::remove_const<
							typename boost::remove_pointer<
								typename boost::function<Fn>::arg1_type
							>::type
						>::type
					>::value
				>::call(f, self, args);
			}
		};

		template<class T, class Fn>
		struct do_call_proxy<T, Fn, 0> {
			static VALUE do_call_o(boost::function<Fn> const& f, VALUE self, va_list args) {
				return do_call<T, Fn, boost::function<Fn>::arity, false>::call(f, self, args);
			}
		};

		template<class T, class Fn, unsigned int N>
		struct class_n {
			static boost::function<Fn> const& get(boost::function<Fn> f = boost::function<Fn>()) {
				static boost::function<Fn> func;
				if(!f.empty())
					func = f;
				return func;
			}

			static VALUE call_proxy(VALUE self ...) {
				va_list args;
				va_start(args, self);
				return do_call_proxy<T, Fn, boost::function<Fn>::arity>::do_call_o(get(), self, args);
			}

			template<class Fn2>
			class_n<T, typename industry::function_traits<Fn2>::signature, N+1> def(std::string const& name, Fn2 f) {
				class_n<T, typename industry::function_traits<Fn2>::signature, N+1>::get(f);
				rb_define_method(::industry::languages::ruby::class_<T>::get_class(), name.c_str(), RUBY_METHOD_FUNC((class_n<T, typename industry::function_traits<Fn2>::signature, N+1>::call_proxy)), industry::function_traits<Fn2>::arity);
				return class_n<T, typename industry::function_traits<Fn2>::signature, N+1>();
			}
		};
	}
}}}
#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS
