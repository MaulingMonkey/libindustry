# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

require 'ibs-lib/uuid'

def require( *patterns )
	results = patterns.collect do |pattern|
		paths = Dir[pattern.gsub(/\\/,'/')]
		if paths.empty? then
			super pattern
		else
			successful = false

			statuses = paths.collect do |path|
				begin
					result = super path
					successful = true
					result
				rescue LoadError
					nil # assumed to be a directory or the like
				end
			end.compact

			raise LoadError, "no such pattern to match/load -- #{pattern}" unless successful

			return *statuses
		end
	end
	return *results
end

def env_list( name )
	ENV[name].split(';')
end

def env( name )
	ENV[name]
end

def attr_writer_once( name )
	class_eval <<-"end_eval"
		def #{name}=(value)
			raise ArgumentError , \"#{name} already set\" if @#{name}
			@#{name} = value
		end
	end_eval
end

def attr_reader_defaults( name , &expr )
	define_method( "#{name}_default_" , &expr )
	class_eval <<-"end_eval"
		def #{name}()
			@#{name} || #{name}_default_
		end
	end_eval
end
