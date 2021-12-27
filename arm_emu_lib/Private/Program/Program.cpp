
#include <API/Api.h>
#include <API/HiddenAPI.h>
#include <Allocation/MemoryResourceProbe.h>
#include <Program/Program.h>

#include <Memory/ProgramMemory.h>

BEGIN_NAMESPACE

ARMEMU_API Program GetTestProgram(size_t idx) {
    IMemory::Address size {};
    switch (idx) {
        case 0:
            size = 68_B;
            break;
        case 1:
            size = 116_B;
            break;
        case 2:
            size = 92_B;
            break;
    }
    auto memory =
        allocate_unique< IMemory, ProgramMemory >(std::pmr::polymorphic_allocator< ProgramMemory > {}, "Program", size);

    auto progMemory = reinterpret_cast< ProgramMemory* >(memory.get());

    /*
        Program 0:
        int main(){
            int x = 5;

            if (x == 7) {
                return 0;
            }
            else if (x == 5) {
                return 5;
            }
            return 0;
        }

        Program 1:
        int main(){
            int* x = new int[2];

            x[0] = 0;
            x[1] = 1;

            if (x[0] == 1) {
                return 0;
            }
            else if (x[1] == 1) {
                return x[1];
            }

            return 0;
        }

        Program 2:
        int main(){
            int* x = new int;

            x[0] = 0;

            if (x[0] == 1) {
                return 0;
            }
            else if (x[0] == 0) {
                return 5;
            }

            return 0;
        }

    */

    switch (idx) {
        case 0:
            progMemory->WriteBlock(0, { 0xd10043ff, 0x528000a0, 0xb9000fe0, 0xb9400fe0, 0x71001c1f, 0x54000061,
                                        0x52800000, 0x14000007, 0xb9400fe0, 0x7100141f, 0x54000061, 0x528000a0,
                                        0x14000002, 0x52800000, 0x910043ff, 0xd65f03c0, 0xd503201f });
            break;
        case 1:
            progMemory->WriteBlock(0, { 0xa9be7bfd, 0x910003fd, 0xd2800100, 0x97ffffa6, 0xf9000fe0, 0xf9400fe0,
                                        0xb900001f, 0xf9400fe0, 0x91001000, 0x52800021, 0xb9000001, 0xf9400fe0,
                                        0xb9400000, 0x7100041f, 0x54000061, 0x52800000, 0x1400000a, 0xf9400fe0,
                                        0x91001000, 0xb9400000, 0x7100041f, 0x54000081, 0xf9400fe0, 0xb9400400,
                                        0x14000002, 0x52800000, 0xa8c27bfd, 0xd65f03c0, 0xd503201f });
            break;
        case 2:
            progMemory->WriteBlock(0, { 0xa9be7bfd, 0x910003fd, 0xd2800080, 0x97ffffae, 0xf9000fe0, 0xf9400fe0,
                                        0xb900001f, 0xf9400fe0, 0xb9400000, 0x7100041f, 0x54000061, 0x52800000,
                                        0x14000008, 0xf9400fe0, 0xb9400000, 0x7100001f, 0x54000061, 0x528000a0,
                                        0x14000002, 0x52800000, 0xa8c27bfd, 0xd65f03c0, 0xd503201f });
            break;
    }

    return Program { std::move(memory), size };
}

END_NAMESPACE
