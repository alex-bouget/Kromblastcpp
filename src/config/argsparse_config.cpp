#include "argsparse_config.hpp"
#include "json_config.hpp"
#include <argparse/argparse.hpp>
#include <string>
#include <vector>

#include "utils.hpp"
#include "config_constructor.hpp"
#include "kromblast_compiler_utils.hpp"

void argparser(argparse::ArgumentParser *parser)
{
    parser->add_argument("-t", "--title")
        .help("Title of the project")
        .required();
    parser->add_argument("-sw", "--width")
        .scan<'i', int>()
        .help("Width of the window")
        .required();
    parser->add_argument("-sh", "--height")
        .scan<'i', int>()
        .help("Height of the window")
        .required();
    parser->add_argument("-f", "--fullscreen")
        .help("Fullscreen mode")
        .default_value(false);
    parser->add_argument("-fl", "--frameless")
        .help("Frameless mode")
        .default_value(false);
    parser->add_argument("-d", "--debug")
        .help("Debug mode")
        .implicit_value(true)
        .default_value(false);
    parser->add_argument("-r", "--registry")
        .default_value<std::vector<std::string>>({})
        .append()
        .help("List of the approved registry");
    parser->add_argument("-m", "--mode")
        .help("Mode of the host")
        .required();
    parser->add_argument("-hs", "--host")
        .help("url of the server or path")
        .required();
    parser->add_argument("-lp", "--lib-path")
        .help("Folder with the libraries");
    parser->add_argument("-l", "--lib")
        .default_value<std::vector<std::string>>({})
        .append()
        .help("List of the libraries");
    parser->add_argument("-lc", "--lib-config")
        .default_value<std::vector<std::string>>({})
        .append()
        .help("Config of all libraries: libname:key=value");
}

std::map<std::string, ::Kromblast::Class::kromlib_config_t, std::less<>> get_configs(const std::vector<std::string> &configs)
{
    std::map<std::string, ::Kromblast::Class::kromlib_config_t, std::less<>> plugins_config;
    for (const auto &config : configs)
    {
        std::string libname = config.substr(0, config.find(":"));
        std::string key = config.substr(config.find(":") + 1, config.find("=") - config.find(":") - 1);
        std::string value = config.substr(config.find("=") + 1);
        if (plugins_config.contains(libname))
        {
            plugins_config[libname][key] = value;
        }
        else
        {
            plugins_config[libname] = {{key, value}};
        }
    }
    return plugins_config;
}

Kromblast::Core::ConfigKromblast Kromblast::Config::argsparse(int argc, const char *argv[])
{
    argparse::ArgumentParser program("kromblast", __KROMBLAST_VERSION__);
    argparse::ArgumentParser json("json", __KROMBLAST_VERSION__);
    argparse::ArgumentParser conf("conf", __KROMBLAST_VERSION__);
    argparser(&conf);
    json.add_argument("json_file")
        .help("Path to the json file")
        .required();

    program.add_subparser(json);
    program.add_subparser(conf);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << std::endl;
        if (program.is_subcommand_used("json"))
        {
            std::cerr << json;
        }
        else if (program.is_subcommand_used("conf"))
        {
            std::cerr << conf;
        }
        else
        {
            std::cerr << program;
        }
        std::exit(1);
    }
    if (program.is_subcommand_used("json"))
    {
        std::string json_file = json.get<std::string>("json_file");
        return get_from_json(json_file);
    }
    if (program.is_subcommand_used("conf"))
    {
        Kromblast::Core::ConfigKromblast config;
        config.window.title = conf.get<std::string>("title");
        config.window.width = conf.get<int>("width");
        config.window.height = conf.get<int>("height");
        config.window.fullscreen = conf.get<bool>("fullscreen");
        config.window.frameless = conf.get<bool>("frameless");
        config.debug = conf.get<bool>("debug");
        std::vector<std::string> lib(conf.get<std::vector<std::string>>("lib"));
        decode_plugins_folder(&lib, conf.get<std::string>("lib-path"));
        config.plugins = Kromblast::Config::create_config_plugins(
            lib,
            get_configs(conf.get<std::vector<std::string>>("lib-config")));
        config.approved_registry = conf.get<std::vector<std::string>>("registry");
        config.mode = get_mode(conf.get<std::string>("mode"));
        config.host = conf.get<std::string>("host");
        return config;
    }
    std::cerr << "No subcommand used" << std::endl;
    std::cerr << program << std::endl;
    std::exit(1);
}