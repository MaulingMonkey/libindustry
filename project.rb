#!/usr/bin/ruby
#
#  Purpouse:  Facilitate the creation and destruction of project build.rb chains
#

module Industry
	class Project
		def Project.create( name )
			# create a new project
			puts "UNIMPLEMENTED: Project.create"
		end
		def Project.destroy( name )
			# destroy an existing project
			puts "UNIMPLEMENTED: Project.destroy"
		end
		def Project.build( name )
			# load and build an existing project
			puts "UNIMPLEMENTED: Project.build"
		end
	end
	
	
	
	mode = case File.basename( $0 , ".rb" )
		   when "project"; :command
		   else          ; :library
		   end
	
	if mode == :command
		command = nil
		target  = nil
		
		case ARGV.size
		when 1
			command = "build"
			target  = ARGV[0]
		when 2
			command = ARGV[0]
			target  = ARGV[1]
		end

		case command
		when "build";   Project.build(target)
		when "create";  Project.create(target)
		when "destroy"; Project.destroy(target)
		else
			puts "Unknown command: #{command}"
			puts "Did not modify   #{target}"
		end
	end
end
