#  Copyright (c) 2007 Michael B. Edwin Rickert
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$
#  $LastChangedDate$

module Kernel
	[ :add_include_directories \
	, :add_library_directories \
	, :add_library_basename    \
	].each do |accessor|
		module_eval <<-"end_eval"
			def #{accessor} (*args)
				$industry_builder_focus.top.#{accessor}(*args)
			end
		end_eval
	end
end
