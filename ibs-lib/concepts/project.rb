# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

module Industry
	class Solution; end
	class Project
		def initialize( id )
			@dependancies   ||= []
			@sources        ||= []
		end

		attr_accessor :dependancies, :sources
	end
end

module Kernel
	def project( id )
		ibs_projects = $ibs_focus.reverse.find { |s| s.kind_of? Industry::Solution }.projects
		ibs_projects[id] ||= Industry::Project.new(id)
		project = ibs_projects[id]

		if block_given?
			$ibs_focus.push project
			yield project
			$ibs_focus.pop
		end

		project
	end
	def dependancy( dep ); $ibs_focus.last.dependancies.push dep; end
	def sources( expr ); $ibs_focus.last.sources.push *Dir[ expr ]; end
end
