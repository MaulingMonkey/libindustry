# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

require 'ibs-lib/concepts/solution'
require 'ibs-lib/utility'

module Industry
	class Project
		def initialize( solution , name )
			@solution       = solution
			@name           = name
			@dependancies   = []
			@sources        = []
			@uuid           = UUID.create #  TODO:  Scan files for UUID reuse
		end

		attr_writer_once :msvc80_project_filename
		attr_writer_once :gnu_makefile_target
		attr_reader_defaults( :msvc80_project_filename ) { File.join(@solution.msvc80_solution_directory,"#{@name}.vcproj") }
		attr_reader_defaults( :gnu_makefile_target     ) { @name }

		attr_accessor :dependancies, :sources
		attr_reader :name, :uuid
	end
end

module Kernel
	def project( name )
		solution = $ibs_focus.reverse.find { |s| s.kind_of? Industry::Solution }
		ibs_projects = solution.projects
		ibs_projects[name] ||= Industry::Project.new(solution,name)
		project = ibs_projects[name]

		if block_given?
			$ibs_focus.push project
			yield
			$ibs_focus.pop
		end

		project
	end
	def msvc80_project_filename( filename ); $ibs_focus.last.msvc80_project_filename = filename; end
	def gnu_makefile_target(     name     ); $ibs_focus.last.gnu_makefile_target     = name    ; end
	def dependancy(              dep      ); $ibs_focus.last.dependancies.push   dep           ; end
	def sources(                 expr     ); $ibs_focus.last.sources.push        *Dir[ expr ]  ; end
end
