# Copyright (c) 2007 Michael B. Edwin Rickert
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt )
# 
# $LastChangedBy$
# $LastChangedDate$

module Industry
	class Project
		def initialize()
			@dependancies   ||= []
			@sources        ||= []
			@targets        ||= []
			@warnings_level ||= nil
		end

		# From least prefered to most
		@@warning_level_ratings = \
			{ :none    => 0   \
			, :low     => 1   \
			, :medium  => 2   \
			, :high    => 3   \
			, :maximum => 4   \
			}
		def warnings_level()
			@warnings_level || :high
		end
		def warnings_level=( suggested )
			existing_rank  = @@warning_level_ratings[ @warnings_level ]
			suggested_rank = @@warning_level_ratings[ suggested ]
			@warnings_level = suggested if suggested_rank > existing_rank
		end

		attr_accessor :dependancies, :sources, :targets
	end
end
