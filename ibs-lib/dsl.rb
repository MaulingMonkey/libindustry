#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

def add_include_paths( *lists )
	raise ArgumentError , "Must be used in the context of a dependancy, program, or library" unless ($focus && $focus.respond_to?('include_paths='))
	
	lists.flatten.each do |list|
		case list
		when String
			$focus.include_paths += list.split(';')
		else
			raise ArgumentError , "Expected a String, got a #{list.class}"
		end
	end
end

def add_library_paths( *lists )
	raise ArgumentError , "Must be used in the context of a dependancy, program, or library" unless ($focus && $focus.respond_to?('library_paths='))
	
	lists.flatten.each do |list|
		case list
		when String
			$focus.library_paths += list.split(';')
		else
			raise ArgumentError , "Expected a String, got a #{list.class}"
		end
	end
end

def add_library( name )
	raise ArgumentError , "Must be used in the context of a dependancy, program, or library" unless ($focus && $focus.respond_to?('libraries='))
	
	if name.kind_of? String
		$focus.libraries += [ name ]
	else
		raise ArgumentError , "Expected a String, got a #{list.class}"
	end
end

def sources( list )
	raise ArgumentError , "Must be used in the context of a program or library" unless ($focus && $focus.respond_to?('sources='))
	
	if list.kind_of? String
		$focus.sources += Dir[ list ]
	elsif list.respond_to? :each
		list.each {|entry| sources(entry)}
	else
		raise ArgumentError , "Expected a String or Array, got a #{list.class}"
	end
end


def start( file )
	raise ArgumentError , "Must be used in the context of a script" unless ($focus && $focus.respond_to?('start='))
	
	$focus.start = file
end

def msvc_supress_warnings( *list )
	raise ArgumentError, "Must be used in the context of a Program or Library" unless ($focus && $focus.respond_to?('msvc_supressed_warnings='))
	$focus.msvc_supressed_warnings += list.flatten
end

def msvc_warning_level( level )
	raise ArgumentError, "Warning level should be between 1 and 4" unless (1..4) === level
	raise ArgumentError, "Must be used in the context of a Program or Library" unless ($focus && $focus.respond_to?('msvc_warning_level='))
	$focus.msvc_warning_level = level
end
def project_root( root )
	$project_root = root
end
