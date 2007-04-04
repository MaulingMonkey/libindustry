#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

class MSVC8_Toolchain
	def scan_solution_file( file , project )
		File.open( filename , File::RDONLY ) do |file|
			line = file.readlines.find{|l|l.scan(/Project/)}
			uuid = line.scan(/Project\s*(\s*("\{.*?\}")\s*)/).flatten[0] | UUID.new
			@solution_uuid = uuid
			file.rewind
		end
	end
	def export_solution_file( filename , list )
		File.open( filename , File::CREAT | File::WRONLY | File::TRUNC ) do |file|
			export_solution_file_header                file
			export_solution_file_projects_list         file, list
			file.puts "Global"
			export_solution_file_solution_config_list  file
			export_solution_file_project_config_list   file, list
			export_solution_file_solution_properties   file
			file.puts "EndGlobal"
		end
	end
	
	private
	
	def export_solution_file_header( file )
		file.puts "\357\273\277"
		file.puts "Microsoft Visual Studio Solution File, Format Version 9.00"
		file.puts "# Visual Studio 2005"
	end
	def export_solution_file_projects_list( file , list )
		list.each do |project|
			project_filename = filename_of( project )
			next unless project_filename
			project_uuid     = "\{#{project.uuid.to_s.upcase}\}"
			file.puts "Project(\"#{@solution_uuid}\") = \"#{project.name}\", \"#{project_filename}\", \"#{project_uuid}\""
			file.puts "EndProject"
		end
	end
	def export_solution_file_solution_config_list( file )
		file.puts "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution"
		@solution_configs.each do |config|
			file.puts "\t\t#{config} = #{config}"
		end
		file.puts "\tEndGlobalSection"
	end
	def export_solution_file_project_config_list( file , list )
		file.puts "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution"
		list.each do |project|
			project_configs  = @solution_configs
			project_uuid     = "\{#{project.uuid.to_s.upcase}\}"
			
			project_configs.each do |config|
				file.puts "\t\t#{project_uuid}.#{config}.ActiveCfg = #{config}" # NOTE:  Assumes all project configs have corresponding solution configs
				file.puts "\t\t#{project_uuid}.#{config}.Build.0 = #{config}"   # NOTE:  Ditto.
			end
		end
		file.puts "\tEndGlobalSection"
	end
	def export_solution_file_solution_properties( file )
		file.puts "\tGlobalSection(SolutionProperties) = preSolution"
		file.puts "\t\tHideSolutionNode = FALSE"
		file.puts "\tEndGlobalSection"
	end
end
