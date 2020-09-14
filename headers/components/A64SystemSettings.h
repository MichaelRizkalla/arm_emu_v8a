#pragma once

#include <A64Enums.h>
#include <utilities.hpp>

namespace arm_emu {

struct SystemSettings {

    template < typename TEnumType, TEnumType TValue >
    static constexpr bool IsImplemented();

    SystemSettings() = delete;
    ~SystemSettings() = delete;
    NULL_COPY_MOVE(SystemSettings)
};

template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_DIT >() {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::EL3 >() {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_FP16 >() {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_PAN >() {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_SSBS >() {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_MTE >() {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_UAO >() {
    return false;
}

template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_BTI >() {
    return false;
}

} // namespace arm_emu