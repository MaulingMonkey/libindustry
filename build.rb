def target(hash)
end

target {
	:name => "libindustry",
	:type => "metatarget",
	:dependancies =>
		[ "libindustry-static"
		, "libindustry-dynamic"
		, "libindustry-unittest"
		]
	}

target {
	:name => "libindustry-static",
	:type => :staticlib,
	:filename => case $OS
		when "Windows"; "libindustry.lib"
		when "Linux";   "libindustry.a"
		when "OS X";    "libindustry.a"
		else;           nil
	end
	}

target {
	:name => "libindustry-dynamic",
	:type => :dynamiclib,
	:filename => case $OS
		when "Windows"; "libindustry.dll"
		when "Linux";   "libindustry.so"
		when "OS X";    "libindustry.dylib"
		else;           nil
	end
	}