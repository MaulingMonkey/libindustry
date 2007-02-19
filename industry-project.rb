import("ruby") {
	add_include_directories env_list(RUBYINCLUDE)
	add_library_directories env_list(RUBYLIB)
	add_library_basename    "ruby-#{VERSION}"
}

group("libindustry") {
	msvc80_solution_filename 'projects\msvc80_libindustry.sln'
	gnu_makefile_filename    'projects\Makefile'

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
	project("unittest") {
		dependancy project("libindustry")
		msvc80_project_filename 'projects\msvc80_unittest.vcproj'

		sources("unittest/*.((c|h)pp)") {
			warnings :maximum
			# supress_msvc80_warnings 1, 2, 3
		}
		sources("unittest/*ruby*.((c|h)pp)") {
			warnings :none
			# supress_msvc80_warnings 1, 2, 3
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
