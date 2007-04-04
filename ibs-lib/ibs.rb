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

Dir[ "ibs-lib/toolchains/*.rb" ].each do |toolchain|
	$programs     = {}
	$libraries    = {}
	$dependancies = {}
	$scripts      = {}
	
	print "Exporting with: #{File.basename(toolchain)}..."
	load toolchain
	load 'ibs-project.rb'
	
	tokenized_root = $project_root.split('/')-['.']
	raise "Error:  $project_root contains filesystem dependant elements (..)" if tokenized_root.member? '..'
	raise "Error:  $project_root contains environment dependant elements"     if tokenized_root.find {|e| e =~ /\$\(.*?\)/}
	$inverse_project_root = tokenized_root.collect{|discarded| '..'}.join('/')
	$inverse_project_root = '.' if $inverse_project_root.empty? # TODO:  Test effects of
	
	File.makedirs( $project_root ) unless File.exists? $project_root
	raise "$project_root is not a directory (#{$project_root})" unless File.directory? $project_root
	
	begin
		$toolchain.export( $programs.values + $libraries.values + $scripts.values )
		puts "OK."
	rescue StandardError => e
		puts "ERROR."
		raise
	end
end
