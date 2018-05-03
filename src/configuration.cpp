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

const char *config_loader = R"config_loader(

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

register_number("Version", 555)




)config_loader";

 



#include <unordered_map>
using std::unordered_map;
using std::string;

const unordered_map<string, gNumberNames> strings_to_enumerated_numbers = {
	{"Version", version},
	{"Second Thing", second_thing}
};

const unordered_map<string, gStringNames> strings_to_enumerated_strings = {
	{"Workspace menu - Notes", workspace_menu_notes}
};

const unordered_map<string, gColorNames> strings_to_enumerated_colors = {
};

const unordered_map<string, gBoolNames> strings_to_enumerated_bools = {
};





const char *default_settings = R"default_settings(
)default_settings";

const char *internationalization = R"i18n(
)i18n"; // internationalization

const char *user_settings = R"user_settings(
)user_settings";

extern "C" {
	void receive_number_from_js(char* setting_name, double value) {
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
	void receive_color_from_js(char* setting_name, double r, double g, double b, double a) {
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
	void receive_string_from_js(char* setting_name, char* value) {
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
	void receive_boolean_from_js(char* setting_name, int value) {
	}

}

void read_in_global_configuration() {
SHOW_TYPE(version);

	gNums[version] = 3;

	
	cout  << "yo\n"<<gNums[version]<<'\n';


    std::cout << "Hello, World!\n";
/*
create javascript function to pass strings to c++, call it from lua
*/
//	std::string ssss = 'print("hello!!)";

	#ifdef __EMSCRIPTEN__

		EM_ASM_({
			// console.log(fengari.load('return 2+2')());
			console.log(fengari.load(Pointer_stringify($0))());
			//window.foo();
			//console.log("thingfo ", thingfo);

		}, config_loader);
	#endif
}
