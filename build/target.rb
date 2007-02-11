# Copyright (c) 2006-2007 Michael B. Edwin Rickert
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt ) 
#
# $LastChangedBy$ - $LastChangedDate$

module Industry
	class Target
		attr_accessor :name  #TODO: Add name validity checking

		attr_reader   :type
		def type=( value )
			case value
			when :program , :staticlib , :dynamiclib
				@type = value
			else
				raise ArgumentError , "#{value} is not a valid Target type"
			end
		end
	end
end
