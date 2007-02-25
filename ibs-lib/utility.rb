# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

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
