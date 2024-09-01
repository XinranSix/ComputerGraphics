#include <ostream>
#include <string>

#include "core/app/layer.h"

namespace CG {

    Layer::Layer(const std::string& name) : debugName_(name.c_str()) {}
} // namespace CG
