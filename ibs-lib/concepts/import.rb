# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

module Industry
	class Import
		def initialize( id )
			@include_directories ||= []
			@library_directories ||= []
			@library_basenames   ||= [] # alternatives
		end
		attr_accessor :include_directories , :library_directories , :library_basenames
	end
end

module Kernel
	def import( id )
		$ibs_imports ||= {}
		$ibs_imports[id] ||= Industry::Import.new(id)
		import = $ibs_imports[id]

		if block_given?
			$ibs_focus.push import
			yield import
			$ibs_focus.pop
		end

		import
	end
	def add_include_directories( *list ); $ibs_focus.last.include_directories.push( *list ); end
	def add_library_directories( *list ); $ibs_focus.last.library_directories.push( *list ); end
	def add_library_basename(     name ); $ibs_focus.last.library_basenames.push( name );    end
end
