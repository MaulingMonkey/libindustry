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
		def initialize( filename , target )
		end
		def build()
		end
		def clean()
		end
	end
	
	command  = nil
	filename = nil
	target   = nil
		
	case ARGV.size
	when 0, 1, 2, 3
		command  = ARGV[0] || "build"
		target   = ARGV[1] || "all"
		filename = ARGV[2] || "industry-project.rb"
	else
		puts "Error:  Expected 0..3 arguments, got #{ARGV.size}"
		puts "Usage:"
		puts "    #{$0} [command] [target] [filename]"
		puts "Examples:"
		puts "    #{$0} build"
		puts "    #{$0} clean all"
		puts "    #{$0} build all oddly-named.rb"
		exit(-1)
	end

	project = Project.new(filename,target)

	case command
	when "build";   project.build
	when "clean";   project.clean
	else
		puts "Unknown command: #{command}"
		puts "Did not modify   #{target}"
	end
end
