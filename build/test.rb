# Copyright (c) 2006-2007 Michael B. Edwin Rickert
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt ) 
#
# $LastChangedBy$ - $LastChangedDate$
#
# Temporary pile for testing out code...

require 'target'
require 'toolchains/msvc8'

module Industry
	targets = (1..3).collect {|n| Target.new( "Program #{n}" ) }
	toolchain = MSVC8_ToolChain.new
	toolchain.build( targets )
end
