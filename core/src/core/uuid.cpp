#include <random>

#include "core/uuid.h"

namespace CG {

    static std::random_device sRandomDevice;
    static std::mt19937_64 sEng { sRandomDevice() };
    static std::uniform_int_distribution<uint64_t> sUniformDistribution;

    static std::mt19937 sEng32 { sRandomDevice() };
    static std::uniform_int_distribution<uint32_t> sUniformDistribution32;

    UUID::UUID() : uuid_ { sUniformDistribution(sEng) } {}

    UUID::UUID(uint64_t uuid) : uuid_ { uuid } {}

    UUID::UUID(const UUID& other) : uuid_ { other.uuid_ } {}

    UUID32::UUID32() : uuid_ { sUniformDistribution32(sEng32) } {}

    UUID32::UUID32(uint32_t uuid) : uuid_ { uuid } {}

    UUID32::UUID32(const UUID32& other) : uuid_ { other.uuid_ } {}

} // namespace CG
