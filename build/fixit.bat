@echo off
ruby -x "%~f0"
goto EOF

#!/usr/bin/ruby
#
#  fixit.bat -- project file monkey patcher
#
#  $LastChangedDate$
#  $LastChangedBy$


Dir.chdir(File.dirname($0))
Dir["fixlib/**"].each {|lib| require lib}

auto_move "win/**/*.[ch]pp"                       # automatically move files created within the IDE to their appropriate directories
auto_add  "../{industry,unit-tests}/**/*/"        # automatically add new directories
auto_add  "../{industry,unit-tests}/**/*.[ch]pp"  # automatically add new source files
ignore    "../*"                                  # assume all desired top level files and directories are already added
ignore    "../**/.svn"                            # ignore SVN directories

vcproj    "win/lib-industry-vs2008.vcproj", :root => ".."
vcproj    "win/unit-tests-vs2008.vcproj"  , :root => "../unit-tests"
vcproj    "win/unit-tests-vs2005.vcproj"  , :root => "../unit-tests"

scan_vcprojs
fix_filepaths
fix_vcprojs

__END__
:EOF
