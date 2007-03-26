# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

module Industry
	class MSVC8_Toolchain
		def export_solution( solution )
			File.new( solution.msvc80_solution_filename , "rw" ) do |file|
				file.puts "Microsoft Visual Studio Solution File, Format Version 9.00"
				solution.projects.each do |project|
					# FIXME:  Replace magic constants with something:
					sln_guid      = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"
					sln_basedir   = "#{solution.name}"
					proj_guid     = "{45E6D470-F236-499B-8786-CDB9D3B5D8E1}"
					proj_filename = "#{sln_basedir}\#{project.name}.vcproj"


					file.puts "Project(\"#{sln_guid}\") = \"#{project.name}\", \"#{proj_filename}\", \"#{proj_guid}\""
					file.puts "EndProject"
				end
				file.puts "Global"
				file.puts "	GlobalSection(SolutionConfigurationPlatforms) = preSolution"

				configs = [ "Debug - All Warnings|Win32" , "Debug|Win32" , "Release|Win32" ]
				configs.each do |config|
					file.puts "		#{config} = #{config}"
				end
				file.puts "	EndGlobalSection"
				file.puts "	GlobalSection(ProjectConfigurationPlatforms) = postSolution"
				file.puts "	EndGlobalSection"
				file.puts "	GlobalSection(SolutionProperties) = preSolution"
				file.puts "		HideSolutionNode = FALSE"
				file.puts "	EndGlobalSection"
				file.puts "EndGlobal"
			end
		end
	end
end
