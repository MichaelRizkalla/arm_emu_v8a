#pragma once

#include <A64Enums.h>
#include <utilities.hpp>

namespace arm_emu {

struct SystemSettings {

    template < typename TEnumType, TEnumType TValue >
    static constexpr bool IsImplemented() noexcept;

    SystemSettings()  = delete;
    ~SystemSettings() = delete;
    NULL_COPY_MOVE(SystemSettings)
};

template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_DIT >() noexcept {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::EL3 >() noexcept {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_FP16 >() noexcept {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_PAN >() noexcept {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_SSBS >() noexcept {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_MTE >() noexcept {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_UAO >() noexcept {
    return false;
}
template <>
constexpr bool SystemSettings::IsImplemented< Features, Features::FEAT_BTI >() noexcept {
    return false;
}

} // namespace arm_emu