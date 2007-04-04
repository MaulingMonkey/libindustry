#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

require 'uuid'

class Program
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
	attr_reader :name, :uuid
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
