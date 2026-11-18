#ifndef INC_2C_LABA1_EXECUTOR_H
#define INC_2C_LABA1_EXECUTOR_H
#include <string>
#include "storage.h"
#include "../structures/array.h"

std::string execute(Storage& storage, const Array& tokens, bool& changed);

#endif
