$project_root = './projects/'

dependancy( :ruby ) {
	add_include_paths ENV['RUBYINCLUDE']
	add_library_paths ENV['RUBYLIB']
	add_library       "msvcr80-ruby#{VERSION.split('.')[0..1].join}.lib"
}

library( :libindustry ) {
	dependancy :ruby
	sources "industry/**/*.{c,h}pp"
}

program( :unittest ) {
	dependancy :libindustry
	sources "unittest/**/*.{c,h}pp"
	msvc_warning_level  4
	msvc_supress_warnings  4512, 4127, 4189, 4701, 4180, 4224
}

script( :ibs , :ruby ) {
	sources "ibs-lib/**/*.rb"
	start   "ibs-lib/ibs.rb"
}
