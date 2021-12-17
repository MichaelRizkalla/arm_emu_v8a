#if !defined(ICACHEMEMORY_H_INCLUDED_01C12737_24CB_4F60_8D82_CFDB4514182F)
    #define ICACHEMEMORY_H_INCLUDED_01C12737_24CB_4F60_8D82_CFDB4514182F

    #include <API/Api.h>
    #include <Memory/IMemory.h>
    #include <array>
    #include <vector>

BEGIN_NAMESPACE

enum class CacheWriteStrategy
{
    WriteThrough,
    WriteBack,
};

enum class CacheMemoryMapping
{
    DirectMapping,
    FullyAssociative,
    SetAssociative,
};

/// <summary>
/// Will implement Write-through for now
/// </summary>
class [[nodiscard]] ICacheMemory : public IMemory {
  public:
    static constexpr Address Max_cache_size  = 32_MB;
    static constexpr Address Cache_line_size = 64_B;

    ICacheMemory(std::string name = "ICacheMemory") : IMemory(std::move(name)) {
        // assert(Size() <= Max_cache_size);
    }
    ICacheMemory(ICacheMemory&&) = default;
    ICacheMemory& operator=(ICacheMemory&&) = default;
    virtual ~ICacheMemory()                 = default;

    ICacheMemory(const ICacheMemory&) = delete;
    ICacheMemory& operator=(const ICacheMemory&) = delete;

    [[nodiscard]] virtual const IMemory* const GetUpStreamMemory() const noexcept = 0;
    virtual void                               ClearCache() noexcept              = 0;
};

END_NAMESPACE

#endif // !defined(ICACHEMEMORY_H_INCLUDED_01C12737_24CB_4F60_8D82_CFDB4514182F)