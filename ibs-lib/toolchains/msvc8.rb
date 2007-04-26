#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

require 'toolchains/msvc8/project.rb'
# require 'toolchains/msvc8/ruby_in_steel.rb'
require 'toolchains/msvc8/solution.rb'
require 'toolchains/msvc8/utility.rb'
require 'ftools'
require 'uuid'

[ Library , Program ].each do |needs_uuid|
	needs_uuid.class_eval do
		attr_writer :uuid
		def uuid(); @uuid ||= UUID.new; @uuid; end
	end
end

class MSVC8_Toolchain
	def initialize()
		@solution_configs = [ "Debug|Win32" , "Release|Win32" ]
		@solution_uuid = "\{#{UUID.new.to_s.upcase}\}"
	end
	def scan( *projects )
		projects.flatten!
		projects.each do |project|
			case project
			when Program, Library; scan_cpp_project_file( root_filename_of(project) , project ) unless !File.exists? root_filename_of(project)
			end
		end
		
		scan_solution_file( root_filename_of("msvc8.sln") , projects ) unless !File.exists? root_filename_of("msvc8.sln")
	end
	def export( *projects )		
		projects.flatten!
		projects.each do |project|
			case project
			when Program, Library; export_cpp_project_file( root_filename_of(project) , project )
			end
		end
		
		export_solution_file( root_filename_of("msvc8.sln") , projects )
	end
end

$toolchain = MSVC8_Toolchain.new
