#if !defined(MEMORYMANAGEMENTUNIT_H_INCLUDED_72AFBDB1_D6B2_4B50_A827_4264669DC9DA)
    #define MEMORYMANAGEMENTUNIT_H_INCLUDED_72AFBDB1_D6B2_4B50_A827_4264669DC9DA

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <Memory/IMemory.h>
    #include <exception>
    #include <unordered_map>

BEGIN_NAMESPACE

class invalid_physical_memory_access : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override {
        return "Invalid physical memory access";
    }
};

class untracked_processing_unit : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override {
        return "Untracked processing unit";
    }
};

class processing_unit_already_tracked : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override {
        return "Processing unit is already tracked";
    }
};

class [[nodiscard]] MemoryManagementUnit final : public Object {
    static constexpr const char* Default_name = "MemoryManagementUnit";

    struct PhysicalAddressRange {
        IMemory::Address m_start;
        IMemory::Address m_end;
    };

  public:
    MemoryManagementUnit(std::string name = Default_name);
    DELETE_COPY_CLASS(MemoryManagementUnit)
    DEFAULT_MOVE_CLASS(MemoryManagementUnit)
    DEFAULT_DTOR(MemoryManagementUnit)

    /// @brief Add a process to be tracked by MMU
    [[nodiscard]] void             AddProcess(void* processAddress, IMemory::Address startAddressSpace,
                                              IMemory::Address endAddressSpace);

    /// @brief Translates virtual address to the physical address in storage   
    [[nodiscard]] IMemory::Address Translate(void* processAddress, IMemory::Address virtualAddress) const;

    /// @brief Returns number of mappings
    [[nodiscard]] std::size_t      Count() const noexcept;

  private:
    std::pmr::unordered_map< void*, PhysicalAddressRange > m_mappings;
};

END_NAMESPACE

#endif // !defined(MEMORYMANAGEMENTUNIT_H_INCLUDED_72AFBDB1_D6B2_4B50_A827_4264669DC9DA)
