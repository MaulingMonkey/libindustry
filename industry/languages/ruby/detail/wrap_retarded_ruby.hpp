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
