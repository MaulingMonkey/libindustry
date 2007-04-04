#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

class MSVC8_Toolchain
	def export_cpp_project_file( filename , project )
		File.open( filename , File::CREAT | File::WRONLY | File::TRUNC ) do |file|
			export_cpp_project_file_header        file, project
			export_cpp_project_platforms          file
			export_cpp_project_toolfiles          file
			export_cpp_project_configurations     file, project
			export_cpp_project_references         file, project
			export_cpp_project_files              file, project
			export_cpp_project_file_tail          file, project
		end
	end
	
	private
	
	def export_cpp_project_file_header( file , project )
		file.puts '<?xml version="1.0" encoding="Windows-1252"?>'
		file.puts "<VisualStudioProject"
		file.puts "\tProjectType=\"Visual C++\""
		file.puts "\tVersion=\"8.00\""
		file.puts "\tName=\"#{project.name}\""
		file.puts "\tProjectGUID=\"\{#{project.uuid.to_s.upcase}\}\""
		file.puts "\tRootNamespace=\"#{project.name.scan(/\w/).join}\""
		file.puts "\tKeyword=\"Win32Proj\""
		file.puts "\t>"
	end
	def export_cpp_project_platforms( file )
		file.puts "\t<Platforms>"
		file.puts "\t\t<Platform"
		file.puts "\t\t\tName=\"Win32\""
		file.puts "\t\t/>"
		file.puts "\t</Platforms>"
	end
	def export_cpp_project_toolfiles( file )
		file.puts "\t<ToolFiles>"
		file.puts "\t</ToolFiles>"
	end
	def export_cpp_project_configurations( file , project )
		file.puts "\t<Configurations>"
		
		project_configs = @solution_configs
		project_configs.each do |config|
			export_cpp_project_configuration_head      file , project , config
			export_cpp_project_configuration_tool      file , project , config , *%w[
				VCPreBuildEventTool
				VCCustomBuildTool
				VCXMLDataGeneratorTool
				VCWebServiceProxyGeneratorTool
				VCMIDLTool
			]
			export_cpp_project_configuration_compiler  file , project , config
			export_cpp_project_configuration_tool      file , project , config , *%w[
				VCManagedResourceCompilerTool
				VCResourceCompilerTool
				VCPreLinkEventTool
			]
			case project
			when Library; export_cpp_project_configuration_librarian   file
			when Program; export_cpp_project_configuration_linker      file, project, config
			end
			
			case project
			when Library; export_cpp_project_configuration_tool      file , project , config ,
				*%w[ VCALinkTool VCXDCMakeTool VCBscMakeTool VCFxCopTool VCPostBuildEventTool ]
			when Program; export_cpp_project_configuration_tool      file , project , config ,
				*%w[ VCALinkTool VCManifestTool VCXDCMakeTool VCBscMakeTool VCFxCopTool
					VCAppVerifierTool VCWebDeploymentTool VCPostBuildEventTool ]
			end
			export_cpp_project_configuration_tail( file , project , config )
		end
		
		file.puts "\t</Configurations>"
	end
	def export_cpp_project_configuration_head( file , project , config )
		file.puts "\t\t<Configuration"
		file.puts "\t\t\tName=\"#{config}\""
		file.puts "\t\t\tOutputDirectory=\"$(SolutionDir)#{$inverse_project_root}\\$(ConfigurationName)\""
		file.puts "\t\t\tIntermediateDirectory=\"$(ConfigurationName)\""
		case project
		when Program
			file.puts "\t\t\tConfigurationType=\"1\""
		when Library
			file.puts "\t\t\tConfigurationType=\"4\""
		else
			raise ArgumentError, "Expected a Program or Library, got a #{project.class}."
		end
		file.puts "\t\t\tCharacterSet=\"1\""
		file.puts "\t\t\t>"
	end
	def export_cpp_project_configuration_tool( file , *tools )
		tools.each do |tool|
			case tool
			when String
				# Empty Tool
				# @tool == name
				file.puts "\t\t\t<Tool"
				file.puts "\t\t\t\tName=\"#{tool}\""
				file.puts "\t\t\t/>"
			when Array
				file.puts "\t\t\t<Tool"
				tool.each do |attribute,value|
					file.puts "\t\t\t\t#{attribute}=\"#{value}\""
				end
				file.puts "\t\t\t/>"
			end
		end
	end
	
	
	def export_cpp_project_configuration_defines( project , config )
		defines = %w[ WIN32 ]
		
		case config
		when /Debug/; defines << '_DEBUG'
		else        ; defines << 'NDEBUG'
		end
		
		case project
		when Library; defines << '_LIB'
		else        ; defines << '_CONSOLE'
		end
		
		defines += %w[ _CRT_SECURE_NO_DEPRECATE _SCL_SECURE_NO_DEPRECATE ]
		defines
	end
	def export_cpp_project_configuration_compiler( file , project , config )
		compiler = []
		compiler << %w[  Name                             VCCLCompilerTool  ]
		compiler << %w[  Optimization                     0                 ] if config =~ /Debug/
		compiler <<   [ 'AdditionalIncludeDirectories' , (project.all_include_paths+[$inverse_project_root]).join(';')           ]
		compiler <<   [ 'PreprocessorDefinitions'      ,  export_cpp_project_configuration_defines( project , config ).join(';') ]
		compiler << %w[  MinimalRebuild                   true              ]
		compiler << %w[  BasicRuntimeChecks               3                 ] if config =~ /Debug/
		compiler << case config
		when /Debug/;%w[ RuntimeLibrary                   3                 ]
		else        ;%w[ RuntimeLibrary                   2                 ]
		end
		compiler << %w[  UsePrecompiledHeader             0                 ]
		compiler << %w[  WarningLevel                     3                 ]
		compiler << %w[  Detect64BitPortabilityProblems   true              ]
		compiler << case config
		when /Debug/;%w[ DebugInformationFormat           4                 ]
		else        ;%w[ DebugInformationFormat           3                 ]
		end
		
		export_cpp_project_configuration_tool( file , compiler )
	end
	
	def export_cpp_project_configuration_librarian( file )
		file.puts "\t\t\t<Tool"
		file.puts "\t\t\t\tName=\"VCLibrarianTool\""
		file.puts "\t\t\t/>"
	end
	def export_cpp_project_configuration_linker( file , project , config )
		linker =   [ %w[ Name                            VCLinkerTool ] ]
		linker <<  [    'AdditionalDependencies'       , project.all_libraries.join(' ') ]
		linker << case config
		when /Debug/;%w[ LinkIncremental                 2            ]
		else        ;%w[ LinkIncremental                 1            ]
		end
		linker <<  [    'AdditionalLibraryDirectories' , project.all_library_paths.join(';') ]
		linker <<%w[    GenerateDebugInformation         true         ]
		linker <<%w[    SubSystem                        1            ]
		if !(config =~ /Debug/)
			linker<<%w[ OptimizeReferences               2            ]
			linker<<%w[ EnableCOMDATFolding              2            ]
		end
		linker <<%w[    TargetMachine                    1            ]
		
		export_cpp_project_configuration_tool( file , linker )
	end
	def export_cpp_project_configuration_tail( file , project , config )
		file.puts "\t\t</Configuration>"
	end
		
		
		

	def export_cpp_project_references( file , project )
		file.puts "\t<References>"
		project.all_dependancies.each do |dependancy|
			next unless (dependancy.kind_of? Program) || (dependancy.kind_of? Library)
			
			dependancy_uuid = "\{#{dependancy.uuid.to_s.upcase}\}"
			dependancy_path = ".\\msvc8_#{dependancy.name}.vcproj"
			
			file.puts "\t\t<ProjectReference"
			file.puts "\t\t\tReferencedProjectIdentifier=\"#{dependancy_uuid}\""
			file.puts "\t\t\tRelativePathToProject=\"#{dependancy_path}\""
			file.puts "\t\t/>"
		end
		file.puts "\t</References>"
	end
	def export_cpp_project_files( file , project )
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
		export_cpp_project_file_tree( file , filetree , "#{$inverse_project_root}\\" , "\t\t" )
		file.puts "\t</Files>"
	end
	def export_cpp_project_file_tree( file , tree , parent , indent )
		raise ArgumentError, "Expected a hash, got #{tree.inspect}" unless tree.kind_of? Hash
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
				export_cpp_project_file_tree( file , info , parent + "#{name}\\" , indent + "\t" )
				file.puts "#{indent}</Filter>"
			else
				raise ArgumentError , "File tree contians non :file/Hash nodes"
			end
		end
	end
	def export_cpp_project_file_tail( file , project )
		file.puts "\t<Globals>"
		file.puts "\t</Globals>"
		file.puts "</VisualStudioProject>"
	end
end
