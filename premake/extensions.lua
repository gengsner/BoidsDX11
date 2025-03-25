require('vstudio')

premake.api.register {
    name = "sdlchecks",
    scope = "config",
    kind = "boolean",
    default = true
}

premake.override(premake.vstudio.vc2010, "multiProcessorCompilation", function(base, prj)
	premake.w('<SDLCheck>' .. tostring(true) .. '</SDLCheck>')
	base(prj)
end)