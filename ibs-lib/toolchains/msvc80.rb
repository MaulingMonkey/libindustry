# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$
#

module Industry
	class MSVC80_Toolchain
		def export_solution( solution )
			File.new( solution.msvc80_solution_filename ,  File::CREAT | File::TRUNC | File::WRONLY ) do |file|
				file.puts "Microsoft Visual Studio Solution File, Format Version 9.00"

				sln_uuid      = solution.uuid
				sln_basedir   = solution.name
				sln_configs = [ "Debug - All Warnings|Win32" , "Debug|Win32" , "Release|Win32" ]


				# -- Enumerates through the projects, lists their ids, filenames, and UUIDS -------------
				solution.projects.each do |project|
					proj_uuid     = project.uuid
					proj_filename = "#{sln_basedir}\#{project.name}.vcproj"


					file.puts "Project(\"#{sln_uuid}\") = \"#{project.name}\", \"#{proj_filename}\", \"#{proj_uuid}\""
					file.puts "EndProject"
				end




				file.puts "Global"


				# -- Enumerates through solution-wide configuration names/ids --------------------------
				file.puts "	GlobalSection(SolutionConfigurationPlatforms) = preSolution"
				sln_configs .each do |config|
					file.puts "		#{config} = #{config}"
				end
				file.puts "	EndGlobalSection"


				# -- Enumerates through each solution-wide configuration for each project --------------
				#    and maps it to their local configuration names
				file.puts "	GlobalSection(ProjectConfigurationPlatforms) = postSolution"
				solution.projects.each do |project|
					proj_configs  = sln_configs
					#  My current libindustry.sln omits a few for some projects
					#  I think this consistency is preferable though --pandamojo

					sln_configs.each do |sln_config|
						proj_configs.each do |config|
							file.puts "		#{proj_uuid}.#{sln_config}.ActiveCfg = #{config}"
							file.puts "		#{proj_uuid}.#{sln_config}.Build.0 = #{config}"
						end
					end
				end
				file.puts "	EndGlobalSection"


				# -- Damned if I know what this section does -- pandamojo ------------------------------
				file.puts "	GlobalSection(SolutionProperties) = preSolution"
				file.puts "		HideSolutionNode = FALSE"
				file.puts "	EndGlobalSection"


				file.puts "EndGlobal"
			end
		end
	end
end
