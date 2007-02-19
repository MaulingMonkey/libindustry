#!/usr/bin/ruby
#
#  Copyright (c) 2007 Michael B. Edwin Rickert
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$
#  $LastChangedDate$
#
#  Purpouse:  Build the project.
#

module Industry
	class Project
		def initialize( filename )
		end
	end
	
	command = nil
	target  = nil
		
	case ARGV.size
	when 1
		command = "build"
		target  = ARGV[0]
	when 2
		command = ARGV[0]
		target  = ARGV[1]
	else
		puts "Expected 1 or 2 arguments -- got #{ARGV.size}"
		exit(-1)
	end

	project = Project.new(target)

	case command
	when "build";   project.build
	when "clean";   project.clean
	else
		puts "Unknown command: #{command}"
		puts "Did not modify   #{target}"
	end
end
