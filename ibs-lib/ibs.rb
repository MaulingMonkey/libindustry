# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

require 'ibs-lib/utility'
require 'ibs-lib/toolchains/*'
require 'ibs-lib/concepts/*'

module Industry
	setttings = {
		#  Defaults:
		:command  => 'build',  #  Never really used (yet?) but this would be the default (?)
		:target   => 'all',
		:filename => 'industry-solution.rb'
	}

	# ibs <command> [<target>] [<filename>]
	case ARGV.size
	when 1, 2, 3 # ibs <command> [<target> [<filename>]]
		settings[:command ] = ARGV[0]
		settings[:target  ] = ARGV[1] || settings[:target]
		settings[:filename] = ARGV[2] || settings[:filename]
	else
		puts ""
		puts "Usage: #{$0} <command> [<target> [<filename>]]"
		puts ""
		puts "  <command>"
		puts "     build :  update any IDE files and build the project"
		puts "     clean :  clean up temporary files"
		puts "     export:  export IDE files for standalone building"
		puts ""
		puts "  <target>    (default 'all')"
		puts "     all   :  all targets found in <filename>"
		puts "     *     :  any valid project described in <filename>"
		puts "     *:*   :  any valid project:target combination described in <filename>"
		puts ""
		puts "  <filename>  (default 'industry-solution.rb')"
		puts "     The ruby script to load which describes the IBS project/solution"
		exit 1
	end

	load 'industry-solution.rb'
end
