def dependancy(name_or_project); end
def import(name,&block); end
def projectgroup(name,&block); end
def project(name,&block); end
def msvc80_solution_filename(name); end
def msvc80_project_filename(name); end
def target(name,&block); end

import("ruby") {
	# add_include_directories ...
	# add_library_directories
	# add_library ruby-1.8.5.lib
}

projectgroup("libindustry") {
	msvc80_solution_filename 'projects\msvc80_libindustry.sln'

	project("libindustry") {
		dependancy import("ruby")
		msvc80_project_filename 'projects\msvc80_libindustry.vcproj'

		mask "*.((c|h)pp)"
		directories "industry"

		target("Static Release") {
		}
		target("Static Debug") {
		}
	}
	project("libindustry-unittest") {
		dependancy project("libindustry")
		msvc80_project_filename 'projects\msvc80_libindustry_unittest.vcproj'

		mask "*.((c|h)pp)"
		directories "unittest"
	}
}
