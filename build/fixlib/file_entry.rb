#  $LastChangedDate$
#  $LastChangedBy$

class FileMetaData
	def auto_move; @auto_move = true; end
	def auto_add ; @auto_add  = true; end
	def ignore
		@ignore    = true
		@auto_move = false
		@auto_add  = false
	end

	def auto_move?; @auto_move && !@ignore; end
	def auto_add? ; @auto_add  && !@ignore; end
	def ignore?   ; @ignore; end
	
	def move_to( dest ); @move_to = dest; end
	def moved?; @move_to; end
	def moved_to; @move_to; end
end

class FileList
	include Enumerable

	def initialize; @data = {}; end
	def each(&b); @data.each(&b); end
	def [] ( filename )       ; @data[File.expand_path(filename.to_s)] ||= FileMetaData.new; end
	def []=( filename, value ); @data[File.expand_path(filename.to_s)]   = value           ; end
end

$files ||= FileList.new

def auto_move( glob ); Dir[glob].each {|file| $files[file].auto_move }; end
def auto_add ( glob ); Dir[glob].each {|file| $files[file].auto_add  }; end
def ignore   ( glob ); Dir[glob].each {|file| $files[file].ignore    }; end

def fix_filepaths
	puts
	$files.each do |filename,meta|
		next unless meta.moved?
		File.rename( filename, meta.moved_to.to_s )
		puts "Moved #{filename} -> #{meta.moved_to}"
	end
end
