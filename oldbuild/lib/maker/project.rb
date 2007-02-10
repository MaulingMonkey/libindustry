require 'maker/maker'

module Maker
	class Project
		def initialize( directory )
			directory = File.expand_path( directory )
			raise ArgumentError , "#{directory} Does not exist or is not a directory!" unless File.directory?( directory )
			
			@maker = Maker.new
			@directory = directory.chomp("/")
			@project_file = @directory + "/.project"
			
			create_project_file_if_missing()
			read_project_file()
			@maker.list_config()
		end
		
		def build()
			@maker.run()
		end
		
		def create_project_file_if_missing()
			if !File.exists?( @project_file ) then
				puts "=== Project file not found, creating a default one ==="
				File.open( @project_file , File::CREAT | File::EXCL | File::WRONLY ) do | file |
					target = File.basename( @directory )
					file.puts "TARGET = #{target}"
					file.puts "VERSION = 0.0"
					file.puts "SOURCES = *.cpp , src/*.cpp"
					file.puts "INCLUDE_DIRS = ./ , ./src , /opt/local/include"
					file.puts "LIBRARY_DIRS = /opt/local/lib"
				end
			end
		end
		
		def read_project_file()
			raise ArgumentError.new( "Directory does not contain a project file!" ) unless File.exists?( @project_file )
			File.open( @project_file ) do | file |
				file.each_line do | line |
					key , value = line.strip.split( "=" ).collect { |v| v.strip }
					case key
					when "TARGET"
						@maker.target = value
					when "VERSION"
						@maker.version = value
					when "ERROR_LOG"
						@maker.error_log = value
					when "SOURCES"
						@maker.sources += value.split(",").collect { |v| v.strip }
					when "FRAMEWORKS"
						@maker.frameworks += value.split(",").collect { |v| v.strip }
					when "INCLUDE_DIRS"
						@maker.include_dirs += value.split(",").collect { |v| v.strip }
					when "LIBRARY_DIRS"
						@maker.library_dirs += value.split(",").collect { |v| v.strip }
					when "DEFINES"
						@maker.defines += value.split(",").collect { |v| v.strip }
					when "LIBRARIES"
						@maker.libraries += value.split(",").collect { |v| v.strip }
					else
						print "Warning: Unrecognized key #{key}"
					end
				end
			end
		end
	end
end
