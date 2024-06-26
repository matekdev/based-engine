#pragma once

#include <string>
#include <vector>

#include "panel.hpp"

#include <glog/logging.h>

class ConsolePanel : public google::LogSink, public Panel
{
public:
    ConsolePanel();

    void Draw() override;
    void send(google::LogSeverity severity, const char *full_filename,
              const char *base_filename, int line,
              const struct ::tm *tm_time,
              const char *message, size_t message_len) override;

private:
    const int MAX_LOGS = 500;
    const std::string INFO = "[INFO] ";
    const std::string WARNING = "[WARNING] ";
    const std::string ERR = "[ERROR] ";

    std::string _input;
    std::vector<std::string> _previousLogs;
    bool _scrollToBottom;

    void ExecuteCommand(const std::string &command);
    std::string SeverityToString(google::LogSeverity severity) const;
};
