#include "file_util.hpp"

std::string GetFileName(const std::string &path)
{
    return path.empty() ? "" : path.substr(path.find_last_of('\\') + 1, path.size());
}
