#if !defined(CACHEMEMORY_H_INCLUDED_4E3C8994_8BC8_413F_916C_466BB3678DBD)
    #define CACHEMEMORY_H_INCLUDED_4E3C8994_8BC8_413F_916C_466BB3678DBD

    #include <API/Api.h>
    #include <Memory/ICacheMemory.h>
    #include <Utility/UniqueRef.h>
    #include <functional>

BEGIN_NAMESPACE

class [[nodiscard]] CacheMemory final : public ICacheMemory {
  public:
    struct Config {
        CacheWriteStrategy m_cacheWriteStrategy;
        CacheMemoryMapping m_memoryMapping;
    };

    static constexpr const char* Default_name = "CacheMemory";

    CacheMemory(Config config, IMemory* upStreamMemory, Address cacheSize);
    CacheMemory(std::string name, Config config, IMemory* upStreamMemory, Address cacheSize);
    CacheMemory(CacheMemory&&) noexcept;
    CacheMemory& operator=(CacheMemory&&) noexcept;
    virtual ~CacheMemory();

    CacheMemory(const CacheMemory&)            = delete;
    CacheMemory& operator=(const CacheMemory&) = delete;

    [[nodiscard]] DataUnit Read(Address address) const noexcept final;

    /// <summary>
    /// Block has to be equal to one cache line
    /// TODO: Fix that
    /// </summary>
    /// <param name="start"></param>
    /// <param name="dataUnitCount"></param>
    /// <returns></returns>
    [[nodiscard]] DataBlock< DataUnit > ReadBlock(Address start, std::uint64_t dataUnitCount) const final;

    void Write(Address address, DataUnit data) final;

    /// <summary>
    /// Block has to be equal to one cache line
    /// TODO: Fix that
    /// </summary>
    /// <param name="start"></param>
    /// <param name="data"></param>
    void WriteBlock(Address start, const DataBlock< DataUnit >& data) final;

    [[nodiscard]] Address Size() const noexcept final;

    [[nodiscard]] const MemoryWatcher& GetMemoryWatcher() const noexcept final;
    [[nodiscard]] const IMemory* const GetUpStreamMemory() const noexcept final;
    void                               ClearCache() noexcept final;

    [[nodiscard]] static constexpr const std::pmr::vector< CacheWriteStrategy >& GetSupportedWriteStrategies() noexcept;
    [[nodiscard]] static constexpr const std::pmr::vector< CacheMemoryMapping >& GetSupportedMemoryMappings() noexcept;

  private:
    class Impl;
    UniqueRef< Impl > m_memory;

    template < class ImplDetail >
    [[nodiscard]] static UniqueRef< Impl > ConstructMemory(IMemory* upstreamMemory, Address size, const Config& config,
                                                           ImplDetail detail);

    // Forward declaration of cache memory types
    class DirectAccessWriteThroughCacheMemory;
    // class FullyAssociativeWriteThroughCacheMemory;
    // class SetAssociativeWriteThroughCacheMemory;
    // class DirectAccessWriteBackCacheMemory;
    // class FullyAssociativeWriteBackCacheMemory;
    // class SetAssociativeWriteBackCacheMemory;
};

END_NAMESPACE

#endif // !defined(CACHEMEMORY_H_INCLUDED_4E3C8994_8BC8_413F_916C_466BB3678DBD)
