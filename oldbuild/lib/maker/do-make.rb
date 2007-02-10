#!/opt/local/bin/ruby
require 'maker/project'

project = Maker::Project.new( "." )
if ARGV.include? "clean" then
	project.clean()
else
	project.build()
end
