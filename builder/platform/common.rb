#  Copyright (c) 2007 Michael B. Edwin Rickert
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$
#  $LastChangedDate$

module Industry
	#  Forwarding functions

	#  Note: delim=nil indicates the OS specific deliminator should be used.
	def env_list(variable,delim=nil); $PLATFORM.env_list(variable,delim); end
end
