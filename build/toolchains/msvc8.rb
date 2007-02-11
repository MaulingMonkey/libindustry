# Copyright (c) 2006-2007 Michael B. Edwin Rickert
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt ) 
#
# $LastChangedBy$ - $LastChangedDate$

module Industry
	class MSVC8_ToolChain
		def build( *targets )
			targets.each do |target|
				if target.respond_to? :each then
					build( *target )
				else
					build_target( target )
				end
			end
		end
		def build_target( target )
			puts "I should build #{target.name}, but MSVC8_ToolChain isn't really implemented yet!"
		end
	end
end
