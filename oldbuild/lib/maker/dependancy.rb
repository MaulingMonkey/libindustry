require 'maker/error'

module Maker
	class DependancyFile
		def initialize( source )
			@source = source
		end
		
		def maker()
			@source.maker
		end
		
		def name()
			File.join( File.dirname( @source.name ) , "." + File.basename( @source.name ) + ".d" )
		end
		
		def update()
			return if up_to_date?
			puts "Updating dependancy file: #{self.name} ..."
			do_update
		end
		
		def force_update()
			puts "Forced updating of dependancy file: #{self.name} ..."
			do_update
		end
		
		def up_to_date?()
			return false if not File.exists?( self.name )
			return false if File.mtime( self.name ) < File.mtime( @source.name )
			File.open( self.name ) do | file |
				file.each_line do | line |
					dependancies = line.split(/:/)[1]             # we ignore the target ([0])
					dependancies = line unless dependancies       # if not target:deps, must be continuation
					dependancies = dependancies.strip.chomp( "\\" ).strip.split(/ /)  # cleanup and convert to array
					
					dependancies.each do |dependancy|
						return false if not File.exists?( dependancy )
						return false if File.mtime( dependancy ) > File.mtime( self.name )
					end
				end
			end
			
			return true                                           # everything up-to-date
		end
		
		def each( & block )
			File.open( self.name ) do | file |
				file.each_line do | line |
					dependancies = line.split(/:/)[1]
					dependancies = line unless dependancies
					dependancies = dependancies.strip.chomp( "\\" ).strip.split(/ /)
					dependancies.each { |d| yield d }
				end
			end
		end
		
		def do_update()
			command = "g++ -c -MM #{@source.name} -MF #{self.name} #{@source.flags} 2>>#{self.maker.error_log}"
			puts( command )
			if not system( command )
				File.delete( self.name )
				raise DependancyError.new( self.name )
			end
			#TODO: Make unsilent?
		end
	end
end
