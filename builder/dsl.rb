#  Copyright (c) 2007 Michael B. Edwin Rickert
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$
#  $LastChangedDate$

require 'builder/import'
require 'builder/project'
require 'builder/solution'


#  These are used to dispatch various in-block statements
$industry_builder_imports ||= {} # import ids => Import instances
$industry_builder_focus   ||= [] # stack of project/solution/target (innermost == .last)


module Kernel
	#  Unimplemented bin:
	def target(name,&definition); end
	def sources(*patterns,&statements); end


	#  Maybe msvc80_*/gnu_* should be refactored elsewhere?
	#  Maybe just def method_missing() instead?
	[ :add_include_directories  \
	, :add_library_directories  \
	, :add_library_basename     \
	, :basename                 \
	, :dependancy               \
	, :type                     \
	, :warnings                 \
                                  \
	, :msvc80_solution_filename \
	, :supress_msvc80_warnings  \
	, :gnu_makefile_filename    \
	].each do |accessor|
		module_eval <<-"end_eval"
			def #{accessor} (*args)
				$industry_builder_focus.top.#{accessor}(*args)
			end
		end_eval
	end

	#  No, I'm not sure there's a reason this shouldn't just be made an array
	{ :import   => Industry::Import    \
	, :project  => Industry::Project   \
	, :solution => Industry::Solution  \
	}.each do |list,klass|
		module_eval <<-"end_eval"
			def #{list}
				if block_given? then
					#  Fill out a new list entry definition, or extend an existing one:
					$industry_builder_list_#{list} ||= {}
					$industry_builder_list_#{list}[id] ||= klass.new(id)
					$industry_builder_focus.push $industry_builder_list_#{list}[id]
					yield
					$industry_builder_focus.pop
				else
					#  Return a reference to the existing import definition:
					$industry_builder_list_#{list}[id]
				end
			end
		end_eval
	end
end
