#include "configuration.h"


#include <iostream>
using std::cout;
using std::endl;




/*
use internal file to populate config structure, strings

on native machine, if there is no external config file, spit out the internal copy

(on web, put one in browser fake filesystem?)

if we've not just created it, read in external configuration

do same for localization strings

*/

 


#include <unordered_map>
using std::unordered_map;
using std::string;

const unordered_map<string, gNumberNames> strings_to_enumerated_numbers = {
	{"Version", version_number},
	{"Second Thing", second_thing}
};

const unordered_map<string, gStringNames> strings_to_enumerated_strings = {
	{"Workspace menu - Notes", workspace_menu_notes}
};

const unordered_map<string, gColorNames> strings_to_enumerated_colors = {
};

const unordered_map<string, gBoolNames> strings_to_enumerated_bools = {
};





const char *default_settings_string = R"default_settings(
default_settings = {
	["Version"] =  0.1

-- RGB component values go from 0-255
-- HSV components: Hue goes from 0-360 like a color wheel, Saturation and Value go from 0.0 to 1.0
-- Grays range from 0.0 (black) to 1.0 (white)
-- Alpha values always go from 0 for transparent to 255 for opaque

-- The functions available for setting colors are: rgb, rgba, hsv, hsva, gray and grayAlpha

}
)default_settings";

const char *internationalization = R"i18n(

)i18n"; // internationalization

const char *user_settings = R"user_settings(
)user_settings";

extern "C" {
	void receive_number(const char* setting_name, double value) {
		auto setting = strings_to_enumerated_numbers.find(setting_name);
		if (setting == strings_to_enumerated_numbers.end()) {
			string complaint = "Attempt to set an unknown setting \"";
			complaint.append(setting_name);
			complaint.append("\" to value ");
			complaint.append(std::to_string(value));
			complain(complaint);
		}
		else {
			::gNums[setting->second] = value;
		}
	}
	void receive_color(char* setting_name, double r, double g, double b, double a) {
		auto setting = strings_to_enumerated_colors.find(setting_name);
		if (setting == strings_to_enumerated_colors.end()) {
			string complaint = "Attempt to set an unknown setting \"";
			complaint.append(setting_name);
			complaint.append("\" to value [");
			complaint.append(std::to_string(r)); complaint.append(", ");
			complaint.append(std::to_string(g)); complaint.append(", ");
			complaint.append(std::to_string(b)); complaint.append(", ");
			complaint.append(std::to_string(a)); complaint.append("]");
			complain(complaint);
		}
		else {
//			::gStrings[setting->second] = value;
		}
	}
	void receive_string(char* setting_name, char* value) {
		auto setting = strings_to_enumerated_numbers.find(setting_name);
		if (setting == strings_to_enumerated_numbers.end()) {
			string complaint = "Attempt to set an unknown setting \"";
			complaint.append(setting_name);
			complaint.append("\" to value \"");
			complaint.append(value);
			complaint.append("\"");
			complain(complaint);
		}
		else {
			::gStrings[setting->second] = value;
		}
	}
	void receive_boolean(char* setting_name, int value) {
	}

}

const char *config_loader_shared_code = R"shared_loader(
	function register_settings()
		for k,v in pairs(default_settings) do
			if (type(v) == "table") then
--				if     (v[1] == "rgba") then register_RGBA_color(k,v[2],v[3],v[4],v[5])
--				elseif (v[1] == "hsva") then register_HSVA_color(k,v[2],v[3],v[4],v[5])
--				end
			elseif (type(v) == "number")  then register_number(k,v)
--			elseif (type(v) == "string")  then register_string(k,v)
--			elseif (type(v) == "boolean") then register_boolean(k,v)
			end
		end
	end
print("there")

)shared_loader";



void read_in_user_settings_web_version() {
	#ifdef __EMSCRIPTEN__
		const char *config_loader_for_web = R"web_loader(
			js = require "js"

			js.global.foo = function() print("WEEE") end

			function register_number(name, val)
				js.global.juxt_setting_name = name
				js.global.juxt_numbers = val
				js.global.pass_number_to_cpp() 
			end

			function register_string(name, val)
				js.global.juxt_setting_name = name
				js.global.juxt_strings = val
				js.global.pass_string_to_cpp() 
			end

			function register_color(name, r, g, b, a)
				js.global.juxt_setting_name = name
				js.global.juxt_color_r = r
				js.global.juxt_color_g = g
				js.global.juxt_color_b = b
				js.global.juxt_color_a = a
				js.global.pass_color_to_cpp() 
			end

			function register_boolean(name, val)
				js.global.juxt_setting_name = name
				js.global.juxt_booleans = val
				js.global.pass_string_to_cpp() 
			end
		)web_loader";

		EM_ASM_({
			// console.log(fengari.load('return 2+2')());
			fengari.load(Pointer_stringify($0))();
			fengari.load(Pointer_stringify($1))();
			fengari.load(Pointer_stringify($2))();
			fengari.load("register_settings()")();
		}, config_loader_for_web, config_loader_shared_code, default_settings_string);

	#endif
}

#ifndef __EMSCRIPTEN__
	void read_in_user_settings_native_version(sel::State& luaInterpreter) {
		luaInterpreter["register_number"] = [&](std::string setting_name, double value) {
			receive_number(setting_name.c_str(), value);
		};

		luaInterpreter(config_loader_shared_code);
		luaInterpreter(default_settings_string);
		luaInterpreter("register_settings()");
	}
#endif