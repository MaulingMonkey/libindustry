#  Copyright (c) 2007 Michael B. Edwin Rickert
# 
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt )
# 
#  $LastChangedBy$ - $LastChangedDate$

$: << 'ibs-lib'
$toolchains = []
require 'dsl'

Dir[ "ibs-lib/concepts/*.rb" ].each do |concept|
	load concept
end

def do_scan_and_export()
	tokenized_root = $project_root.split('/')-['.']
	raise "Error:  $project_root contains filesystem dependant elements (..)" if tokenized_root.member? '..'
	raise "Error:  $project_root contains environment dependant elements"     if tokenized_root.find {|e| e =~ /\$\(.*?\)/}
	$inverse_project_root = tokenized_root.collect{|discarded| '..'}.join('\\')
	$inverse_project_root = '.' if $inverse_project_root.empty? # TODO:  Test effects of
	
	File.makedirs( $project_root ) unless File.exists? $project_root
	raise "$project_root is not a directory (#{$project_root})" unless File.directory? $project_root
	begin
		print "    Writing to: #{$project_root}..."
		list = $programs.values + $libraries.values + $scripts.values
		$toolchain.scan    list
		$toolchain.export  list
		puts "OK."
	rescue StandardError => e
		puts "ERROR."
		raise
	end
end

Dir[ "ibs-lib/toolchains/*.rb" ].each do |toolchain|
	sets = [ :programs , :libraries , :dependancies , :scripts ]
	sets.each{|set| eval( "$#{set} = {}" )}
	
	puts "Exporting with: #{File.basename(toolchain)}..."
	load toolchain
	
	load 'ibs-project.rb'
	do_scan_and_export() unless File.exists? 'ibs-project-local.rb' #  Update SVN version
	
	if File.exists? 'ibs-project-local.rb'
		base_sets = {}
		sets.each do |set|
			base_sets = base_sets.merge({ set => eval("$#{set}")})
			eval( "$#{set} = {}" )
		end
		
		load 'ibs-project-local.rb' if File.exists? 'ibs-project-local.rb'
		sets.each {|set| eval( "$#{set} = base_sets[:#{set}].merge $#{set}" )}
		do_scan_and_export()  #  Update locally customized version
	end
end
