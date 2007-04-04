#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

class MSVC8_Toolchain
	def filename_of( project )
		case project
		when String          ;  project
		when Program, Library; "msvc8_#{project.name}.vcproj"
		end
	end
	def root_filename_of( project ); File.expand_path( filename_of( project ) , $project_root ); end
end
