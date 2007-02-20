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

require 'builder/multi-require'
require 'builder/toolchains/*'
require 'builder/platform/*'

module Industry
	class Solution
		def initialize( filename , target )
			@filename = filename
			@target   = target

			$industry_builder_solution ||= self
			raise ArgumentError, "Solution already exists" unless $industry_builder_solution == self
		end
		def build()
		end
		def clean()
		end
		def export()
			# export for IDEs (standalone solution/project which does not depend on the ruby build system)
		end
		def test()
			# Run tests (build unit tests, as well as test compile-fail tests)
		end
	end
	
	command  = nil
	filename = nil
	target   = nil
		
	case ARGV.size
	when 0, 1, 2, 3
		command  = ARGV[0] || "build"
		target   = ARGV[1] || "all"
		filename = ARGV[2] || "industry-solution.rb"
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

	solution = Solution.new(filename,target)

	case command
	when "build";   solution.build
	when "clean";   solution.clean
	when "export";  solution.export
	when "test";    solution.test
	else
		puts "Unknown command: #{command}"
		puts "Did not modify   #{target}"
	end
end
