#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

class Program
	def initialize( name )
		@name          = name.to_s
		@include_paths = []
		@library_paths = []
		@libraries     = []
		@dependancies  = []
		@sources       = []
		@msvc_warning_level      = 3
		@msvc_supressed_warnings = []
	end
	attr_accessor :include_paths, :library_paths, :libraries, :dependancies, :sources
	attr_accessor :msvc_warning_level, :msvc_supressed_warnings
	def all_include_paths(); (@include_paths + self.all_dependancies.collect {|d| d.all_include_paths}).uniq.flatten; end
	def all_library_paths(); (@library_paths + self.all_dependancies.collect {|d| d.all_library_paths}).uniq.flatten; end
	def all_libraries();     (@libraries     + self.all_dependancies.collect {|d| d.all_libraries    }).uniq.flatten; end
	def all_dependancies();  (@dependancies  +         @dependancies.collect {|d| d.all_dependancies }).uniq.flatten; end
	attr_reader :name
end

def program( name )
	if !block_given?
		raise ArgumentError , "Cannot refer to a program (expected a block to define one)"
	else
		raise ArgumentError , "Multiple definition of program #{name}" unless !$programs[name]
		$focus = $programs[name] = Program.new( name )
		yield
		$focus = nil
	end
end