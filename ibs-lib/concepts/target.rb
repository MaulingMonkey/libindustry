# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

module Industry
	class Target
		def initialize(name)
			@dependancies ||= []
			@basename     ||= nil
			@debug_info   ||= true
			@type         ||= :program
		end
		attr_accessor :dependancies, :basename, :debug_info, :type
	end
end
