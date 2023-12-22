#ifndef CLI_HPP
#define CLI_HPP

#include <string>

namespace CLI {

namespace Tokens {
// Activities
inline const std::string generate = "generate";
inline const std::string add = "add";
inline const std::string remove = "remove";
inline const std::string list = "list";

// Types
inline const std::string component = "component";
inline const std::string fileTemplate = "template";

// flags
inline const std::string css = "css";
}  // namespace Tokens

namespace Config {
inline const std::string assetsDir = "/usr/local/share/react-cli/assets/templates/";
inline const std::string customAssetsDir = "/usr/local/share/react-cli/assets/templates/custom/";
inline const std::string flagPrefix = "--";
}  // namespace Config

enum class Error {
    NOT_ENOUGH_ARGUMENTS,
    INVALID_ACTIVITY,
    INVALID_TYPE,
    INVALID_FILE_PATH,
    INVALID_TYPE_FOR_ACTIVITY,
    INSUFFICIENT_PERMISSIONS,
    SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE,
    NO_CUSTOM_TEMPLATES_FOUND,
    INVALID_FLAG_SYNTAX,
    UNKNOWN,
};

enum class Activity {
    GENERATE,
    ADD,
    REMOVE,
    LIST,
};

enum class Type {
    COMPONENT,
    TEMPLATE,
};

class Executor;
class ErrorHandler;

}  // namespace CLI

#endif
