#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

require 'ftools'
require 'uuid'

class MSVC8_Toolchain
	def initialize()
		@solution_configs = [ "Debug|Win32" , "Release|Win32" ]
		@solution_uuid = "\{#{UUID.new.to_s.upcase}\}"
	end
	def export( *list )
		
		
		list.flatten!
		list.each do |exportee|
			export_project_file( File.expand_path( "msvc8_#{exportee.name}.vcproj" , $project_root ) , exportee )
		end
		
		export_solution_file( File.expand_path( "msvc8.sln" , $project_root ) , list )
	end
	def export_solution_file( filename , list )
		File.open( filename , File::CREAT | File::WRONLY | File::TRUNC ) do |file|
			file.puts "\357\273\277"
			file.puts "Microsoft Visual Studio Solution File, Format Version 9.00"
			file.puts "# Visual Studio 2005"
			
			list.each do |project|
				project_filename = "msvc8_#{project.name}.vcproj"
				project_uuid     = "\{#{project.uuid.to_s.upcase}\}"
				file.puts "Project(\"#{@solution_uuid}\") = \"#{project.name}\", \"#{project_filename}\", \"#{project_uuid}\""
				file.puts "EndProject"
			end
			
			file.puts "Global"
			
			file.puts "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution"
			@solution_configs.each do |config|
				file.puts "\t\t#{config} = #{config}"
			end
			file.puts "\tEndGlobalSection"
			
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
			
			file.puts "\tGlobalSection(SolutionProperties) = preSolution"
			file.puts "\t\tHideSolutionNode = FALSE"
			file.puts "\tEndGlobalSection"
			
			file.puts "EndGlobal"
		end
	end
	def write_project_file_tree_to( tree , file , parent , indent )
		raise ArgumentError, "Expected a hash" unless tree.kind_of? Hash
		tree.each do |name,info|
			if info == :file then
				file.puts "#{indent}<File"
				file.puts "#{indent}\tRelativePath=\"#{parent}#{name}\""
				file.puts "#{indent}\t>"
				file.puts "#{indent}</File>"
			elsif info.kind_of? Hash then
				file.puts "#{indent}<Filter"
				file.puts "#{indent}\tName=\"#{name}\""
				file.puts "#{indent}\t>"
				write_project_file_tree_to( info , file , parent + "#{name}\\" , indent + "\t" )
				file.puts "#{indent}</Filter>"
			else
				raise ArgumentError , "File tree contians non :file/Hash nodes"
			end
		end
	end
	private :write_project_file_tree_to
	
	def export_project_file( filename , project )
		File.open( filename , File::CREAT | File::WRONLY | File::TRUNC ) do |file|
			file.puts '<?xml version="1.0" encoding="Windows-1252"?>'
			file.puts "<VisualStudioProject"
			file.puts "\tProjectType=\"Visual C++\""
			file.puts "\tVersion=\"8.00\""
			file.puts "\tName=\"#{project.name}\""
			file.puts "\tProjectGUID=\"\{#{project.uuid.to_s.upcase}\}\""
			file.puts "\tRootNamespace=\"#{project.name.scan(/\w/).join}\""
			file.puts "\tKeyword=\"Win32Proj\""
			file.puts "\t>"
			
			file.puts "\t<Platforms>"
			file.puts "\t\t<Platform"
			file.puts "\t\t\tName=\"Win32\""
			file.puts "\t\t/>"
			file.puts "\t</Platforms>"
			
			file.puts "\t<ToolFiles>"
			file.puts "\t</ToolFiles>"
			
			file.puts "\t<Configurations>"
			
			project_configs = @solution_configs
			project_configs.each do |config|
				file.puts "\t\t<Configuration"
				file.puts "\t\t\tName=\"#{config}\""
				file.puts "\t\t\tOutputDirectory=\"$(SolutionDir)#{$inverse_project_root}\\$(ConfigurationName)\""
				file.puts "\t\t\tIntermediateDirectory=\"$(ConfigurationName)\""
				file.puts "\t\t\tConfigurationType=\"4\""
				file.puts "\t\t\tCharacterSet=\"1\""
				file.puts "\t\t\t>"
				
				
				
				%w[
					VCPreBuildEventTool
					VCCustomBuildTool
					VCXMLDataGeneratorTool
					VCWebServiceProxyGeneratorTool
					VCMIDLTool
				].each do |tool|
					file.puts "\t\t\t<Tool"
					file.puts "\t\t\t\tName=\"#{tool}\""
					file.puts "\t\t\t/>"
				end
				
				
				
				compiler = []
				compiler << %w[ Name          VCCLCompilerTool ]
				compiler << %w[ Optimization  0                ] if config =~ /Debug/
				compiler << [ 'AdditionalIncludeDirectories' , project.include_paths ]
				
				defines = %w[ WIN32 ]
				if config =~ /Debug/
					defines << '_DEBUG'
				else
					defines << 'NDEBUG'
				end
				if project.kind_of? Library
					defines << '_LIB'
				else
					defines << '_CONSOLE'
				end
				defines += %w[ _CRT_SECURE_NO_DEPRECATE _SCL_SECURE_NO_DEPRECATE ]
				compiler << [ 'PreprocessorDefinitions' , defines.join(';') ]
				
				compiler << %w[ MinimalRebuild                  true ]
				compiler << %w[ BasicRuntimeChecks              3    ] if config =~ /Debug/
				if config =~ /Debug/
					compiler << %w[ RuntimeLibrary              3    ]
				else
					compiler << %w[ RuntimeLibrary              2    ]
				end
				compiler << %w[ UsePrecompiledHeader            0    ]
				compiler << %w[ WarningLevel                    3    ]
				compiler << %w[ Detect64BitPortabilityProblems  true ]
				if config =~ /Debug/
					compiler << %w[ DebugInformationFormat      4    ]
				else
					compiler << %w[ DebugInformationFormat      3    ]
				end
				
				file.puts "\t\t\t<Tool"
				compiler.each do |attribute,value|
					file.puts "\t\t\t\t#{attribute}=\"#{value}\""
				end
				file.puts "\t\t\t/>"
				
				%w[
					VCManagedResourceCompilerTool
					VCResourceCompilerTool
					VCPreLinkEventTool
				].each do |tool|
					file.puts "\t\t\t<Tool"
					file.puts "\t\t\t\tName=\"#{tool}\""
					file.puts "\t\t\t/>"
				end
				
				if project.kind_of? Library
					file.puts "\t\t\t<Tool"
					file.puts "\t\t\t\tName=\"VCLibrarianTool\""
					file.puts "\t\t\t/>"
				elsif project.kind_of? Program
					linker = [ %w[ Name VCLinkerTool ] ]
					linker << [ 'AdditionalDependancies' , '' ]
					if config =~ /Debug/
						linker << %w[ LinkIncremental 2 ]
					else
						linker << %w[ LinkIncremental 1 ]
					end
					linker << [ 'AdditionalLibraryDirectories' , '' ]
					linker << %w[ GenerateDebugInformation true ]
					linker << %w[ SubSystem                1    ]
					if !(config =~ /Debug/)
						linker << %w[ OptimizeReferences   2    ]
						linker << %w[ EnableCOMDATFolding  2    ]
					end
					linker << %w[ TargetMachine            1    ]
					
					file.puts "\t\t\t<Tool"
					linker.each do |attribute,value|
						file.puts "\t\t\t\t#{attribute}=\"#{value}\""
					end
					file.puts "\t\t\t/>"
				else
					raise "WTF??!??"
				end
				
				if project.kind_of? Library
					%w[ VCALinkTool VCXDCMakeTool VCBscMakeTool VCFxCopTool VCPostBuildEventTool ]
				elsif project.kind_of? Program
					%w[ VCALinkTool VCManifestTool VCXDCMakeTool VCBscMakeTool VCFxCopTool
					    VCAppVerifierTool VCWebDeploymentTool VCPostBuildEventTool ]
				else
					raise "WTF?!?!?!?"
				end.each do |tool|
					file.puts "\t\t\t<Tool"
					file.puts "\t\t\t\tName=\"#{tool}\""
					file.puts "\t\t\t/>"
				end
				
				file.puts "\t\t</Configuration>"
			end
			
			file.puts "\t</Configurations>"
			
			
			
			file.puts "\t<References>"
			project.dependancies.each do |dependancy|
				next unless (dependancy.kind_of? Program) || (dependancy.kind_of? Library)
				
				dependancy_uuid = "\{#{dependancy.uuid.to_s.upcase}\}"
				dependancy_path = ".\msvc8_#{dependancy.name}.vcproj"
				
				file.puts "\t\t<ProjectReference"
				file.puts "\t\t\tReferencedProjectIdentifier=\"#{dependancy_uuid}\""
				file.puts "\t\t\tRelativePathToProject=\"#{dependancy_path}\""
				file.puts "\t\t/>"
			end
			file.puts "\t</References>"
			
			
			
			file.puts "\t<Files>"
			filetree = {}
			project.sources.each do |source|
				tokenized = source.split( /[\/]/ )-['.']
				node = filetree
				tokenized[0..-2].each do |token|
					node[token] = {} if !node[token]
					node = node[token]
				end
				node[tokenized[-1]] = :file
			end
			write_project_file_tree_to( filetree , file , "#{$inverse_project_root}\\" , "\t\t" )
			file.puts "\t</Files>"
			
			
			
			file.puts "\t<Globals>"
			file.puts "\t</Globals>"
			
			
			
			file.puts "</VisualStudioProject>"
		end
	end
end

$toolchain = MSVC8_Toolchain.new
