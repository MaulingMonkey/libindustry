require 'maker/dependancy'
require 'maker/source'
require 'maker/error'
module Maker
	class Maker
		attr_accessor :target
		attr_accessor :version
		attr_accessor :sources
		attr_accessor :include_dirs
		attr_accessor :frameworks
		attr_accessor :error_log
		attr_accessor :defines
		attr_accessor :libraries
		attr_accessor :library_dirs
		def list_config()
			puts "Target:              #{@target}"
			puts "Version:             #{@version}"
			puts "Error log:           #{@error_log}"
			puts "Sources:             #{@sources.inspect}"
			puts "Defines:             #{@defines.inspect}"
			puts "Include Directorys:  #{@include_dirs.inspect}"
			puts "Frameworks:          #{@frameworks.inspect}"
			puts "Libraries:           #{@libraries.inspect}"
			puts "Library Directorys:  #{@library_dirs.inspect}"
		end
		def initialize()
			@target = "program"
			@error_log = "errors.log"
			@sources = []
			@defines = []
			@include_dirs = []
			@frameworks = []
			@libraries = []
			@library_dirs = []
			@version = "0.0"
		end
		def run()
			begin
				if File.exists?( @error_log ) then
					puts "Clearing #{@error_log}..."
					File.delete( @error_log )
				end
				puts "Preparing to compile..."
				sources = @sources.collect { |glob| Dir[ glob ] }
				sources.flatten!
				sources.collect! { |file| Source.new( self , file ) }
				
				puts "Compiling..."
				sources.each do |source|
					source.update
				end
				
				puts "Linking..."
				link( sources.collect{|s| s.object} )
				
				puts "Done!"
				
				if File.exists?( @error_log ) and File.size( @error_log ) != 0
					puts "Error log not empty, probably warnings, opening..."
					if not system( "open #{@error_log}" ) then
						puts "Couldn't open #{error_log}"
					end
				end
			rescue CompileError , LinkError , DependancyError => builderror
				puts "Bailing out: #{builderror.to_str}"
				puts "Opening #{@error_log}..."
				if not system( "open #{@error_log}" ) then
					puts "Error opening #{@error_log}.  Sorry bud, you're on your own!"
				end
			end
		end
		def link( objects )
			return do_link( objects ) unless File.exists?( @target )
			objects.each do |o|
				return do_link( objects ) if File.mtime(o) > File.mtime(@target)
			end
		end
		def do_link( objects )
			puts "Linking program..."
			objects.collect! { |o| "#{o} " }
			command = "g++ #{flags} #{objects} -o #{@target} 2>> #{@error_log}"
			puts( command )
			if not system( command ) then
				raise LinkError.new( @target )
			end
		end
		def flags()
			string = ""
			@frameworks.each { |framework| string += "-framework #{framework} " }
			@library_dirs.each { |dir| string += "-L#{dir} " }
			@libraries.each { |library| string += "-l#{library} " }
			if target =~ /\.dylib\Z/ then string += "-dynamiclib" end
			if target =~ /\.so\Z/    then string += "-shared" end
			if target =~ /\.dll\Z/   then string += "-shared" end #TODO: Win32 only?
			string
		end
	end
end
