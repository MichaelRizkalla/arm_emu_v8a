
#include <API/Api.h>
#include <API/HiddenAPI.h>
#include <Allocation/MemoryResourceProbe.h>
#include <Program/Program.h>

#include <Memory/ProgramMemory.h>

BEGIN_NAMESPACE

ARMEMU_API Program GetTestProgram() {
    auto memory =
        allocate_unique< IMemory, ProgramMemory >(std::pmr::polymorphic_allocator< ProgramMemory > {}, "Program", 64_B);

    auto progMemory = reinterpret_cast< ProgramMemory* >(memory.get());

    progMemory->WriteBlock(0, { 0xd10043ff, 0x528000a0, 0xb9000fe0, 0xb9400fe0, 0x71000c1f, 0x54000061, 0x52800060,
                               0x14000007, 0xb9400fe0, 0x7100141f, 0x54000061, 0x528000a0, 0x14000002, 0x52800000,
                               0x910043ff, 0xd65f03c0 });

    return Program { std::move(memory), 16 };
}

END_NAMESPACE
