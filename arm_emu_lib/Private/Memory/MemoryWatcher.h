#if !defined(MEMORYWATCHER_H_INCLUDED_ED19DE87_72CE_4C3C_9085_32F4320E8570)
    #define MEMORYWATCHER_H_INCLUDED_ED19DE87_72CE_4C3C_9085_32F4320E8570

    #include <API/Api.h>
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
        size_t m_readCount { 0 };
        size_t m_readBlockCount { 0 };
        size_t m_writeCount { 0 };
        size_t m_writeBlockCount { 0 };
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

    [[nodiscard]] size_t GetHitCount() const noexcept;
    [[nodiscard]] size_t GetMissCount() const noexcept;
    [[nodiscard]] double GetMissRatio() const noexcept;
    [[nodiscard]] double GetHitRatio() const noexcept;

    [[nodiscard]] size_t GetReadCount() const noexcept;
    [[nodiscard]] size_t GetReadBlockCount() const noexcept;
    [[nodiscard]] size_t GetAllReadCount() const noexcept;
    [[nodiscard]] size_t GetWriteCount() const noexcept;
    [[nodiscard]] size_t GetWriteBlockCount() const noexcept;
    [[nodiscard]] size_t GetAllWriteCount() const noexcept;

    [[nodiscard]] size_t GetUpStreamReadCount() const noexcept;
    [[nodiscard]] size_t GetUpStreamReadBlockCount() const noexcept;
    [[nodiscard]] size_t GetAllUpStreamReadCount() const noexcept;
    [[nodiscard]] size_t GetUpStreamWriteCount() const noexcept;
    [[nodiscard]] size_t GetUpStreamWriteBlockCount() const noexcept;
    [[nodiscard]] size_t GetAllUpStreamWriteCount() const noexcept;

    [[nodiscard]] size_t GetMemoryAccessCount() const noexcept;

  private:
    size_t m_hits;
    size_t m_misses;

    MemoryAccessTypeCounter m_memory;
    MemoryAccessTypeCounter m_upStreamMemory;
    Hint                    m_hint;
};

END_NAMESPACE

#endif // !defined(MEMORYWATCHER_H_INCLUDED_ED19DE87_72CE_4C3C_9085_32F4320E8570)
