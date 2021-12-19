
[[nodiscard]] bool HasArchVersion(IProcessingUnit::ExtensionVersion extensionVersion) const noexcept {
    return extensionVersion <= extensionVersion;
}

[[nodiscard]] bool HaveVirtHostExt() noexcept {
    return HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p1);
}

[[nodiscard]] bool HaveSecureEL2Ext() noexcept {
    return HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p4);
}

[[nodiscard]] bool HaveBTIExt() noexcept {
    return HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p5);
}

[[nodiscard]] bool HaveFlagManipulateExt() const noexcept {
    return HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p4);
}