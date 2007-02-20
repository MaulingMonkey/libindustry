#  Copyright (c) 2007 Michael B. Edwin Rickert
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$
#  $LastChangedDate$

module Industry
	class Solution
		def initialize(name)
			@name     = name
			@projects = {}
		end
		def add_project(project)
			@projects << {project.name => project}
		end
		def build(*targets)
			targets.each do |target|
				if target == 'all' then
					@projects.each do |name,project|
						project.build
					end
				else
					project = projects[target]
					if project then
						project.build
					else
						raise ArgumentError, "No such target: #{project}"
					end
				end
			end
		end
	end
end
