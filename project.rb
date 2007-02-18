def dependancy(name_or_project); end
def import(name,&block); end
def group(name,&block); end
def project(name,&block); end
def msvc80_solution_filename(name); end
def msvc80_project_filename(name); end
def target(name,&block); end

def env( variable )
	

import("ruby") {
	# add_include_directories `echo RUBYINCLUDE`
	# add_library_directories $(RUBYLIB).split(';')
	# add_library ruby-1.8.5.lib
}

group("libindustry") {
	msvc80_solution_filename 'projects\msvc80_libindustry.sln'

	project("libindustry") {
		dependancy import("ruby")
		msvc80_project_filename 'projects\msvc80_libindustry.vcproj'

		sources('industry/*.((c|h)pp)') {
			warnings :maximum
			# supress_msvc80_warnings 1, 2, 3
		}
		sources('industry/*ruby*.((c|h)pp)') {
			warnings :none
			# supress_msvc80_warnings 1, 2, 3
		}

		target("Static Release") {
			basename  'release\libindustry'
			debug     false
			type      :staticlib
		}
		target("Static Debug") {
			basename  'debug\libindustry'
			debug     true
			type      :staticlib
		}
		target("Dynamic Release") {
			basename  'release\libindustry'
			debug     false
			type      :dynamiclib
		}
		target("Dynamic Debug") {
			basename  'debug\libindustry'
			debug     true
			type      :dynamiclib
		}
	}
	project("libindustry-unittest") {
		dependancy project("libindustry")
		msvc80_project_filename 'projects\msvc80_libindustry_unittest.vcproj'

		sources("unittest/*.((c|h)pp)") {
			warnings :maximum
			# supress_msvc80_warnings 1, 2, 3
		}
		sources("unittest/*ruby*.((c|h)pp)") {
			warnings :none
		}

		target("Release") {
			dependancy project("libindustry").target("Static Release")
			basename 'release\unittest'
			debug    false
			type     :program
		}
		target("Debug") {
			dependancy project("libindustry").target("Static Debug")
			basename 'debug\unittest'
			debug    true
			type     :program
		}
	}
}
