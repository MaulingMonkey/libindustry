require 'uuid'

class Script
	def initialize( name , language )
		@name         = name.to_s
		@language     = language
		@dependancies = []
		@sources      = []
		@uuid         = UUID.new
	end
	attr_accessor :dependancies, :sources, :start
	attr_reader :name, :language, :uuid
end

def script( name , language )
	if !block_given?
		raise ArgumentError , "Expected a block"
	else
		raise ArgumentError , "Multiple definitions of script #{name}" unless !$scripts[name]
		$focus = $scripts[name] = Script.new( name , language )
		yield
		$focus = nil
	end
end
