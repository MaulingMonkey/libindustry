# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

require 'ibs-lib/utility'

module Industry
	class Solution
		def initialize( name )
			@name           = name
			@projects       = {}
			@uuid           = UUID.create #  TODO:  Scan files for UUID reuse
		end
		attr_writer_once :msvc80_solution_filename
		attr_writer_once :msvc80_solution_directory
		attr_reader_defaults( :msvc80_solution_filename  ) { "#{@name}.sln" }
		attr_reader_defaults( :msvc80_solution_directory ) { File.dirname( self.msvc80_solution_filename ) }


		attr_accessor :projects , :msvc80_solution_filename , :gnu_makefile_filename
		attr_reader :uuid
	end
end

module Kernel
	def solution( id )
		$ibs_solutions ||= {}
		$ibs_solutions[id] ||= Industry::Solution.new(id)
		solution = $ibs_solutions[id]

		if block_given?
			$ibs_focus.push solution
			yield
			$ibs_focus.pop
		end

		solution
	end
	def msvc80_solution_filename( name ); $ibs_focus.last.msvc80_solution_filename=name; end
	def gnu_makefile_filename(    name ); $ibs_focus.last.gnu_makefile_filename=   name; end
end
