// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_WRAP_RETARDED_RUBY
#define IG_INDUSTRY_LANGUAGES_RUBY_WRAP_RETARDED_RUBY

#include <ruby.h>

#ifdef RUBY_WIN32_H
#undef getc
#undef putc
#undef fgetc
#undef fputc
#undef getchar
#undef putchar
#undef fgetchar
#undef fputchar
#undef utime

#undef read
#undef write
#undef fclose
#undef close
#undef sleep
#endif
#endif//IG_INDUSTRY_LANGUAGES_RUBY_WRAP_RETARDED_RUBY
