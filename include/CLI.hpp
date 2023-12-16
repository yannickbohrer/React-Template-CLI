#ifndef CLI_HPP
#define CLI_HPP

#include <string>

namespace CLI {

namespace Tokens {
inline const std::string generate = "generate";

inline const std::string component = "component";
}  // namespace Tokens

namespace Config {
inline const std::string assetsDir = "/usr/local/share/react-cli/assets/templates";
}  // namespace Config

enum class Error {
    NOT_ENOUGH_ARGUMENTS,
    INVALID_ACTIVITY,
    INVALID_TYPE,
    UNKNOWN,
};

enum class Activity {
    GENERATE,
};

enum class Type {
    COMPONENT,
};

class Executor;
class ErrorHandler;

}  // namespace CLI

#endif
