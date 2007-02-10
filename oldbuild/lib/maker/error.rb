module Maker
	class CompileError < Exception
		def initialize( file )
			@file = file
		end
		def to_str()
			"Error compiling #{@file}"
		end
	end
	class LinkError < Exception
		def initialize( file )
			@file = file
		end
		def to_str()
			"Error linking #{@file}"
		end
	end
	class DependancyError < Exception
		def initialize( file )
			@file = file
		end
		def to_str()
			"Error parsing dependancies #{@file}"
		end
	end
end
