/*
 * Copyright © 2018 AperLambda <aper.entertainment@gmail.com>
 *
 * This file is part of λcommon.
 *
 * Licensed under the MIT license. For more information,
 * see the LICENSE file.
 */

#ifndef LAMBDACOMMON_CLI_ARGUMENTS_H
#define LAMBDACOMMON_CLI_ARGUMENTS_H

// WIP

/*#include "lambdacommon.h"
#include <map>
#include <vector>

namespace lambdacommon
{
    class Argument
    {
    private:
        pstring _name;
        char _shortcut;
        std::vector<std::string> *_aliases;
        pstring _description;
        pstring _usage;

    public:
        Argument(std::string name, std::string description, char shortcut = 0, std::vector<std::string> aliases = {}, std::string usage = "");

        Argument(const Argument &arg);

        Argument(Argument &&arg) noexcept;

        ~Argument();

        std::string getName() const;

        char getShortcut() const;

        std::vector<std::string> getAliases() const;

        std::string getDescription() const;

        std::string getUsage() const;
    };

    class ArgumentManager
    {
    private:
        std::map<std::string, Argument> *_arguments;

        std::map<std::string, Argument> getArguments() const;

    public:
        ArgumentManager();

        ArgumentManager(const ArgumentManager &argManager);

        ArgumentManager(ArgumentManager &&argManager) noexcept;

        ~ArgumentManager();

        bool addArgument(std::string name, std::string description, char shortcut = 0, std::vector<std::string> aliases = {}, std::string usage = "");

        bool addArgument(std::string name, std::string description, char shortcut = 0, std::string aliases = "", std::string usage = "");

        bool removeArgument(std::string name);

        void parse(int argc, char** argv);

        std::map<std::string, std::string> parse(std::vector<std::string> args);
    };
}
*/
#endif //LAMBDACOMMON_CLI_ARGUMENTS_H