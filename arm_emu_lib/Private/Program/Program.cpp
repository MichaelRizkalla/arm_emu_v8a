
#include <API/Api.h>
#include <API/HiddenAPI.h>
#include <Allocation/MemoryResourceProbe.h>
#include <Program/Program.h>

#include <Memory/RandomAccessMemory.h>

BEGIN_NAMESPACE

ARMEMU_API std::size_t GetLostMem() noexcept {
    auto& probe = MemoryResourceProbe::GetInstance();
    return probe.GetCurrentAllocationCount();
}

 ARMEMU_API UniqueRef< IMemory > GetProgram() {
    auto memory =
         allocate_unique< IMemory, RandomAccessMemory >(std::pmr::polymorphic_allocator< RandomAccessMemory > {}, "Program", 1_MB);

    auto ramMemory = reinterpret_cast< RandomAccessMemory* >(memory.get());
    
    ramMemory->Write(0, 0xd10043ff);
    ramMemory->Write(1, 0x528000a0);
    ramMemory->Write(2, 0xb9000fe0);
    ramMemory->Write(3, 0xb9400fe0);
    ramMemory->Write(4, 0x71000c1f);
    ramMemory->Write(5, 0x54000061);
    ramMemory->Write(6, 0x52800060);
    ramMemory->Write(7, 0x14000007);
    ramMemory->Write(8, 0xb9400fe0);
    ramMemory->Write(9, 0x7100141f);
    ramMemory->Write(10, 0x54000061);
    ramMemory->Write(11, 0x528000a0);
    ramMemory->Write(12, 0x14000002);
    ramMemory->Write(13, 0x52800000);
    ramMemory->Write(14, 0x910043ff);
    ramMemory->Write(15, 0xd65f03c0);

    return memory;
 }

END_NAMESPACE
