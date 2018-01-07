/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

/*
#include "../include/lambdacommon/cli_arguments.h"
#include "../include/lambdacommon/string.h"

using namespace std;

namespace lambdacommon
{
    Argument::Argument(string name, string description, char shortcut, vector<string> aliases, string usage) :
            _name(new string(name)), _description(new string(description)), _aliases(new vector<string>(aliases)),
            _usage(new string(usage))
    {
        _shortcut = shortcut;
    }

    Argument::Argument(const Argument &arg) : _name(new string(arg.getName())),
                                              _description(new string(arg.getDescription())),
                                              _aliases(new vector<string>(arg.getAliases())),
                                              _usage(new string(arg.getUsage()))
    {
        _shortcut = arg._shortcut;
    }

    Argument::Argument(Argument &&arg) : _name(new string(move(arg.getName()))),
                                         _description(new string(move(arg.getDescription()))),
                                         _aliases(new vector<string>(move(arg.getAliases()))),
                                         _usage(new string(move(arg.getUsage())))
    {
        _shortcut = move(arg._shortcut);
    }

    Argument::~Argument()
    {
        delete _name;
        delete _description;
        delete _aliases;
        delete _usage;
    }

    string Argument::getName() const
    {
        return *_name;
    }

    char Argument::getShortcut() const
    {
        return _shortcut;
    }

    vector<string> Argument::getAliases() const
    {
        return *_aliases;
    }

    string Argument::getDescription() const
    {
        return *_description;
    }

    string Argument::getUsage() const
    {
        return *_usage;
    }

    /*
     * Argument manager
     *

    ArgumentManager::ArgumentManager() : _arguments(new map<string, Argument>({}))
    {}

    ArgumentManager::ArgumentManager(const ArgumentManager &argManager) : _arguments(
            new map<string, Argument>(*argManager._arguments))
    {}

    ArgumentManager::ArgumentManager(ArgumentManager &&argManager) : _arguments(
            new map<string, Argument>(move(*argManager._arguments)))
    {}

    ArgumentManager::~ArgumentManager()
    {
        delete _arguments;
    }

    map<string, Argument> ArgumentManager::getArguments() const
    {
        return *_arguments;
    }

    bool
    ArgumentManager::addArgument(string name, string description, char shortcut, vector<string> aliases, string usage)
    {
        if (getArguments().count(name) > 1)
            return false;

        getArguments().emplace(name, Argument(name, description, shortcut, aliases, usage));
        return true;
    }

    bool
    ArgumentManager::addArgument(std::string name, std::string description, char shortcut, string aliases, string usage)
    {
        return addArgument(name, description, shortcut, lambdastring::split(aliases, ','), usage);
    }

    bool ArgumentManager::removeArgument(std::string name)
    {
        if (getArguments().count(name) == 0)
            return false;
        getArguments().erase(name);
        return true;
    }

    void ArgumentManager::parse(int argc, char **argv)
    {
        vector<string> line{static_cast<size_t>(argc)};
        if (argc > 0)
            for (size_t i = 0; i < argc; i++)
                line[i] = argv[i];
        parse(line);
    }

    map<string, string> ArgumentManager::parse(std::vector<std::string> args)
    {
        if (args.size() == 0)
            return {};

        map<string, string> result{};

        for (size_t i = 0; i < args.size(); i++)
        {
            if (args.)
        }
    }
}*/