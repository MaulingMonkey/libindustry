#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

class Script
	def initialize( name , language )
		@name         = name.to_s
		@language     = language
		@dependancies = []
		@sources      = []
	end
	attr_accessor :dependancies, :sources, :start
	attr_reader :name, :language
end

def script( name , language )
	if !block_given?
		raise ArgumentError , "Expected a block"
	else
		raise ArgumentError , "Multiple definitions of script #{name}" unless !$scripts[name]
		$focus = $scripts[name] = Script.new( name , language )
		yield
		$focus = nil
	end
end
