
[[nodiscard]] bool HavePACExt() const noexcept {
    return HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3);
}