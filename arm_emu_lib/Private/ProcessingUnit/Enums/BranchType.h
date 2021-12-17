#if !defined(BRANCHTYPE_H_INCLUDED_B8CBCADC_DB5F_4DC0_8F95_DDA5E8FCE677)
    #define BRANCHTYPE_H_INCLUDED_B8CBCADC_DB5F_4DC0_8F95_DDA5E8FCE677

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::BranchType : std::uint32_t
{
    DirCall,    // Direct Branch with link
    IndCall,    // Indirect Branch with link
    ERet,       // Exception return (indirect)
    DbgExit,    // Exit from Debug state
    Ret,        // Indirect branch with function return hint
    Dir,        // Direct branch
    InDir,      // Indirect branch
    Eexception, // Exception entry
    Reset,      // Reset
    Unknown     // Other
};

END_NAMESPACE

#endif // !defined(BRANCHTYPE_H_INCLUDED_B8CBCADC_DB5F_4DC0_8F95_DDA5E8FCE677)
