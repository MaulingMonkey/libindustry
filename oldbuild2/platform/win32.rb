#  Copyright (c) 2007 Michael B. Edwin Rickert
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$
#  $LastChangedDate$

module Industry
	class Win32Platform
		def env_list(variable, delim=nil)
			delim ||= ';'
			ENV[variable].split(delim)
		end
	end

	if PLATFORM =~ /mswin32/ then
		$PLATFORM = Win32Platform.new
	end
end
