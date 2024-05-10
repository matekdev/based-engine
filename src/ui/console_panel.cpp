#include "console_panel.hpp"

#include <glog/logging.h>
#include <iostream>
#include <chrono>
#include <ctime>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ConsolePanel::ConsolePanel()
{
    google::AddLogSink(this);
}

void ConsolePanel::Render()
{
    ImGui::Begin("Console");

    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    for (const auto &log : _previousLogs)
    {
        auto color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        if (log.contains(INFO))
            color = ImVec4(0.78f, 0.78f, 0.78f, 1.0f);
        if (log.contains(WARNING))
            color = ImVec4(0.99f, 0.73f, 0.01f, 1.0f);
        else if (log.contains(ERROR))
            color = ImVec4(0.99f, 0.01f, 0.01f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(log.c_str());
        ImGui::PopStyleColor();
    }

    if (_scrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    _scrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    ImGui::PushItemWidth(-1);
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("##label", &_input, input_text_flags))
        ExecuteCommand(_input);

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

void ConsolePanel::ExecuteCommand(const std::string &command)
{
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const struct tm *tmPtr = std::localtime(&currentTime);

    send(-1, nullptr, nullptr, -1, tmPtr, command.c_str(), command.size());

    if (_input.contains("clear"))
        _previousLogs.clear();

    _input = {};
    _scrollToBottom = true;
}

void ConsolePanel::send(google::LogSeverity severity, const char *full_filename,
                        const char *base_filename, int line,
                        const struct ::tm *tm_time,
                        const char *message, size_t message_len)
{
    std::string log(message, message_len);
    std::string severityString = SeversityToString(severity);

    char formattedTime[12];
    std::strftime(formattedTime, sizeof(formattedTime), "%H:%M:%S", tm_time);

    std::stringstream out;
    out << std::string(formattedTime) << " ";
    out << severityString;
    out << log;

    if (_previousLogs.size() >= MAX_LOGS)
        _previousLogs.erase(_previousLogs.begin());

    _previousLogs.push_back(out.str());
}

std::string ConsolePanel::SeversityToString(google::LogSeverity severity)
{
    switch (severity)
    {
    case google::GLOG_INFO:
        return INFO;
    case google::GLOG_WARNING:
        return WARNING;
    case google::GLOG_ERROR:
        return ERROR;
    default:
        return {};
    }
}
