#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

require 'uuid'

class Library
	def initialize( name )
		@name          = name.to_s
		@include_paths = []
		@library_paths = []
		@libraries     = []
		@dependancies  = []
		@sources       = []
		@uuid          = UUID.new
	end
	attr_accessor :include_paths, :library_paths, :libraries, :dependancies, :sources
	def all_include_paths(); @include_paths + self.all_dependancies.collect {|d| d.all_include_paths}; end
	def all_library_paths(); @library_paths + self.all_dependancies.collect {|d| d.all_library_paths}; end
	def all_libraries();     @libraries     + self.all_dependancies.collect {|d| d.all_libraries    }; end
	def all_dependancies();  @dependancies  +         @dependancies.collect {|d| d.all_dependancies }; end
	attr_reader :name, :uuid
end

def library( name )
	if !block_given?
		$libraries[name]
	else
		raise ArgumentError , "Multiple definition of library #{name}" unless !$libraries[name]
		$focus = $libraries[name] = Library.new( name )
		yield
		$focus = nil
	end
end
