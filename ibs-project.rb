$project_root = './projects/'

dependancy( :ruby ) {
	add_include_paths ENV['RUBYINCLUDE']
	add_library_paths ENV['RUBYLIB']
	add_library       "ruby-#{VERSION}.lib"
}

library( :libindustry ) {
	dependancy :ruby
	sources "industry/**/*.{c,h}pp"
}

program( :unittest ) {
	dependancy library(:libindustry)
	sources "unittest/**/*.{c,h}pp"
}
