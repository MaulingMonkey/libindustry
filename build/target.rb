# Copyright (c) 2006-2007 Michael B. Edwin Rickert
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt ) 
#
# $LastChangedBy$ - $LastChangedDate$

module Industry
	class Target
		# TODO: Add name validity checking
		attr_accessor :name


		# Conceptual target dependancies (e.g. program depends on source files, not
		# the object files, which are a toolchain implementation detail)
		attr_reader   :dependancies


		# Conceptual target type (e.g. program, static lib, or dynamic lib, not the OS
		# specific renditions of these, of which multiple may exist (.so and .dylib on
		# OS X for example)
		attr_reader   :type
		def type=( value )
			case value
			when :program , :staticlib , :dynamiclib
				@type = value
			else
				raise ArgumentError , "#{value} is not a valid Target type"
			end
		end

		def initialize( *args )
			case args.size
			when 0
			when 1
				@name = args[0]
			else
				raise ArgumentError , "Expected [0..1] arguments, got #{args.size}"
			end

			@dependancies = []
		end
	end
end
