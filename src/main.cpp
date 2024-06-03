#include "application.hpp"

int main(int argc, char *argv[])
{
    const auto application = Application(1400, 900, "Based Engine");
    application.Run();
}
