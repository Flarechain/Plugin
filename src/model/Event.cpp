#include "Event.h"

void Event::clear()
{
    ip_address = std::nullopt;
    osc_message = std::nullopt;
}
