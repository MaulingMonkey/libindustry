def target(name, &block)
	block.call()
end
def targets(*names, &block)
	names.each do |name|
		target(name,&block)
	end
end
def type(name); end
def dependancies(*list); end
def filename(name); end

target("libindustry") {
	type    :meta

	dependancies "libindustry-static", "libindustry-dynamic", "libindustry-unittest"
}

target("libindustry-static") {
	type    :staticlib
	filename case $OS
		when "Windows"; "libindustry.lib"
		when "Linux";   "libindustry.a"
		when "OS X";    "libindustry.a"
		else;           nil
	end
}

target("libindustry-dynamic") {
	type    :dynamiclib
	filename case $OS
		when "Windows"; "libindustry.dll"
		when "Linux";   "libindustry.so"
		when "OS X";    "libindustry.dylib"
		else;           nil
	end
}

targets("libindustry-static","libindustry-dynamic") {
	dependancies "ruby"
}
