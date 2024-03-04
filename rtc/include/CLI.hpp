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
inline const std::string history = "history";
inline const std::string rename = "rename";

// Types
inline const std::string component = "component";
inline const std::string fileTemplate = "template";

// flags
inline const std::string css = "css";
inline const std::string typescript = "ts";
inline const std::string test = "test";

// templates
inline const std::string componentTemplate = "component.txt";
inline const std::string jsTestTemplate = "component-test-js.txt";
inline const std::string tsTestTemplate = "component-test-js.txt";
inline const std::string cssTemplate = "component-styles-css.txt";

}  // namespace Tokens

namespace Config {

inline const std::string templatesDir = std::string(ASSETS_DIR) + "/templates/";
inline const std::string customTemplatesDir = std::string(ASSETS_DIR) + "/templates/custom/";
inline const std::string historyDir = std::string(ASSETS_DIR) + "/history/";
inline const std::string templatePlaceholder = "%TEMPLATE_PLACEHOLDER%";
inline const std::string flagPrefix = "--";

}  // namespace Config

enum class Error {
    NOT_ENOUGH_ARGUMENTS,
    INVALID_ACTIVITY,
    INVALID_TYPE,
    INVALID_FILE_PATH,
    INVALID_TYPE_FOR_ACTIVITY,
    INSUFFICIENT_PERMISSIONS,
    SELECTED_FILE_IS_NOT_REACT_COMPONENT,
    SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE,
    INVALID_FLAG_SYNTAX,
    COULD_NOT_DELETE_CUSTOM_TEMPLATE,
    NEW_FILE_EXTENSION_DOES_NOT_MATCH_OLD_EXTENSION,
    UNKNOWN,
};

enum class Activity {
    GENERATE,
    ADD,
    REMOVE,
    LIST,
    HISTORY,
    RENAME,
};

enum class Type {
    COMPONENT,
    TEMPLATE,
};

class Executor;
void ErrorHandler(CLI::Error);

}  // namespace CLI

#endif
