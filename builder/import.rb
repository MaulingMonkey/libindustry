#  Copyright (c) 2007 Michael B. Edwin Rickert
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$
#  $LastChangedDate$

require 'builder/globals'

module Industry
	class Import
		def initialize(id)
			@id = id
			@include_directories = []
			@library_directories = []
			@library_basename = nil
		end


		attr_reader :include_directories, :library_directories
		def library_basename(); @library_basename || @id; end


		def add_include_directories(*list)
			include_directories.push *list
		end
		def add_library_directories(*list)
			library_directories.push *list
		end
		def add_library_basename(name)
			if @library_basename != nil then
				raise ArgumentError, "Library basename already set!"
			else
				@library_basename = name
			end
		end
	end
end
