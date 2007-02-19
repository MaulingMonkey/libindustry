# Copyright (c) 2006-2007 Michael B. Edwin Rickert
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt ) 
#
# $LastChangedBy$ - $LastChangedDate$

require 'builder/toolchains/base'

module Industry
	class Target
		attr_writer :msvc8_project_filename
		def msvc8_filename()
			@msvc8_filename || (self.name + {:program=>".exe",:staticlib=>".lib",:dynamiclib=>".dll"}[self.type])
		end
		def msvc8_project_filename()
			@msvc8_project_filename || (self.name + ".msvc8.vcproj")
		end
	end

	class MSVC8_ToolChain < Base_ToolChain
		def build_target(target)
			super(target)
			puts "Should've exported #{target.msvc8_project_filename}"
			puts "Should've built #{target.msvc8_filename}"
		end
	end
end
