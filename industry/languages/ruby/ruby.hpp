#ifndef IG_LANGUAGES_RUBY_RUBY
#define IG_LANGUAGES_RUBY_RUBY

#include "detail/wrap_retarded_ruby.hpp"
#include "detail/ruby_value.hpp"
#include "detail/class.hpp"
#include <string>

namespace industry { namespace languages { namespace ruby {
#	define INDUSTRY_RUBY_MODULE(name) void Do_##name##_Init();\
	extern "C" void  __declspec(dllexport) Init_##name () {\
		ruby_init();\
		Do_##name##_Init();\
	}\
	void Do_##name##_Init()

	template<class T>
	struct class_ {
		static VALUE get_class(VALUE v = 0) {
			static VALUE klass;
			if(v) {
				klass = v;
			}

			return klass;
		}

		static void free_type(T* ptr) {
			delete ptr;
		}

		static VALUE alloc_type(VALUE klass) {
			T* ptr = new T();
			return Data_Wrap_Struct(klass, 0, free_type, ptr);
		}

		class_(std::string const& name) {
			get_class(rb_define_class(name.c_str(), rb_cObject));
			rb_define_alloc_func(get_class(), alloc_type);
		}

		template<class Fn2>
		detail::class_n<T, typename industry::function_traits<Fn2>::signature, 0> def(std::string const& name, Fn2 f) {
			typedef detail::class_n<T, typename industry::function_traits<Fn2>::signature, 0> f_proxy_class;
			f_proxy_class::get(f);
			rb_define_method(class_<T>::get_class(), name.c_str(), RUBY_METHOD_FUNC(f_proxy_class::call_proxy), industry::function_traits<Fn2>::arity);
			return f_proxy_class();
		}
	};
}}}
#endif//IG_LANGUAGES_RUBY_RUBY
