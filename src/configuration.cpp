#include "configuration.h"


/*
use internal file to populate config structure, strings

on native machine, if there is no external config file, spit out the internal copy

(on web, put one in browser fake filesystem?)

if we've not just created it, read in external configuration

do same for localization strings

*/

const char *config_loader = R"config_loader(



)config_loader";


const char *default_settings = R"default_settings(



)default_settings";


const char *internationalization = R"i18n(



)i18n"; // internationalization

const char *user_settings = R"user_settings(



)user_settings";

void read_in_global_configuration() {
	cout  << "yo\n";

//	SHOW_TYPE(default_settings)
}
