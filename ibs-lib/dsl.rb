def add_include_paths( list )
	raise ArgumentError , "Must be used in the context of a dependancy, program, or library" unless $focus
	
	if list.kind_of? String
		$focus.include_paths += list.split(';')
	else
		raise ArgumentError , "Expected a String, got a #{list.class}"
	end
end

def add_library_paths( list )
	raise ArgumentError , "Must be used in the context of a dependancy, program, or library" unless $focus
	
	if list.kind_of? String
		$focus.library_paths += list.split(';')
	else
		raise ArgumentError , "Expected a String, got a #{list.class}"
	end
end

def add_library( name )
	raise ArgumentError , "Must be used in the context of a dependancy, program, or library" unless $focus
	
	if name.kind_of? String
		$focus.libraries += [ name ]
	else
		raise ArgumentError , "Expected a String, got a #{list.class}"
	end
end

def sources( list )
	raise ArgumentError , "Must be used in the context of a program or library" unless ($focus && !$focus.kind_of?(Dependancy))
	
	if list.kind_of? String
		$focus.sources += Dir[ list ]
	elsif list.respond_to? :each
		list.each {|entry| sources(entry)}
	else
		raise ArgumentError , "Expected a String or Array, got a #{list.class}"
	end
end
