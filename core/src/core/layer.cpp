#include <string>
#include <ostream>


#include "core/layer.h"

namespace CG {

    Layer::Layer(const std::string& name) : debugName_(name.c_str()) {}
} // namespace CG
