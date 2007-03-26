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
	settings = {
		#  Defaults:
		:command  => 'update',
		:filename => 'industry-solution.rb'
	}

	# ibs <command>
	case ARGV.size
	when 1
		settings[:command] = ARGV[0]
	else
		puts ""
		puts "Usage: #{$0} <command>"
		puts ""
		puts "  <command>"
		# puts "     build :  build the entire project"  # not yet implemented
		# puts "     test  :  do compile tests"          # not yet implemented
		puts "     export:  update all IDE files without ibs hooks"
		puts "     update:  update all IDE files with    ibs hooks"
		puts "     verify:  error message & code if an update is required"
		exit 1
	end

	$ibs_focus = [] # focus stack, used by concepts/*

	load settings[:filename]
end
