#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

class Dependancy
	def initialize( name )
		@include_paths = []
		@library_paths = []
		@libraries     = []
	end
	attr_accessor :include_paths, :library_paths, :libraries
	def all_include_paths(); @include_paths; end
	def all_library_paths(); @library_paths; end
	def all_libraries();     @libraries    ; end
	def all_dependancies();  self          ; end
end

def dependancy( name )
	if !$focus
		if !block_given?
			$dependancies[name]
		else
			raise ArgumentError , "Multiple definition of dependancy #{name}" unless !$dependancies[name]
			$focus = $dependancies[name] = Dependancy.new( name )
			yield
			$focus = nil
		end
	else
		dependancy = name
		dependancy = ($dependancies[name] || $libraries[name] || $scripts[name] || $programs[name]) if name.kind_of? Symbol
		raise ArgumentError, "Couldn't find dependancy: #{dependancy}" unless dependancy
		$focus.dependancies << dependancy
	end
end
