#include <vector>
#include <string>
#include "kromblast_lib_config.hpp"

void decode_plugins_folder(std::vector<std::string>* lib_name, const std::string &plugin_folder);

Kromblast::Core::Mode get_mode(const std::string &json_mode);