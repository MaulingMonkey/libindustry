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
}

script( :ibs , :ruby ) {
	sources "ibs-lib/**/*.rb"
	start   "ibs-lib/ibs.rb"
}
