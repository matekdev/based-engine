#include "application.hpp"

#include <glog/logging.h>

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);

    const auto application = Application(1400, 900, "Based Engine");
    application.Run();
}
