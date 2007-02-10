require 'maker/dependancy'
require 'maker/error'

module Maker
	class Source
		attr_reader :name
		attr_reader :maker

		def initialize( maker , name )
			@maker = maker
			@name = name
			@deps = DependancyFile.new( self )
		end
		
		def include_dirs()
			@maker.include_dirs
		end
		
		def object()
			File.join( File.dirname( @name ) , "." + File.basename( @name ).chomp( ".cpp" ) + ".o" )
			#@name.chomp( ".cpp" ) + ".o" #TODO: Add more extensions
		end
		
		def update()
			return if up_to_date?
			puts "Compiling #{self.name}..."
			do_update
		end
		
		def force_update()
			@deps.force_update
			puts "Forced compiling of #{self.name}..."
			do_update
		end
		
		def do_update()
			command = "g++ #{flags} -c #{self.name} -o #{self.object} 2>> #{maker.error_log}"
			puts( command )
			if not system( command ) then
				raise CompileError.new( self.name )
			end
		end
		
		def up_to_date?()
			@deps.update
			return false unless File.exists?( object )
			@deps.each do | dep |
				#return false if not File.exists?( dep )
				return false if File.mtime( dep ) > File.mtime( object )
			end
			return true
		end
		
		def flags()
			string = "-Wall"
			self.maker.include_dirs.each { |dir| string += " -I#{dir}" }
			self.maker.defines.each { |define| string += " -D#{define}" }
			string += " "
			string
		end
	end
end
