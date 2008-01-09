#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$
#
#  NOTE:  This file is currently nothing but dead code.

class MSVC8_Toolchain
	def export_ruby_in_steel_project_file( filename , project )
		File.open( filename , File::CREAT | File::WRONLY | File::TRUNC ) do |file|
			file.puts "<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">"
			%w[ Debug Release ].each do |config|
				file.puts "  <PropertyGroup Condition=\" '$(Configuration)' == '#{config}' \">"
				file.puts "    <Name>#{config}</Name>"
				file.puts "  </PropertyGroup>"
			end
			lines = <<-END
				  <UsingTask TaskName="RML.Steel.SteelRubyBuild" AssemblyName="SteelBuild, Version=1.0.0.0, Culture=neutral, PublicKeyToken=8a42f26dbbcba2be, processorArchitecture=MSIL" />
				  <UsingTask TaskName="RML.Steel.SteelRailsBuild" AssemblyName="SteelBuild, Version=1.0.0.0, Culture=neutral, PublicKeyToken=8a42f26dbbcba2be, processorArchitecture=MSIL" />
				  <Target Name="BuildRails" Inputs="@(EmbeddedRuby)" Outputs="$(MSBuildProjectDirectory)\\SyntaxCheck\\%(Identity)">
				    <MakeDir Directories="SyntaxCheck" />
				    <SteelRailsBuild ERBFile="@(EmbeddedRuby)" ProjectDirectory="$(MSBuildProjectDirectory)" ERBProcessor="$(ERBProcessor)" ERBFlags="$(ERBFlags)" ERBLibraryFiles="$(ERBLibraryFiles)" ERBTimeout="$(ERBTimeout)" />
				  </Target>
				  <Target Name="BuildRuby" Inputs="@(Ruby)" Outputs="$(MSBuildProjectDirectory)\\SyntaxCheck\\%(Identity)">
				    <MakeDir Directories="SyntaxCheck" />
				    <SteelRubyBuild RubyFile="@(Ruby)" ProjectDirectory="$(MSBuildProjectDirectory)" RubyInterpreter="$(RubyInterpreter)" RubyInterpreterFlags="$(RubyInterpreterFlags)" RubyLibraryFiles="$(RubyLibraryFiles)" RubyLibraryPaths="$(LibraryPath)" RubyTimeout="$(RubyTimeout)" />
				  </Target>
				  <Target Name="Clean">
				    <RemoveDir Directories="$(MSBuildProjectDirectory)\\SyntaxCheck" />
				  </Target>
				  <Target Name="Build" DependsOnTargets="$(BuildDependsOn)" Inputs="@(Ruby); @(EmbeddedRuby)" Outputs="$(MSBuildProjectDirectory)\\SyntaxCheck\\%(Identity)">
				  </Target>
				  <Target Name="Clean">
				    <RemoveDir Directories="$(MSBuildProjectDirectory)\\SyntaxCheck" />
				  </Target>
				  <PropertyGroup>
				    <RebuildDependsOn>
				      Clean;
				      Build;
				    </RebuildDependsOn>
				    <BuildDependsOn>
				      BeforeBuild;
				      BuildRuby; 
				      BuildRails;
				      AfterBuild;
				    </BuildDependsOn>
			END
			lines.each_line do |line|
				file.puts line.gsub(/^\t\t\t\t/,'')
			end
			file.puts "    <ProjectGuid>\{#{project.uuid.to_s}\}</ProjectGuid>"
			file.puts "    <AssemblyName>#{project.name}</AssemblyName>"
			file.puts "    <Name>#{project.name}</Name>"
			file.puts "    <RootNamespace>#{project.name}</RootNamespace>"
			file.puts "    <SynchronizationExcludeHiddenFiles>True</SynchronizationExcludeHiddenFiles>"
			file.puts "    <SynchronizeExcludeNoExtension>False</SynchronizeExcludeNoExtension>"
			file.puts "    <RubyTimeout>60</RubyTimeout>"
			file.puts "    <ERBTimeout>60</ERBTimeout>"
			file.puts "  </PropertyGroup>"
			directories = []
			project.sources.each do |source|
				source_dirs = []
				dir = File.dirname( "#{source}" )
				while dir != '.'
					source_dirs.unshift dir
					dir = File.dirname dir
				end
				directories += source_dirs
			end
			directories.uniq!
			
			file.puts "  <ItemGroup>"
			directories.each do |dir|
				file.puts "    <Folder Include=\"#{dir}\" />"
			end
			file.puts "  </ItemGroup>"
			
			file.puts "  <ItemGroup>"
			project.sources.each do |source|
				file.puts "    <Ruby Include=\"#{$inverse_project_root}/#{source}\" />"
			end
			file.puts "  </ItemGroup>"
			
			lines = <<-END
				  <Target Name="Rebuild" DependsOnTargets="$(RebuildDependsOn)" Inputs="@(Ruby)" Outputs="$(MSBuildProjectDirectory)\SyntaxCheck\%(Identity)">
				  </Target>
				  <Target Name="BeforeBuild" Condition="'$(PreBuildEvent)'!='' ">
				    <Exec Command="$(PreBuildEvent)" />
				  </Target>
				  <Target Name="AfterBuild" Condition="'$(PostBuildEvent)'!='' ">
				    <Exec Command="$(PostBuildEvent)" />
				  </Target>
				</Project>
			END
			lines.each_line do |line|
				file.puts line.gsub(/^\t\t\t\t/,'')
			end
		end
	end
end
