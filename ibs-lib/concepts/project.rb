# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

require 'ibs-lib/uuid'

module Industry
	class Solution; end
	class Project
		def initialize( name )
			@name           = name
			@dependancies   = []
			@sources        = []
			@uuid           = UUID.create #  TODO:  Scan files for UUID reuse
		end

		attr_accessor :name, :dependancies, :sources , :msvc80_project_filename , :gnu_makefile_target
		attr_reader :uuid
	end
end

module Kernel
	def project( name )
		ibs_projects = $ibs_focus.reverse.find { |s| s.kind_of? Industry::Solution }.projects
		ibs_projects[name] ||= Industry::Project.new(name)
		project = ibs_projects[name]

		if block_given?
			$ibs_focus.push project
			yield project
			$ibs_focus.pop
		end

		project
	end
	def dependancy( dep ); $ibs_focus.last.dependancies.push dep; end
	def sources( expr ); $ibs_focus.last.sources.push *Dir[ expr ]; end
	def msvc80_project_filename( filename ); $ibs_focus.last.msvc80_project_filename=filename; end
	def gnu_makefile_target( name ); $ibs_focus.last.gnu_makefile_target=name; end
end
