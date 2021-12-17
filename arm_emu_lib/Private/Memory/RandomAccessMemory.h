#if !defined(RANDOMACCESSMEMORY_H_INCLUDED_43BCD972_B193_41C4_9FC4_E61E6D156A59)
    #define RANDOMACCESSMEMORY_H_INCLUDED_43BCD972_B193_41C4_9FC4_E61E6D156A59

    #include <API/Api.h>
    #include <Memory/IMemory.h>
    #include <Utility/UniqueRef.h>
    #include <memory>

BEGIN_NAMESPACE

class [[nodiscard]] RandomAccessMemory final : public IMemory {
  private:
    static constexpr size_t      Default_memory_size = 1_MB;
    static constexpr const char* Default_name        = "RandomAccessMemory";

  public:
    RandomAccessMemory(std::string name = Default_name, Address addressableSize = Default_memory_size);

    RandomAccessMemory(RandomAccessMemory&&) noexcept;
    RandomAccessMemory& operator=(RandomAccessMemory&&) noexcept;
    virtual ~RandomAccessMemory();

    RandomAccessMemory(const RandomAccessMemory&) = delete;
    RandomAccessMemory& operator=(const RandomAccessMemory&) = delete;

    [[nodiscard]] DataUnit              Read(Address address) noexcept final;
    [[nodiscard]] DataBlock< DataUnit > ReadBlock(Address start, std::uint64_t dataUnitCount) final;

    void Write(Address address, DataUnit data) noexcept final;
    void WriteBlock(Address start, const DataBlock< DataUnit >& data) final;

    [[nodiscard]] Address Size() const noexcept final;

    [[nodiscard]] const MemoryWatcher& GetMemoryWatcher() const noexcept final;

  private:
    class Impl;
    UniqueRef< Impl > m_memory;

    template < class ImplDetail >
    [[nodiscard]] static UniqueRef< Impl > ConstructMemory(Address arg, ImplDetail detail);
};

END_NAMESPACE

#endif // !defined(RANDOMACCESSMEMORY_H_INCLUDED_43BCD972_B193_41C4_9FC4_E61E6D156A59)
