#  $LastChangedDate$
#  $LastChangedBy$

require 'rexml/document'
require 'pathname'

class VCProj
	def initialize( filename, rootpath )
		@filename = Pathname.new(filename)
		@projpath = @filename.dirname
		@rootpath = Pathname.new(rootpath)
		File.open(filename) {|file| @document = REXML::Document.new(file)}
	end

	def give_vcproj_context
		unless @context_given
			puts "", "In #{@filename}..."
			@context_given = true
		end
	end
	
	def scan( *args )
		case args.size
		when 0; scan( nil, @rootpath, @document.elements["//Files"] )
		when 3; do_scan(*args)
		else  ; raise ArgumentError, "Expected 0 or 3 arguments, got #{args.size}"
		end
	end
	def fix( xml = @document.elements["//Files"], path = nil )
		@context_given = false if xml == @document.elements["//Files"] and path == nil
		
		xml.each_element( "Filter" ) do |filter|
			name    = filter.attributes["Name"]
			subpath = File.join(*[path,name].compact)
			fix( filter, subpath )
		end
		
		xml.each_element( "File" ) do |file|
			oldpath = file.attributes["RelativePath"]
			abspath = File.expand_path( oldpath, @projpath )
			next unless $files[abspath].moved?
			newpath = Pathname.new($files[abspath].moved_to).relative_path_from(Pathname.getwd+@projpath).to_s.gsub("/","\\")
			
			give_vcproj_context
			puts "    Tracked #{oldpath} -> #{newpath}"
			file.attributes["RelativePath"] = newpath
			@changes = true
		end
	end
	def save_xml( file, node, indent = "" )
		@@containers ||= [ 'VisualStudioProject', 'Platforms', 'ToolFiles', 'Configurations', 'Configuration', 'References', 'Files', 'Filter', 'File', 'Globals' ]
		
		if node.attributes.empty?
			if @@containers.include? node.name or !node.elements.empty?
				file.puts "#{indent}<#{node.name}>"
				
				node.elements.each {|e| save_xml(file,e,indent+"\t") }
				
				file.puts "#{indent}</#{node.name}>"
			else
				file.puts "#{indent}<#{node.name} />" # never used by MSVC's XML
			end
		else
			if @@containers.include? node.name
				file.puts "#{indent}<#{node.name}"
				node.attributes.each {|attr,val| file.puts "#{indent}\t#{attr}=\"#{val}\""}
				file.puts "#{indent}\t>"
				
				node.elements.each {|e| save_xml(file,e,indent+"\t") }
				
				file.puts "#{indent}</#{node.name}>"
			else
				file.puts "#{indent}<#{node.name}"
				node.attributes.each {|attr,val| file.puts "#{indent}\t#{attr}=\"#{val}\"" }
				file.puts "#{indent}/>"
			end
		end
	end
	def save
		return unless @changes
		
		give_vcproj_context
		File.open(@filename,"w") do |file|
			file.puts '<?xml version="1.0" encoding="WINDOWS-1251"?>'
			save_xml( file, @document.root )
		end
		puts "    Changes saved"
	end
private
	def check_create_filter_for(path,name)
		return true if $files[path].auto_add?
		
		give_vcproj_context
		print "  Create a filter for #{path}, #{name}? (y/n) "
		$>.flush
		
		gets =~ /^y(e(s)?)?$/i
	end
	def check_move_file_from_to( source, dest )
		return true if $files[File.expand_path( source, @projpath )].auto_move?
		
		give_vcproj_context
		print "    Move file from #{source} to #{dest}? (y/n) "
		$>.flush
		
		gets =~ /^y(e(s)?)?$/i
	end
	def do_scan( path, dir, xml )
		# path = nil for root
		# dir  = Dir instance
		# xml  = REXML::Element instance
		
		dirs, files = dir.children.partition { |f| f.directory? }
		xml.each_element( "Filter" ) do |filter|
			name = filter.attributes["Name"]
			
			subdir = dirs.find {|d| d.basename.to_s == name }
			if subdir
				scan( File.join(*[path,name].compact), subdir, filter )
				dirs.delete subdir
			else
				give_vcproj_context
				puts "    Warning:  No corresponding directory to #{name}"
			end
		end
		
		# No filters correspond to remaining dirs
		dirs.reject! {|d| $files[d].ignore? }
		dirs.each do |d|
			name = d.basename.to_s
			if (check_create_filter_for(d.to_s,name))
				new_node = xml.add_element( "Filter" )
				new_node.attributes["Name"] = name
				@changes = true
				puts "  Created filter #{name}"
			end
		end
		
		xml.each_element( "File" ) do |file|
			actual_path   = file.attributes["RelativePath"] # relative to @projpath
			name          = File.basename(actual_path)
			expected_path = (dir+name).relative_path_from(@projpath).to_s.gsub("/","\\")
			files.reject! {|f| f.basename.to_s == name }
			
			abs_actual_path    = File.expand_path(actual_path  , @projpath)
			abs_expected_path  = File.expand_path(expected_path, @projpath)
			
			next if $files[actual_path].moved?                                 # already moved
			next if actual_path == expected_path                               # already in a good path
			next unless check_move_file_from_to( actual_path, expected_path )  # already acceptable to user
				
			$files[abs_actual_path].move_to( abs_expected_path )
		end
		
		files.reject! {|f| $files[f].ignore? }
		files.each do |f|
			give_vcproj_context
			puts "    Warning:  No corresponding file entry to #{f}"
		end
	end
end

def vcproj( filename, args )
	$vcprojs ||= []
	$vcprojs << VCProj.new( filename, args[:root] )
end

def scan_vcprojs; $vcprojs.each {|proj| proj.scan }; end
def  fix_vcprojs; $vcprojs.each {|proj| proj.fix; proj.save }; end
