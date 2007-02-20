#  Copyright (c) 2007 Michael B. Edwin Rickert
#
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$
#  $LastChangedDate$

#  These are used to dispatch various in-block statements
$industry_builder_imports ||= {} # import ids => Import instances
$industry_builder_focus   ||= [] # stack of project/solution/target (innermost == .last)