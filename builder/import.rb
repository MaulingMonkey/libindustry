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
		end
		def add_include_directories(*list)
			raise "implement me!"
		end
		def add_library_directories(*list)
			raise "implement me!"
		end
		def add_library_basename(name)
			raise "implement me!"
		end
	end
end

module Kernel
	def import(id)
		if block_given? then
			$industry_builder_imports[id] ||= Import.new(id)
			$industry_builder_focus.push $industry_builder_imports[id]
			yield
			$industry_builder_focus.pop
		else
			$industry_builder_imports[id]
		end
	end
end
