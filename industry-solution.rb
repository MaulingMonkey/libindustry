import("ruby") {
	add_include_directories *env_list('RUBYINCLUDE')
	add_library_directories *env_list('RUBYLIB')
	add_library_basename    "ruby-#{VERSION}"
}

solution("libindustry") {
	msvc80_solution_filename 'projects\msvc80_libindustry.sln'
	gnu_makefile_filename    'projects\Makefile'

	project("libindustry") {
		dependancy import("ruby")
		sources('industry/**/*.{c,h}pp)') {}
	}
	project("unittest") {
		dependancy project("libindustry")
		sources('unittest/**/*.{c,h}pp') {}
	}
}
