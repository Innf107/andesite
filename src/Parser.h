#include <string>
#include <vector>

class Parser {
public:
    static std::vector<std::string> splitCommand(const std::string& cmd);

private:
    static bool isSpace(char c);
};
