#pragma once

#include <string>

namespace CLI {

namespace Tokens {

// Activities
inline const std::string generate = "generate";
inline const std::string add = "add";
inline const std::string remove = "remove";
inline const std::string list = "list";
inline const std::string rename = "rename";
inline const std::string print = "print";
inline const std::string clear = "clear";
inline const std::string help = "help";

// Types
inline const std::string component = "component";
inline const std::string fileTemplate = "template";
inline const std::string history = "history";

// Flags
inline const std::string css = "css";
inline const std::string typescript = "ts";
inline const std::string test = "test";
inline const std::string styles = "styles";

// rtc provided templates
inline const std::string componentTemplate = "component.txt";
inline const std::string jsTestTemplate = "component-test-js.txt";
inline const std::string tsTestTemplate = "component-test-js.txt";
inline const std::string cssTemplate = "component-styles-css.txt";

}  // namespace Tokens

namespace Config {

inline const std::string assetsDir = std::string(ASSETS_DIR);
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
    SELECTED_FILE_IS_NOT_SUITABLE_AS_TEMPLATE,
    SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE,
    INVALID_FLAG_SYNTAX,
    COULD_NOT_DELETE_CUSTOM_TEMPLATE,
    NEW_FILE_EXTENSION_DOES_NOT_MATCH_OLD_EXTENSION,
    TOO_MANY_STYLES_FILES_REQUESTED,
    UNKNOWN,
};

enum class Activity {
    GENERATE,
    ADD,
    REMOVE,
    LIST,
    PRINT,
    CLEAR,
    RENAME,
    HELP,
};

enum class Type {
    COMPONENT,
    TEMPLATE,
    HISTORY,
    STYLES,
};

class Executor;
void ErrorHandler(CLI::Error);

}  // namespace CLI
