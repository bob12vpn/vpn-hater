#include "filter.h"

bool Filter::openRawSocket(char *interface) {
    return sendSocket.open(interface);
}