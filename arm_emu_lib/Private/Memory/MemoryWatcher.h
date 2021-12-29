#if !defined(MEMORYWATCHER_H_INCLUDED_ED19DE87_72CE_4C3C_9085_32F4320E8570)
    #define MEMORYWATCHER_H_INCLUDED_ED19DE87_72CE_4C3C_9085_32F4320E8570

    #include <API/Api.h>
    #include <cstddef>
    #include <cstdint>

BEGIN_NAMESPACE

enum class MemoryAccessResult
{
    Hit,
    Miss,
};

enum class MemoryAccessType
{
    Read,
    ReadBlock,
    Write,
    WriteBlock,

    UpStreamRead,
    UpStreamReadBlock,
    UpStreamWrite,
    UpStreamWriteBlock,
};

class [[nodiscard]] MemoryWatcher {

    struct MemoryAccessTypeCounter {
        std::size_t m_readCount { 0 };
        std::size_t m_readBlockCount { 0 };
        std::size_t m_writeCount { 0 };
        std::size_t m_writeBlockCount { 0 };
    };

  public:
    /// <summary>
    /// Will guide the MemoryWatcher when counting the total access count
    /// </summary>
    enum class Hint
    {
        CacheMemory,
        RandomAccessMemory,
    };

    MemoryWatcher(Hint hint = Hint::RandomAccessMemory);

    MemoryWatcher(MemoryWatcher&&) noexcept;
    MemoryWatcher& operator=(MemoryWatcher&&) noexcept;
    ~MemoryWatcher();

    MemoryWatcher(const MemoryWatcher&) = delete;
    MemoryWatcher& operator=(const MemoryWatcher&) = delete;

    void RecordAccessType(MemoryAccessType type);
    void RecordAccessResult(MemoryAccessResult result);

    [[nodiscard]] std::size_t GetHitCount() const noexcept;
    [[nodiscard]] std::size_t GetMissCount() const noexcept;
    [[nodiscard]] double      GetMissRatio() const noexcept;
    [[nodiscard]] double      GetHitRatio() const noexcept;

    [[nodiscard]] std::size_t GetReadCount() const noexcept;
    [[nodiscard]] std::size_t GetReadBlockCount() const noexcept;
    [[nodiscard]] std::size_t GetAllReadCount() const noexcept;
    [[nodiscard]] std::size_t GetWriteCount() const noexcept;
    [[nodiscard]] std::size_t GetWriteBlockCount() const noexcept;
    [[nodiscard]] std::size_t GetAllWriteCount() const noexcept;

    [[nodiscard]] std::size_t GetUpStreamReadCount() const noexcept;
    [[nodiscard]] std::size_t GetUpStreamReadBlockCount() const noexcept;
    [[nodiscard]] std::size_t GetAllUpStreamReadCount() const noexcept;
    [[nodiscard]] std::size_t GetUpStreamWriteCount() const noexcept;
    [[nodiscard]] std::size_t GetUpStreamWriteBlockCount() const noexcept;
    [[nodiscard]] std::size_t GetAllUpStreamWriteCount() const noexcept;

    [[nodiscard]] std::size_t GetMemoryAccessCount() const noexcept;

  private:
    std::size_t m_hits;
    std::size_t m_misses;

    MemoryAccessTypeCounter m_memory;
    MemoryAccessTypeCounter m_upStreamMemory;
    Hint                    m_hint;
};

END_NAMESPACE

#endif // !defined(MEMORYWATCHER_H_INCLUDED_ED19DE87_72CE_4C3C_9085_32F4320E8570)
