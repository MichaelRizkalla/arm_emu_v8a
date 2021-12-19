
#include <ProcessingUnit/A64InstructionSet/A64InstructionSet.h>
#include <Utility/StreamableEnum.h>
#include <ostream>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup, std::string_view,
                                    enum_size_v< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup > >
        A64DataProcessingScalarFloatingPointAdvancedSIMDGroupStringMap { { {
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_AES, "CRYPTOGRAPHIC_AES"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOHRAPHIC_THREE_REGISTER_SHA,
              "CRYPTOHRAPHIC_THREE_REGISTER_SHA"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_TWO_REGISTER_SHA,
              "CRYPTOGRAPHIC_TWO_REGISTER_SHA"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_COPY,
              "ADVANCED_SIMD_SCALAR_COPY"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMG_SCALAR_THREE_SAME_FP16,
              "ADVANCED_SIMG_SCALAR_THREE_SAME_FP16"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                  ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS_FP16,
              "ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS_FP16"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_THREE_SAME_EXTRACTION,
              "ADVANCED_SIMD_SCALAR_THREE_SAME_EXTRACTION"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS,
              "ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_PAIRWISE,
              "ADVANCED_SIMD_SCALAR_PAIRWISE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_THREE_DIFFERENT,
              "ADVANCED_SIMD_SCALAR_THREE_DIFFERENT"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_THREE_SAME,
              "ADVANCED_SIMD_SCALAR_THREE_SAME"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_SHIFT_BY_IMMEDIATE,
              "ADVANCED_SIMD_SCALAR_SHIFT_BY_IMMEDIATE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_X_INDEXED_ELEMENT,
              "ADVANCED_SIMD_SCALAR_X_INDEXED_ELEMENT"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_TABLE_LOOKUP,
              "ADVANCED_SIMD_TABLE_LOOKUP"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_PERMUTE, "ADVANCED_SIMD_PERMUTE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_EXTRACT, "ADVANCED_SIMD_EXTRACT"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_COPY, "ADVANCED_SIMD_COPY"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_SAME_FP16,
              "ADVANCED_SIMD_THREE_SAME_FP16"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS_FP16,
              "ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS_FP16"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_REGISTER_EXTENSION,
              "ADVANCED_SIMD_THREE_REGISTER_EXTENSION"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS,
              "ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_ACROSS_LANES,
              "ADVANCED_SIMD_ACROSS_LANES"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_DIFFERENT,
              "ADVANCED_SIMD_THREE_DIFFERENT"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_SAME,
              "ADVANCED_SIMD_THREE_SAME"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_MODIFIED_IMMEDIATE,
              "ADVANCED_SIMD_MODIFIED_IMMEDIATE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SHIFT_BY_IMMEDIATE,
              "ADVANCED_SIMD_SHIFT_BY_IMMEDIATE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_VECTOR_X_INDEXED_ELEMENT,
              "ADVANCED_SIMD_VECTOR_X_INDEXED_ELEMENT"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_THREE_REGISTER_IMM2,
              "CRYPTOGRAPHIC_THREE_REGISTER_IMM2"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_THREE_REGISTER_SHA512,
              "CRYPTOGRAPHIC_THREE_REGISTER_SHA512"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_FOUR_REGISTER,
              "CRYPTOGRAPHIC_FOUR_REGISTER"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::XAR, "XAR"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_TWO_REGISTER_SHA512,
              "CRYPTOGRAPHIC_TWO_REGISTER_SHA512"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CONVERSION_FLOATING_POINT_AND_FIXED_POINT,
              "CONVERSION_FLOATING_POINT_AND_FIXED_POINT"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CONVERSION_FLOATING_POINT_AND_INTEGER,
              "CONVERSION_FLOATING_POINT_AND_INTEGER"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_DATA_PROCESSING_ONE_SOURCE,
              "FLOATING_POINT_DATA_PROCESSING_ONE_SOURCE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_COMPARE,
              "FLOATING_POINT_COMPARE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_IMMEDIATE,
              "FLOATING_POINT_IMMEDIATE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_CONDITIONAL_COMPARE,
              "FLOATING_POINT_CONDITIONAL_COMPARE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_DATA_PROCESSING_TWO_SOURCE,
              "FLOATING_POINT_DATA_PROCESSING_TWO_SOURCE"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_CONDITIONAL_SELECT,
              "FLOATING_POINT_CONDITIONAL_SELECT"sv },
            { A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_DATA_PROCESSING_THREE_SOURCE,
              "FLOATING_POINT_DATA_PROCESSING_THREE_SOURCE"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES > >
        CryptographicAESStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES::AESE, "AESE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES::AESD, "AESD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES::AESMC, "AESMC"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES::AESIMC, "AESIMC"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA > >
        CryptographicThreeRegisterSHAStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA::SHA1C, "SHA1C"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA::SHA1P, "SHA1P"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA::SHA1M, "SHA1M"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA::SHA1SU0, "SHA1SU0"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA::SHA256H, "SHA256H"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA::SHA256H2,
              "SHA256H2"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA::SHA256SU1,
              "SHA256SU1"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA > >
        CryptographicTwoRegisterSHAStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA::SHA1H, "SHA1H"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA::SHA1SU1, "SHA1SU1"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA::SHA256SU0,
              "SHA256SU0"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarCopy, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarCopy > >
        AdvancedSIMDScalarCopyStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarCopy::DUP_ELEMENT,
              "DUP_ELEMENT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16 > >
        AdvancedSIMDScalarThreeSameFP16StringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FMULX, "FMULX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FCMEQr, "FCMEQr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FRECPS, "FRECPS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FRSQRTS,
              "FRSQRTS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FCMGEr, "FCMGEr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FACGE, "FACGE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FABD, "FABD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FCMGTr, "FCMGTr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16::FACGT, "FACGT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16,
        std::string_view,
        enum_size_v<
            DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16 > >
        AdvancedSIMDScalarTwoRegisterMiscellaneousFP16StringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTNS_VECTOR,
              "FCVTNS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTMS_VECTOR,
              "FCVTMS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTAS_VECTOR,
              "FCVTAS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  SCVTF_VECTOR_INTEGER,
              "SCVTF_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCMGT_ZERO,
              "FCMGT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCMEQ_ZERO,
              "FCMEQ_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCMLT_ZERO,
              "FCMLT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTPS_VECTOR,
              "FCVTPS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTZS_VECTOR_INTEGER,
              "FCVTZS_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FRECPE,
              "FRECPE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FRECPX,
              "FRECPX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTNU_VECTOR,
              "FCVTNU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTMU_VECTOR,
              "FCVTMU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTAU_VECTOR,
              "FCVTAU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  UCVTF_VECTOR_INTEGER,
              "UCVTF_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCMGE_ZERO,
              "FCMGE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCMLE_ZERO,
              "FCMLE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTPU_VECTOR,
              "FCVTPU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FCVTZU_VECTOR_INTEGER,
              "FCVTZU_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::
                  FRSQRTE,
              "FRSQRTE"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameExtraction, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameExtraction > >
        AdvancedSIMDScalarThreeSameExtractionStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameExtraction::
                  SQRDMLAH_VECTOR,
              "SQRDMLAH_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameExtraction::
                  SQRDMLSH_VECTOR,
              "SQRDMLSH_VECTOR"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous,
        std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous > >
        AdvancedSIMDScalarTwoRegisterMiscellaneousStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::SUQADD,
              "SUQADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::SQABS,
              "SQABS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::CMGT_ZERO,
              "CMGT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::CMEQ_ZERO,
              "CMEQ_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::CMLT_ZERO,
              "CMLT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::ABS,
              "ABS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::SQXTN,
              "SQXTN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTNS_VECTOR,
              "FCVTNS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTMS_VECTOR,
              "FCVTMS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTAS_VECTOR,
              "FCVTAS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  SCVTF_VECTOR_INTEGER,
              "SCVTF_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCMGT_ZERO,
              "FCMGT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCMEQ_ZERO,
              "FCMEQ_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCMLT_ZERO,
              "FCMLT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTPS_VECTOR,
              "FCVTPS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTZS_VECTOR_INTEGER,
              "FCVTZS_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::FRECPE,
              "FRECPE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::FRECPX,
              "FRECPX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::USQADD,
              "USQADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::SQNEG,
              "SQNEG"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::CMGE_ZERO,
              "CMGE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::CMLE_ZERO,
              "CMLE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  NEG_VECTOR,
              "NEG_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::SQXTUN,
              "SQXTUN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::UQXTN,
              "UQXTN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::FCVTXN,
              "FCVTXN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTNU_VECTOR,
              "FCVTNU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTMU_VECTOR,
              "FCVTMU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTAU_VECTOR,
              "FCVTAU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  UCVTF_VECTOR_INTEGER,
              "UCVTF_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCMGE_ZERO,
              "FCMGE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCMLE_ZERO,
              "FCMLE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTPU_VECTOR,
              "FCVTPU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::
                  FCVTZU_VECTOR_INTEGER,
              "FCVTZU_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous::FRSQRTE,
              "FRSQRTE"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise > >
        AdvancedSIMDScalarPairwiseStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::ADDP_SCALAR,
              "ADDP_SCALAR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FMAXNMP_SCALAR_HALF_PRECISION,
              "FMAXNMP_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FADDP_SCALAR_HALF_PRECISION,
              "FADDP_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FMAXP_SCALAR_HALF_PRECISION,
              "FMAXP_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FMINNMP_SCALAR_HALF_PRECISION,
              "FMINNMP_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FMINP_SCALAR_HALF_PRECISION,
              "FMINP_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FMAXNMP_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FMAXNMP_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FADDP_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FADDP_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FMAXP_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FMAXP_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FMINNMP_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FMINNMP_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise::
                  FMINP_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FMINP_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeDifferent, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeDifferent > >
        AdvancedSIMDScalarThreeDifferentStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeDifferent::SQDMLAL,
              "SQDMLAL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeDifferent::SQDMLSL,
              "SQDMLSL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeDifferent::SQDMULL,
              "SQDMULL"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame > >
        AdvancedSIMDScalarThreeSameStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SQADD, "SQADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SQSUB, "SQSUB"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::CMGTr, "CMGTr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::CMGEr, "CMGEr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SSHL, "SSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SQSHLr, "SQSHLr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SRSHL, "SRSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SQRSHL, "SQRSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::ADD_VECTOR,
              "ADD_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::CMTST, "CMTST"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SQDMULH_VECTOR,
              "SQDMULH_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FMULX, "FMULX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FCMEQr, "FCMEQr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FRECPS, "FRECPS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FRSQRTS, "FRSQRTS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::UQADD, "UQADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::UQSUB, "UQSUB"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::CMHIr, "CMHIr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::CMHSr, "CMHSr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::USHL, "USHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::UQSHLr, "UQSHLr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::URSHL, "URSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::UQRSHL, "UQRSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SUB_VECTOR,
              "SUB_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::CMEQr, "CMEQr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::SQRDMULH_VECTOR,
              "SQRDMULH_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FCMGEr, "FCMGEr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FACGE, "FACGE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FABD, "FABD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FCMGTr, "FCMGTr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame::FACGT, "FACGT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate > >
        AdvancedSIMDScalarShiftByImmediateStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SSHR, "SSHR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SSRA, "SSRA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SRSHR,
              "SRSHR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SRSRA,
              "SRSRA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SHL, "SHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SQSHLi,
              "SQSHLi"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SQSHRN,
              "SQSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SQRSHRN,
              "SQRSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::
                  SCVTF_VECTOR_FIXED_POINT,
              "SCVTF_VECTOR_FIXED_POINT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::
                  FCVTZS_VECTOR_FIXED_POINT,
              "FCVTZS_VECTOR_FIXED_POINT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::USHR, "USHR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::USRA, "USRA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::URSHR,
              "URSHR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::URSRA,
              "URSRA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SRI, "SRI"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SLI, "SLI"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SQSHLU,
              "SQSHLU"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::UQSHLi,
              "UQSHLi"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SQSHRUN,
              "SQSHRUN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::SQRSHRUN,
              "SQRSHRUN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::UQSHRN,
              "UQSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::UQRSHRN,
              "UQRSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::
                  UCVTF_VECTOR_FIXED_POINT,
              "UCVTF_VECTOR_FIXED_POINT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate::
                  FCVTZU_VECTOR_FIXED_POINT,
              "FCVTZU_VECTOR_FIXED_POINT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement > >
        AdvancedSIMDScalarXIndexedElementStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::SQDMLAL_ELEMENT,
              "SQDMLAL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::SQDMLSL_ELEMENT,
              "SQDMLSL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::SQDMULL_ELEMENT,
              "SQDMULL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::SQDMULH_ELEMENT,
              "SQDMULH_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::SQRDMULH_ELEMENT,
              "SQRDMULH_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::
                  FMLA_ELEMENT_SCALAR_HALF_PRECISION,
              "FMLA_ELEMENT_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::
                  FMLS_ELEMENT_SCALAR_HALF_PRECISION,
              "FMLS_ELEMENT_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::
                  FMUL_ELEMENT_SCALAR_HALF_PRECISION,
              "FMUL_ELEMENT_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::
                  FMLA_ELEMENT_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FMLA_ELEMENT_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::
                  FMLS_ELEMENT_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FMLS_ELEMENT_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::
                  FMUL_ELEMENT_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FMUL_ELEMENT_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::SQRDMLAH_ELEMENT,
              "SQRDMLAH_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::SQRDMLSH_ELEMENT,
              "SQRDMLSH_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::
                  FMULX_ELEMENT_SCALAR_HALF_PRECISION,
              "FMULX_ELEMENT_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement::
                  FMULX_ELEMENT_SCALAR_SINGLE_DOUBLE_PRECISION,
              "FMULX_ELEMENT_SCALAR_SINGLE_DOUBLE_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup > >
        AdvancedSIMDTableLookupStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup::TBL_SINGLE_REGISTER,
              "TBL_SINGLE_REGISTER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup::TBX_SINGLE_REGISTER,
              "TBX_SINGLE_REGISTER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup::TBL_TWO_REGISTER,
              "TBL_TWO_REGISTER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup::TBX_TWO_REGISTER,
              "TBX_TWO_REGISTER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup::TBL_THREE_REGISTER,
              "TBL_THREE_REGISTER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup::TBX_THREE_REGISTER,
              "TBX_THREE_REGISTER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup::TBL_FOUR_REGISTER,
              "TBL_FOUR_REGISTER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup::TBX_FOUR_REGISTER,
              "TBX_FOUR_REGISTER"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute > >
        AdvancedSIMDPermuteStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute::UZP1, "UZP1"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute::TRN1, "TRN1"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute::ZIP1, "ZIP1"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute::UZP2, "UZP2"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute::TRN2, "TRN2"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute::ZIP2, "ZIP2"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDExtract, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDExtract > >
        AdvancedSIMDExtractStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDExtract::EXT, "EXT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy > >
        AdvancedSIMDCopyStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy::DUP_ELEMENT, "DUP_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy::DUP_GENERAL, "DUP_GENERAL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy::SMOV, "SMOV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy::UMOV, "UMOV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy::INS_GENERAL, "INS_GENERAL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy::INS_ELEMENT, "INS_ELEMENT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16 > >
        AdvancedSIMDThreeSameFP16StringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMAXNM_VECTOR,
              "FMAXNM_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMLA_VECTOR,
              "FMLA_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FADD_VECTOR,
              "FADD_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMULX, "FMULX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FCMEQr, "FCMEQr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMAX_VECTOR,
              "FMAX_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FRECPS, "FRECPS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMINNM_VECTOR,
              "FMINNM_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMLS_VECTOR,
              "FMLS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FSUB_VECTOR,
              "FSUB_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMIN_VECTOR,
              "FMIN_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FRSQRTS, "FRSQRTS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMAXNMP_VECTOR,
              "FMAXNMP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FADDP_VECTOR,
              "FADDP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMUL_VECTOR,
              "FMUL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FCMGEr, "FCMGEr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FACGE, "FACGE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMAXP_VECTOR,
              "FMAXP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FDIV_VECTOR,
              "FDIV_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMINNMP_VECTOR,
              "FMINNMP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FABD, "FABD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FCMGTr, "FCMGTr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FACGT, "FACGT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16::FMINP_VECTOR,
              "FMINP_VECTOR"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16 > >
        AdvancedSIMDTwoRegisterMiscellaneousFP16StringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FRINTN_VECTOR,
              "FRINTN_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FRINTM_VECTOR,
              "FRINTM_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTNS_VECTOR,
              "FCVTNS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTMS_VECTOR,
              "FCVTMS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTAS_VECTOR,
              "FCVTAS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  SCVTF_VECTOR_INTEGER,
              "SCVTF_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FCMGT_ZERO,
              "FCMGT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FCMEQ_ZERO,
              "FCMEQ_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FCMLT_ZERO,
              "FCMLT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FABS_VECTOR,
              "FABS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FRINTP_VECTOR,
              "FRINTP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FRINTZ_VECTOR,
              "FRINTZ_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTPS_VECTOR,
              "FCVTPS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTZS_VECTOR_INTEGER,
              "FCVTZS_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FRECPE,
              "FRECPE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FRINTA_VECTOR,
              "FRINTA_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FRINTX_VECTOR,
              "FRINTX_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTNU_VECTOR,
              "FCVTNU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTMU_VECTOR,
              "FCVTMU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTAU_VECTOR,
              "FCVTAU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  UCVTF_VECTOR_INTEGER,
              "UCVTF_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FCMGE_ZERO,
              "FCMGE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FCMLE_ZERO,
              "FCMLE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FNEG_VECTOR,
              "FNEG_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FRINTI_VECTOR,
              "FRINTI_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTPU_VECTOR,
              "FCVTPU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FCVTZU_VECTOR_INTEGER,
              "FCVTZU_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::FRSQRTE,
              "FRSQRTE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16::
                  FSQRT_VECTOR,
              "FSQRT_VECTOR"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension > >
        AdvancedSIMDThreeRegisterExtensionStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::SDOT_VECTOR,
              "SDOT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::USDOT_VECTOR,
              "USDOT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::SQRDMLAH_VECTOR,
              "SQRDMLAH_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::SQRDMLSH_VECTOR,
              "SQRDMLSH_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::UDOT_VECTOR,
              "UDOT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::FCMLA,
              "FCMLA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::FCADD,
              "FCADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::BFDOT_VECTOR,
              "BFDOT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::BFMLALB_VECTOR,
              "BFMLALB_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::SMMLA_VECTOR,
              "SMMLA_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::USMMLA_VECTOR,
              "USMMLA_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::BFMMLA,
              "BFMMLA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension::UMMLA_VECTOR,
              "UMMLA_VECTOR"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous > >
        AdvancedSIMDTwoRegisterMiscellaneousStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::REV64,
              "REV64"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::REV16_VECTOR,
              "REV16_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::SADDLP,
              "SADDLP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::SUQADD,
              "SUQADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::CLS_VECTOR,
              "CLS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::CNT, "CNT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::SADALP,
              "SADALP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::SQABS,
              "SQABS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::CMGT_ZERO,
              "CMGT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::CMEQ_ZERO,
              "CMEQ_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::CMLT_ZERO,
              "CMLT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::ABS, "ABS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::XTN, "XTN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::SQXTN,
              "SQXTN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTN,
              "FCVTN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTL,
              "FCVTL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINTN_VECTOR,
              "FRINTN_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINTM_VECTOR,
              "FRINTM_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTNS_VECTOR,
              "FCVTNS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTMS_VECTOR,
              "FCVTMS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTAS_VECTOR,
              "FCVTAS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::
                  SCVTF_VECTOR_INTEGER,
              "SCVTF_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINT32Z_VECTOR,
              "FRINT32Z_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINT64Z_VECTOR,
              "FRINT64Z_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCMGT_ZERO,
              "FCMGT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCMEQ_ZERO,
              "FCMEQ_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCMLT_ZERO,
              "FCMLT_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FABS_VECTOR,
              "FABS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINTP_VECTOR,
              "FRINTP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINTZ_VECTOR,
              "FRINTZ_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTPS_VECTOR,
              "FCVTPS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::
                  FCVTZS_VECTOR_INTEGER,
              "FCVTZS_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::URECPE,
              "URECPE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRECPE,
              "FRECPE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::BFCVTN,
              "BFCVTN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::REV32_VECTOR,
              "REV32_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::UADDLP,
              "UADDLP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::USQADD,
              "USQADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::CLZ_VECTOR,
              "CLZ_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::UADALP,
              "UADALP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::SQNEG,
              "SQNEG"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::CMGE_ZERO,
              "CMGE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::CMLE_ZERO,
              "CMLE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::NEG_VECTOR,
              "NEG_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::SQXTUN,
              "SQXTUN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::SHLL,
              "SHLL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::UQXTN,
              "UQXTN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTXN,
              "FCVTXN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINTA_VECTOR,
              "FRINTA_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINTX_VECTOR,
              "FRINTX_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTNU_VECTOR,
              "FCVTNU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTMU_VECTOR,
              "FCVTMU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTAU_VECTOR,
              "FCVTAU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::
                  UCVTF_VECTOR_INTEGER,
              "UCVTF_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINT32X_VECTOR,
              "FRINT32X_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINT64X_VECTOR,
              "FRINT64X_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::NOT, "NOT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::RBIT_VECTOR,
              "RBIT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCMGE_ZERO,
              "FCMGE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCMLE_ZERO,
              "FCMLE_ZERO"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FNEG_VECTOR,
              "FNEG_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRINTI_VECTOR,
              "FRINTI_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FCVTPU_VECTOR,
              "FCVTPU_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::
                  FCVTZU_VECTOR_INTEGER,
              "FCVTZU_VECTOR_INTEGER"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::URSQRTE,
              "URSQRTE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FRSQRTE,
              "FRSQRTE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous::FSQRT_VECTOR,
              "FSQRT_VECTOR"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes > >
        AdvancedSIMDAcrossLanesStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::SADDLV, "SADDLV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::SMAXV, "SMAXV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::SMINV, "SMINV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::ADDV, "ADDV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::FMAXNMV_HALF_PRECISION,
              "FMAXNMV_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::FMAXV_HALF_PRECISION,
              "FMAXV_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::FMINNMV_HALF_PRECISION,
              "FMINNMV_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::FMINV_HALF_PRECISION,
              "FMINV_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::UADDLV, "UADDLV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::UMAXV, "UMAXV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::UMINV, "UMINV"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::
                  FMAXNMV_SINGLE_DOUBLE_PRECISION,
              "FMAXNMV_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::
                  FMAXV_SINGLE_DOUBLE_PRECISION,
              "FMAXV_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::
                  FMINNMV_SINGLE_DOUBLE_PRECISION,
              "FMINNMV_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes::
                  FMINV_SINGLE_DOUBLE_PRECISION,
              "FMINV_SINGLE_DOUBLE_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent > >
        AdvancedSIMDThreeDifferentStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SADDL, "SADDL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SADDW, "SADDW"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SSUBL, "SSUBL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SSUBW, "SSUBW"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::ADDHN, "ADDHN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SABAL, "SABAL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SUBHN, "SUBHN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SABDL, "SABDL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SMLAL_VECTOR,
              "SMLAL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SQDMLAL_VECTOR,
              "SQDMLAL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SMLSL_VECTOR,
              "SMLSL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SQDMLSL_VECTOR,
              "SQDMLSL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SMULL_VECTOR,
              "SMULL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::SQDMULL_VECTOR,
              "SQDMULL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::PMULL, "PMULL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::UADDL, "UADDL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::UADDW, "UADDW"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::USUBL, "USUBL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::USUBW, "USUBW"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::RADDHN, "RADDHN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::UABAL, "UABAL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::RSUBHN, "RSUBHN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::UABDL, "UABDL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::UMLAL_VECTOR,
              "UMLAL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::UMLSL_VECTOR,
              "UMLSL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent::UMULL_VECTOR,
              "UMULL_VECTOR"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame > >
        AdvancedSIMDThreeSameStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SHADD, "SHADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SQADD, "SQADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SRHADD, "SRHADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SHSUB, "SHSUB"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SQSUB, "SQSUB"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::CMGTr, "CMGTr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::CMGEr, "CMGEr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SSHL, "SSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SQSHLr, "SQSHLr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SRSHL, "SRSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SQRSHL, "SQRSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SMAX, "SMAX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SMIN, "SMIN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SABD, "SABD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SABA, "SABA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::ADD_VECTOR, "ADD_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::CMTST, "CMTST"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::MLA_VECTOR, "MLA_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::MUL_VECTOR, "MUL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SMAXP, "SMAXP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SMINP, "SMINP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SQDMULH_VECTOR,
              "SQDMULH_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::ADDP_VECTOR, "ADDP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMAXNM_VECTOR,
              "FMAXNM_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMLA_VECTOR, "FMLA_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FADD_VECTOR, "FADD_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMULX, "FMULX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FCMEQr, "FCMEQr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMAX_VECTOR, "FMAX_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FRECPS, "FRECPS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::AND_VECTOR, "AND_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMLAL, "FMLAL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::BICr_VECTOR, "BICr_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMINNM_VECTOR,
              "FMINNM_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMLS_VECTOR, "FMLS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FSUB_VECTOR, "FSUB_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMIN_VECTOR, "FMIN_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FRSQRTS, "FRSQRTS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::ORRr_VECTOR, "ORRr_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMLSL, "FMLSL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::ORN_VECTOR, "ORN_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UHADD, "UHADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UQADD, "UQADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::URHADD, "URHADD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UHSUB, "UHSUB"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UQSUB, "UQSUB"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::CMHIr, "CMHIr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::CMHSr, "CMHSr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::USHL, "USHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UQSHLr, "UQSHLr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::URSHL, "URSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UQRSHL, "UQRSHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UMAX, "UMAX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UMIN, "UMIN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UABD, "UABD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UABA, "UABA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SUB_VECTOR, "SUB_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::CMEQr, "CMEQr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::MLS_VECTOR, "MLS_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::PMUL, "PMUL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UMAXP, "UMAXP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::UMINP, "UMINP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::SQRDMULH_VECTOR,
              "SQRDMULH_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMAXNMP_VECTOR,
              "FMAXNMP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FADDP_VECTOR,
              "FADDP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMUL_VECTOR, "FMUL_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FCMGEr, "FCMGEr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FACGE, "FACGE"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMAXP_VECTOR,
              "FMAXP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FDIV_VECTOR, "FDIV_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::EOR_VECTOR, "EOR_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMLAL2, "FMLAL2"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::BSL, "BSL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMINNMP_VECTOR,
              "FMINNMP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FABD, "FABD"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FCMGTr, "FCMGTr"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FACGT, "FACGT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMINP_VECTOR,
              "FMINP_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::BIT, "BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::FMLSL2, "FMLSL2"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame::BIF, "BIF"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate > >
        AdvancedSIMDModifiedImmediateStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::MOVIi_32BIT_SHIFTED,
              "MOVIi_32BIT_SHIFTED"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::ORRi_32BIT_VECTOR,
              "ORRi_32BIT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::MOVIi_16BIT_SHIFTED,
              "MOVIi_16BIT_SHIFTED"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::ORRi_16BIT_VECTOR,
              "ORRi_16BIT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::
                  MOVI_32BIT_SHIFTING_ONES,
              "MOVI_32BIT_SHIFTING_ONES"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::MOVI_8BIT,
              "MOVI_8BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::
                  FMOVi_VECTOR_SINGLE_PRECISION,
              "FMOVi_VECTOR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::
                  FMOVi_VECTOR_HALF_PRECISION,
              "FMOVi_VECTOR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::MVNIi_32BIT_SHIFTED,
              "MVNIi_32BIT_SHIFTED"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::BICi_32BIT_VECTOR,
              "BICi_32BIT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::MVNIi_16BIT_SHIFTED,
              "MVNIi_16BIT_SHIFTED"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::BICi_16BIT_VECTOR,
              "BICi_16BIT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::
                  MVNI_32BIT_SHIFTING_ONES,
              "MVNI_32BIT_SHIFTING_ONES"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::MOVI_64BIT_SCALAR,
              "MOVI_64BIT_SCALAR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::MOVI_64BIT_VECTOR,
              "MOVI_64BIT_VECTOR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate::
                  FMOVi_VECTOR_DOUBLE_PRECISION,
              "FMOVi_VECTOR_DOUBLE_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate > >
        AdvancedSIMDShiftByImmediateStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SSHR, "SSHR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SSRA, "SSRA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SRSHR, "SRSHR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SRSRA, "SRSRA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SHL, "SHL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SQSHLi, "SQSHLi"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SHRN, "SHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::RSHRN, "RSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SQSHRN, "SQSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SQRSHRN, "SQRSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SSHLL, "SSHLL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::
                  SCVTF_VECTOR_FIXED_POINT,
              "SCVTF_VECTOR_FIXED_POINT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::
                  FCVTZS_VECTOR_FIXED_POINT,
              "FCVTZS_VECTOR_FIXED_POINT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::USHR, "USHR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::USRA, "USRA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::URSHR, "URSHR"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::URSRA, "URSRA"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SRI, "SRI"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SLI, "SLI"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SQSHLU, "SQSHLU"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::UQSHLi, "UQSHLi"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SQSHRUN, "SQSHRUN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::SQRSHRUN,
              "SQRSHRUN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::UQSHRN, "UQSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::UQRSHRN, "UQRSHRN"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::USHLL, "USHLL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::
                  UCVTF_VECTOR_FIXED_POINT,
              "UCVTF_VECTOR_FIXED_POINT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate::
                  FCVTZU_VECTOR_FIXED_POINT,
              "FCVTZU_VECTOR_FIXED_POINT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement > >
        AdvancedSIMDVectorXIndexedElementStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SMLAL_ELEMENT,
              "SMLAL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SQDMLAL_ELEMENT,
              "SQDMLAL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SMLSL_ELEMENT,
              "SMLSL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SQDMLSL_ELEMENT,
              "SQDMLSL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::MUL_ELEMENT,
              "MUL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SMULL_ELEMENT,
              "SMULL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SQDMULL_ELEMENT,
              "SQDMULL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SQDMULH_ELEMENT,
              "SQDMULH_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SQRDMULH_ELEMENT,
              "SQRDMULH_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SDOT_ELEMENT,
              "SDOT_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::
                  FMLA_ELEMENT_VECTOR_HALF_PRECISION,
              "FMLA_ELEMENT_VECTOR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::
                  FMLS_ELEMENT_VECTOR_HALF_PRECISION,
              "FMLS_ELEMENT_VECTOR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::
                  FMUL_ELEMENT_VECTOR_HALF_PRECISION,
              "FMUL_ELEMENT_VECTOR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SUDOT_ELEMENT,
              "SUDOT_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::BFDOT_ELEMENT,
              "BFDOT_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::
                  FMLA_ELEMENT_VECTOR_SINGLE_DOUBLE_PRECISION,
              "FMLA_ELEMENT_VECTOR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::
                  FMLS_ELEMENT_VECTOR_SINGLE_DOUBLE_PRECISION,
              "FMLS_ELEMENT_VECTOR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::
                  FMUL_ELEMENT_VECTOR_SINGLE_DOUBLE_PRECISION,
              "FMUL_ELEMENT_VECTOR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::FMLAL, "FMLAL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::FMLSL, "FMLSL"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::USDOT_ELEMENT,
              "USDOT_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::BFMLALB_ELEMENT,
              "BFMLALB_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::MLA_ELEMENT,
              "MLA_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::UMLAL_ELEMENT,
              "UMLAL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::MLS_ELEMENT,
              "MLS_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::UMLSL_ELEMENT,
              "UMLSL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::UMULL_ELEMENT,
              "UMULL_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SQRDMLAH_ELEMENT,
              "SQRDMLAH_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::UDOT_ELEMENT,
              "UDOT_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::SQRDMLSH_ELEMENT,
              "SQRDMLSH_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::
                  FMULX_ELEMENT_VECTOR_HALF_PRECISION,
              "FMULX_ELEMENT_VECTOR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::FCMLA_ELEMENT,
              "FCMLA_ELEMENT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::
                  FMULX_ELEMENT_VECTOR_SINGLE_DOUBLE_PRECISION,
              "FMULX_ELEMENT_VECTOR_SINGLE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::FMLAL2,
              "FMLAL2"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement::FMLSL2,
              "FMLSL2"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2 > >
        CryptographicThreeRegisterIMM2StringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2::SM3TT1A,
              "SM3TT1A"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2::SM3TT1B,
              "SM3TT1B"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2::SM3TT2A,
              "SM3TT2A"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2::SM3TT2B,
              "SM3TT2B"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512 > >
        CryptographicThreeRegisterSHA512StringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512::SHA512H,
              "SHA512H"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512::SHA512H2,
              "SHA512H2"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512::SHA512SU1,
              "SHA512SU1"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512::RAX1, "RAX1"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512::SM3PARTW1,
              "SM3PARTW1"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512::SM3PARTW2,
              "SM3PARTW2"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512::SM4EKEY,
              "SM4EKEY"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicFourRegister, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicFourRegister > >
        CryptographicFourRegisterStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicFourRegister::EOR3, "EOR3"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicFourRegister::BCAX, "BCAX"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicFourRegister::SM3SS1, "SM3SS1"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingScalarFloatingPointAdvancedSIMDGroup::XAR, std::string_view,
                                    enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::XAR > >
        XARStringMap { { {} } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA512, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA512 > >
        CryptographicTwoRegisterSHA512StringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA512::SHA512SU0,
              "SHA512SU0"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA512::SM4E, "SM4E"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint > >
        ConversionFloatingPointAndFixedPointStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  SCVTF_SCALAR_FIXED_POINT_32BIT_TO_SINGLE_PRECISION,
              "SCVTF_SCALAR_FIXED_POINT_32BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  UCVTF_SCALAR_FIXED_POINT_32BIT_TO_SINGLE_PRECISION,
              "UCVTF_SCALAR_FIXED_POINT_32BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZS_SCALAR_FIXED_POINT_SINGLE_PRECISION_TO_32BIT,
              "FCVTZS_SCALAR_FIXED_POINT_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZU_SCALAR_FIXED_POINT_SINGLE_PRECISION_TO_32BIT,
              "FCVTZU_SCALAR_FIXED_POINT_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  SCVTF_SCALAR_FIXED_POINT_32BIT_TO_DOUBLE_PRECISION,
              "SCVTF_SCALAR_FIXED_POINT_32BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  UCVTF_SCALAR_FIXED_POINT_32BIT_TO_DOUBLE_PRECISION,
              "UCVTF_SCALAR_FIXED_POINT_32BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZS_SCALAR_FIXED_POINT_DOUBLE_PRECISION_TO_32BIT,
              "FCVTZS_SCALAR_FIXED_POINT_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZU_SCALAR_FIXED_POINT_DOUBLE_PRECISION_TO_32BIT,
              "FCVTZU_SCALAR_FIXED_POINT_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  SCVTF_SCALAR_FIXED_POINT_32BIT_TO_HALF_PRECISION,
              "SCVTF_SCALAR_FIXED_POINT_32BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  UCVTF_SCALAR_FIXED_POINT_32BIT_TO_HALF_PRECISION,
              "UCVTF_SCALAR_FIXED_POINT_32BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZS_SCALAR_FIXED_POINT_HALF_PRECISION_TO_32BIT,
              "FCVTZS_SCALAR_FIXED_POINT_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZU_SCALAR_FIXED_POINT_HALF_PRECISION_TO_32BIT,
              "FCVTZU_SCALAR_FIXED_POINT_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  SCVTF_SCALAR_FIXED_POINT_64BIT_TO_SINGLE_PRECISION,
              "SCVTF_SCALAR_FIXED_POINT_64BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  UCVTF_SCALAR_FIXED_POINT_64BIT_TO_SINGLE_PRECISION,
              "UCVTF_SCALAR_FIXED_POINT_64BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZS_SCALAR_FIXED_POINT_SINGLE_PRECISION_TO_64BIT,
              "FCVTZS_SCALAR_FIXED_POINT_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZU_SCALAR_FIXED_POINT_SINGLE_PRECISION_TO_64BIT,
              "FCVTZU_SCALAR_FIXED_POINT_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  SCVTF_SCALAR_FIXED_POINT_64BIT_TO_DOUBLE_PRECISION,
              "SCVTF_SCALAR_FIXED_POINT_64BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  UCVTF_SCALAR_FIXED_POINT_64BIT_TO_DOUBLE_PRECISION,
              "UCVTF_SCALAR_FIXED_POINT_64BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZS_SCALAR_FIXED_POINT_DOUBLE_PRECISION_TO_64BIT,
              "FCVTZS_SCALAR_FIXED_POINT_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZU_SCALAR_FIXED_POINT_DOUBLE_PRECISION_TO_64BIT,
              "FCVTZU_SCALAR_FIXED_POINT_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  SCVTF_SCALAR_FIXED_POINT_64BIT_TO_HALF_PRECISION,
              "SCVTF_SCALAR_FIXED_POINT_64BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  UCVTF_SCALAR_FIXED_POINT_64BIT_TO_HALF_PRECISION,
              "UCVTF_SCALAR_FIXED_POINT_64BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZS_SCALAR_FIXED_POINT_HALF_PRECISION_TO_64BIT,
              "FCVTZS_SCALAR_FIXED_POINT_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint::
                  FCVTZU_SCALAR_FIXED_POINT_HALF_PRECISION_TO_64BIT,
              "FCVTZU_SCALAR_FIXED_POINT_HALF_PRECISION_TO_64BIT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger > >
        ConversionFloatingPointAndIntegerStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNS_SCALAR_SINGLE_PRECISION_TO_32BIT,
              "FCVTNS_SCALAR_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNU_SCALAR_SINGLE_PRECISION_TO_32BIT,
              "FCVTNU_SCALAR_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  SCVTF_SCALAR_INTEGER_32BIT_TO_SINGLE_PRECISION,
              "SCVTF_SCALAR_INTEGER_32BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  UCVTF_SCALAR_INTEGER_32BIT_TO_SINGLE_PRECISION,
              "UCVTF_SCALAR_INTEGER_32BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAS_SCALAR_SINGLE_PRECISION_TO_32BIT,
              "FCVTAS_SCALAR_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAU_SCALAR_SINGLE_PRECISION_TO_32BIT,
              "FCVTAU_SCALAR_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_SINGLE_PRECISION_TO_32BIT,
              "FMOV_GENERAL_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_32BIT_TO_SINGLE_PRECISION,
              "FMOV_GENERAL_32BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPS_SCALAR_SINGLE_PRECISION_TO_32BIT,
              "FCVTPS_SCALAR_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPU_SCALAR_SINGLE_PRECISION_TO_32BIT,
              "FCVTPU_SCALAR_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMS_SCALAR_SINGLE_PRECISION_TO_32BIT,
              "FCVTMS_SCALAR_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMU_SCALAR_SINGLE_PRECISION_TO_32BIT,
              "FCVTMU_SCALAR_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZS_SCALAR_INTEGER_SINGLE_PRECISION_TO_32BIT,
              "FCVTZS_SCALAR_INTEGER_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZU_SCALAR_INTEGER_SINGLE_PRECISION_TO_32BIT,
              "FCVTZU_SCALAR_INTEGER_SINGLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNS_SCALAR_DOUBLE_PRECISION_TO_32BIT,
              "FCVTNS_SCALAR_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNU_SCALAR_DOUBLE_PRECISION_TO_32BIT,
              "FCVTNU_SCALAR_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  SCVTF_SCALAR_INTEGER_32BIT_TO_DOUBLE_PRECISION,
              "SCVTF_SCALAR_INTEGER_32BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  UCVTF_SCALAR_INTEGER_32BIT_TO_DOUBLE_PRECISION,
              "UCVTF_SCALAR_INTEGER_32BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAS_SCALAR_DOUBLE_PRECISION_TO_32BIT,
              "FCVTAS_SCALAR_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAU_SCALAR_DOUBLE_PRECISION_TO_32BIT,
              "FCVTAU_SCALAR_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPS_SCALAR_DOUBLE_PRECISION_TO_32BIT,
              "FCVTPS_SCALAR_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPU_SCALAR_DOUBLE_PRECISION_TO_32BIT,
              "FCVTPU_SCALAR_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMS_SCALAR_DOUBLE_PRECISION_TO_32BIT,
              "FCVTMS_SCALAR_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMU_SCALAR_DOUBLE_PRECISION_TO_32BIT,
              "FCVTMU_SCALAR_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZS_SCALAR_INTEGER_DOUBLE_PRECISION_TO_32BIT,
              "FCVTZS_SCALAR_INTEGER_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZU_SCALAR_INTEGER_DOUBLE_PRECISION_TO_32BIT,
              "FCVTZU_SCALAR_INTEGER_DOUBLE_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::FJCVTZS,
              "FJCVTZS"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNS_SCALAR_HALF_PRECISION_TO_32BIT,
              "FCVTNS_SCALAR_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNU_SCALAR_HALF_PRECISION_TO_32BIT,
              "FCVTNU_SCALAR_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  SCVTF_SCALAR_INTEGER_32BIT_TO_HALF_PRECISION,
              "SCVTF_SCALAR_INTEGER_32BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  UCVTF_SCALAR_INTEGER_32BIT_TO_HALF_PRECISION,
              "UCVTF_SCALAR_INTEGER_32BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAS_SCALAR_HALF_PRECISION_TO_32BIT,
              "FCVTAS_SCALAR_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAU_SCALAR_HALF_PRECISION_TO_32BIT,
              "FCVTAU_SCALAR_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_HALF_PRECISION_TO_32BIT,
              "FMOV_GENERAL_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_32BIT_TO_HALF_PRECISION,
              "FMOV_GENERAL_32BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPS_SCALAR_HALF_PRECISION_TO_32BIT,
              "FCVTPS_SCALAR_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPU_SCALAR_HALF_PRECISION_TO_32BIT,
              "FCVTPU_SCALAR_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMS_SCALAR_HALF_PRECISION_TO_32BIT,
              "FCVTMS_SCALAR_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMU_SCALAR_HALF_PRECISION_TO_32BIT,
              "FCVTMU_SCALAR_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZS_SCALAR_INTEGER_HALF_PRECISION_TO_32BIT,
              "FCVTZS_SCALAR_INTEGER_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZU_SCALAR_INTEGER_HALF_PRECISION_TO_32BIT,
              "FCVTZU_SCALAR_INTEGER_HALF_PRECISION_TO_32BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNS_SCALAR_SINGLE_PRECISION_TO_64BIT,
              "FCVTNS_SCALAR_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNU_SCALAR_SINGLE_PRECISION_TO_64BIT,
              "FCVTNU_SCALAR_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  SCVTF_SCALAR_INTEGER_64BIT_TO_SINGLE_PRECISION,
              "SCVTF_SCALAR_INTEGER_64BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  UCVTF_SCALAR_INTEGER_64BIT_TO_SINGLE_PRECISION,
              "UCVTF_SCALAR_INTEGER_64BIT_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAS_SCALAR_SINGLE_PRECISION_TO_64BIT,
              "FCVTAS_SCALAR_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAU_SCALAR_SINGLE_PRECISION_TO_64BIT,
              "FCVTAU_SCALAR_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPS_SCALAR_SINGLE_PRECISION_TO_64BIT,
              "FCVTPS_SCALAR_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPU_SCALAR_SINGLE_PRECISION_TO_64BIT,
              "FCVTPU_SCALAR_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMS_SCALAR_SINGLE_PRECISION_TO_64BIT,
              "FCVTMS_SCALAR_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMU_SCALAR_SINGLE_PRECISION_TO_64BIT,
              "FCVTMU_SCALAR_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZS_SCALAR_INTEGER_SINGLE_PRECISION_TO_64BIT,
              "FCVTZS_SCALAR_INTEGER_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZU_SCALAR_INTEGER_SINGLE_PRECISION_TO_64BIT,
              "FCVTZU_SCALAR_INTEGER_SINGLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNS_SCALAR_DOUBLE_PRECISION_TO_64BIT,
              "FCVTNS_SCALAR_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNU_SCALAR_DOUBLE_PRECISION_TO_64BIT,
              "FCVTNU_SCALAR_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  SCVTF_SCALAR_INTEGER_64BIT_TO_DOUBLE_PRECISION,
              "SCVTF_SCALAR_INTEGER_64BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  UCVTF_SCALAR_INTEGER_64BIT_TO_DOUBLE_PRECISION,
              "UCVTF_SCALAR_INTEGER_64BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAS_SCALAR_DOUBLE_PRECISION_TO_64BIT,
              "FCVTAS_SCALAR_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAU_SCALAR_DOUBLE_PRECISION_TO_64BIT,
              "FCVTAU_SCALAR_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_DOUBLE_PRECISION_TO_64BIT,
              "FMOV_GENERAL_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_64BIT_TO_DOUBLE_PRECISION,
              "FMOV_GENERAL_64BIT_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPS_SCALAR_DOUBLE_PRECISION_TO_64BIT,
              "FCVTPS_SCALAR_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPU_SCALAR_DOUBLE_PRECISION_TO_64BIT,
              "FCVTPU_SCALAR_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMS_SCALAR_DOUBLE_PRECISION_TO_64BIT,
              "FCVTMS_SCALAR_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMU_SCALAR_DOUBLE_PRECISION_TO_64BIT,
              "FCVTMU_SCALAR_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZS_SCALAR_INTEGER_DOUBLE_PRECISION_TO_64BIT,
              "FCVTZS_SCALAR_INTEGER_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZU_SCALAR_INTEGER_DOUBLE_PRECISION_TO_64BIT,
              "FCVTZU_SCALAR_INTEGER_DOUBLE_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_TOP_HALF_128BIT_TO_64BIT,
              "FMOV_GENERAL_TOP_HALF_128BIT_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_64BIT_TO_TOP_HALF_128BIT,
              "FMOV_GENERAL_64BIT_TO_TOP_HALF_128BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNS_SCALAR_HALF_PRECISION_TO_64BIT,
              "FCVTNS_SCALAR_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTNU_SCALAR_HALF_PRECISION_TO_64BIT,
              "FCVTNU_SCALAR_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  SCVTF_SCALAR_INTEGER_64BIT_TO_HALF_PRECISION,
              "SCVTF_SCALAR_INTEGER_64BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  UCVTF_SCALAR_INTEGER_64BIT_TO_HALF_PRECISION,
              "UCVTF_SCALAR_INTEGER_64BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAS_SCALAR_HALF_PRECISION_TO_64BIT,
              "FCVTAS_SCALAR_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTAU_SCALAR_HALF_PRECISION_TO_64BIT,
              "FCVTAU_SCALAR_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_HALF_PRECISION_TO_64BIT,
              "FMOV_GENERAL_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FMOV_GENERAL_64BIT_TO_HALF_PRECISION,
              "FMOV_GENERAL_64BIT_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPS_SCALAR_HALF_PRECISION_TO_64BIT,
              "FCVTPS_SCALAR_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTPU_SCALAR_HALF_PRECISION_TO_64BIT,
              "FCVTPU_SCALAR_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMS_SCALAR_HALF_PRECISION_TO_64BIT,
              "FCVTMS_SCALAR_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTMU_SCALAR_HALF_PRECISION_TO_64BIT,
              "FCVTMU_SCALAR_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZS_SCALAR_INTEGER_HALF_PRECISION_TO_64BIT,
              "FCVTZS_SCALAR_INTEGER_HALF_PRECISION_TO_64BIT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger::
                  FCVTZU_SCALAR_INTEGER_HALF_PRECISION_TO_64BIT,
              "FCVTZU_SCALAR_INTEGER_HALF_PRECISION_TO_64BIT"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource > >
        FloatingPointDataProcessingOneSourceStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FMOVr_SINGLE_PRECISION,
              "FMOVr_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FABS_SCALAR_SINGLE_PRECISION,
              "FABS_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FNEG_SCALAR_SINGLE_PRECISION,
              "FNEG_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FSQRT_SCALAR_SINGLE_PRECISION,
              "FSQRT_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FCVT_SINGLE_PRECISION_TO_DOUBLE_PRECISION,
              "FCVT_SINGLE_PRECISION_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FCVT_SINGLE_PRECISION_TO_HALF_PRECISION,
              "FCVT_SINGLE_PRECISION_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTN_SCALAR_SINGLE_PRECISION,
              "FRINTN_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTP_SCALAR_SINGLE_PRECISION,
              "FRINTP_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTM_SCALAR_SINGLE_PRECISION,
              "FRINTM_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTZ_SCALAR_SINGLE_PRECISION,
              "FRINTZ_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTA_SCALAR_SINGLE_PRECISION,
              "FRINTA_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTX_SCALAR_SINGLE_PRECISION,
              "FRINTX_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTI_SCALAR_SINGLE_PRECISION,
              "FRINTI_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINT32Z_SCALAR_SINGLE_PRECISION,
              "FRINT32Z_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINT32X_SCALAR_SINGLE_PRECISION,
              "FRINT32X_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINT64Z_SCALAR_SINGLE_PRECISION,
              "FRINT64Z_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINT64X_SCALAR_SINGLE_PRECISION,
              "FRINT64X_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FMOVr_DOUBLE_PRECISION,
              "FMOVr_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FABS_SCALAR_DOUBLE_PRECISION,
              "FABS_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FNEG_SCALAR_DOUBLE_PRECISION,
              "FNEG_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FSQRT_SCALAR_DOUBLE_PRECISION,
              "FSQRT_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FCVT_DOUBLE_PRECISION_TO_SINGLE_PRECISION,
              "FCVT_DOUBLE_PRECISION_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::BFCVT,
              "BFCVT"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FCVT_DOUBLE_PRECISION_TO_HALF_PRECISION,
              "FCVT_DOUBLE_PRECISION_TO_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTN_SCALAR_DOUBLE_PRECISION,
              "FRINTN_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTP_SCALAR_DOUBLE_PRECISION,
              "FRINTP_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTM_SCALAR_DOUBLE_PRECISION,
              "FRINTM_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTZ_SCALAR_DOUBLE_PRECISION,
              "FRINTZ_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTA_SCALAR_DOUBLE_PRECISION,
              "FRINTA_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTX_SCALAR_DOUBLE_PRECISION,
              "FRINTX_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTI_SCALAR_DOUBLE_PRECISION,
              "FRINTI_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINT32Z_SCALAR_DOUBLE_PRECISION,
              "FRINT32Z_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINT32X_SCALAR_DOUBLE_PRECISION,
              "FRINT32X_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINT64Z_SCALAR_DOUBLE_PRECISION,
              "FRINT64Z_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINT64X_SCALAR_DOUBLE_PRECISION,
              "FRINT64X_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FMOVr_HALF_PRECISION,
              "FMOVr_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FABS_SCALAR_HALF_PRECISION,
              "FABS_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FNEG_SCALAR_HALF_PRECISION,
              "FNEG_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FSQRT_SCALAR_HALF_PRECISION,
              "FSQRT_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FCVT_HALF_PRECISION_TO_SINGLE_PRECISION,
              "FCVT_HALF_PRECISION_TO_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FCVT_HALF_PRECISION_TO_DOUBLE_PRECISION,
              "FCVT_HALF_PRECISION_TO_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTN_SCALAR_HALF_PRECISION,
              "FRINTN_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTP_SCALAR_HALF_PRECISION,
              "FRINTP_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTM_SCALAR_HALF_PRECISION,
              "FRINTM_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTZ_SCALAR_HALF_PRECISION,
              "FRINTZ_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTA_SCALAR_HALF_PRECISION,
              "FRINTA_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTX_SCALAR_HALF_PRECISION,
              "FRINTX_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource::
                  FRINTI_SCALAR_HALF_PRECISION,
              "FRINTI_SCALAR_HALF_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointCompare, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointCompare > >
        FloatingPointCompareStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointCompare::FCMP, "FCMP"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointCompare::FCMPE, "FCMPE"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointImmediate, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointImmediate > >
        FloatingPointImmediateStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointImmediate::FMOVi_SCALAR_SINGLE_PRECISION,
              "FMOVi_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointImmediate::FMOVi_SCALAR_DOUBLE_PRECISION,
              "FMOVi_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointImmediate::FMOVi_SCALAR_HALF_PRECISION,
              "FMOVi_SCALAR_HALF_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare > >
        FloatingPointConditionalCompareStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare::
                  FCCMP_SINGLE_PRECISION,
              "FCCMP_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare::
                  FCCMPE_SINGLE_PRECISION,
              "FCCMPE_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare::
                  FCCMP_DOUBLE_PRECISION,
              "FCCMP_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare::
                  FCCMPE_DOUBLE_PRECISION,
              "FCCMPE_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare::FCCMP_HALF_PRECISION,
              "FCCMP_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare::
                  FCCMPE_HALF_PRECISION,
              "FCCMPE_HALF_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource > >
        FloatingPointDataProcessingTwoSourceStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMUL_SCALAR_SINGLE_PRECISION,
              "FMUL_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FDIV_SCALAR_SINGLE_PRECISION,
              "FDIV_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FADD_SCALAR_SINGLE_PRECISION,
              "FADD_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FSUB_SCALAR_SINGLE_PRECISION,
              "FSUB_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMAX_SCALAR_SINGLE_PRECISION,
              "FMAX_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMIN_SCALAR_SINGLE_PRECISION,
              "FMIN_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMAXNM_SCALAR_SINGLE_PRECISION,
              "FMAXNM_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMINNM_SCALAR_SINGLE_PRECISION,
              "FMINNM_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FNMUL_SCALAR_SINGLE_PRECISION,
              "FNMUL_SCALAR_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMUL_SCALAR_DOUBLE_PRECISION,
              "FMUL_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FDIV_SCALAR_DOUBLE_PRECISION,
              "FDIV_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FADD_SCALAR_DOUBLE_PRECISION,
              "FADD_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FSUB_SCALAR_DOUBLE_PRECISION,
              "FSUB_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMAX_SCALAR_DOUBLE_PRECISION,
              "FMAX_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMIN_SCALAR_DOUBLE_PRECISION,
              "FMIN_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMAXNM_SCALAR_DOUBLE_PRECISION,
              "FMAXNM_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMINNM_SCALAR_DOUBLE_PRECISION,
              "FMINNM_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FNMUL_SCALAR_DOUBLE_PRECISION,
              "FNMUL_SCALAR_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMUL_SCALAR_HALF_PRECISION,
              "FMUL_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FDIV_SCALAR_HALF_PRECISION,
              "FDIV_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FADD_SCALAR_HALF_PRECISION,
              "FADD_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FSUB_SCALAR_HALF_PRECISION,
              "FSUB_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMAX_SCALAR_HALF_PRECISION,
              "FMAX_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMIN_SCALAR_HALF_PRECISION,
              "FMIN_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMAXNM_SCALAR_HALF_PRECISION,
              "FMAXNM_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FMINNM_SCALAR_HALF_PRECISION,
              "FMINNM_SCALAR_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource::
                  FNMUL_SCALAR_HALF_PRECISION,
              "FNMUL_SCALAR_HALF_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalSelect, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalSelect > >
        FloatingPointConditionalSelectStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalSelect::
                  FCSEL_SINGLE_PRECISION,
              "FCSEL_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalSelect::
                  FCSEL_DOUBLE_PRECISION,
              "FCSEL_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalSelect::FCSEL_HALF_PRECISION,
              "FCSEL_HALF_PRECISION"sv },
        } } };

    static constexpr SmallConstMap<
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource, std::string_view,
        enum_size_v< DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource > >
        FloatingPointDataProcessingThreeSourceStringMap { { {
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FMADD_SINGLE_PRECISION,
              "FMADD_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FMSUB_SINGLE_PRECISION,
              "FMSUB_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FNMADD_SINGLE_PRECISION,
              "FNMADD_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FNMSUB_SINGLE_PRECISION,
              "FNMSUB_SINGLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FMADD_DOUBLE_PRECISION,
              "FMADD_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FMSUB_DOUBLE_PRECISION,
              "FMSUB_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FNMADD_DOUBLE_PRECISION,
              "FNMADD_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FNMSUB_DOUBLE_PRECISION,
              "FNMSUB_DOUBLE_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FMADD_HALF_PRECISION,
              "FMADD_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FMSUB_HALF_PRECISION,
              "FMSUB_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FNMADD_HALF_PRECISION,
              "FNMADD_HALF_PRECISION"sv },
            { DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource::
                  FNMSUB_HALF_PRECISION,
              "FNMSUB_HALF_PRECISION"sv },
        } } };

} // namespace

template <>
static std::string_view Enum::ToChar(A64DataProcessingScalarFloatingPointAdvancedSIMDGroup elem) {
    return A64DataProcessingScalarFloatingPointAdvancedSIMDGroupStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, A64DataProcessingScalarFloatingPointAdvancedSIMDGroup elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES elem) {
    return CryptographicAESStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA elem) {
    return CryptographicThreeRegisterSHAStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                     os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA elem) {
    return CryptographicTwoRegisterSHAStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                   os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarCopy elem) {
    return AdvancedSIMDScalarCopyStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                              os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarCopy elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16 elem) {
    return AdvancedSIMDScalarThreeSameFP16StringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                       os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16 elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(
    DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16 elem) {
    return AdvancedSIMDScalarTwoRegisterMiscellaneousFP16StringMap.At(elem);
}

template <>
std::ostream& operator<<(
    std::ostream&                                                                                      os,
    DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16 elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameExtraction elem) {
    return AdvancedSIMDScalarThreeSameExtractionStringMap.At(elem);
}

template <>
std::ostream&
    operator<<(std::ostream&                                                                             os,
               DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameExtraction elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous elem) {
    return AdvancedSIMDScalarTwoRegisterMiscellaneousStringMap.At(elem);
}

template <>
std::ostream&
    operator<<(std::ostream&                                                                                  os,
               DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise elem) {
    return AdvancedSIMDScalarPairwiseStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                  os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeDifferent elem) {
    return AdvancedSIMDScalarThreeDifferentStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                        os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeDifferent elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame elem) {
    return AdvancedSIMDScalarThreeSameStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                   os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate elem) {
    return AdvancedSIMDScalarShiftByImmediateStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                          os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement elem) {
    return AdvancedSIMDScalarXIndexedElementStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                         os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup elem) {
    return AdvancedSIMDTableLookupStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                               os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute elem) {
    return AdvancedSIMDPermuteStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                           os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDExtract elem) {
    return AdvancedSIMDExtractStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                           os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDExtract elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy elem) {
    return AdvancedSIMDCopyStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16 elem) {
    return AdvancedSIMDThreeSameFP16StringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                 os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16 elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16 elem) {
    return AdvancedSIMDTwoRegisterMiscellaneousFP16StringMap.At(elem);
}

template <>
std::ostream&
    operator<<(std::ostream&                                                                                os,
               DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16 elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension elem) {
    return AdvancedSIMDThreeRegisterExtensionStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                          os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous elem) {
    return AdvancedSIMDTwoRegisterMiscellaneousStringMap.At(elem);
}

template <>
std::ostream&
    operator<<(std::ostream&                                                                            os,
               DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes elem) {
    return AdvancedSIMDAcrossLanesStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                               os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent elem) {
    return AdvancedSIMDThreeDifferentStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                  os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame elem) {
    return AdvancedSIMDThreeSameStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                             os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate elem) {
    return AdvancedSIMDModifiedImmediateStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                     os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate elem) {
    return AdvancedSIMDShiftByImmediateStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                    os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement elem) {
    return AdvancedSIMDVectorXIndexedElementStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                         os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2 elem) {
    return CryptographicThreeRegisterIMM2StringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                      os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2 elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512 elem) {
    return CryptographicThreeRegisterSHA512StringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                        os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512 elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicFourRegister elem) {
    return CryptographicFourRegisterStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                 os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicFourRegister elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::XAR elem) {
    return XARStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingScalarFloatingPointAdvancedSIMDGroup::XAR elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA512 elem) {
    return CryptographicTwoRegisterSHA512StringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                      os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA512 elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint elem) {
    return ConversionFloatingPointAndFixedPointStringMap.At(elem);
}

template <>
std::ostream&
    operator<<(std::ostream&                                                                            os,
               DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger elem) {
    return ConversionFloatingPointAndIntegerStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                         os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource elem) {
    return FloatingPointDataProcessingOneSourceStringMap.At(elem);
}

template <>
std::ostream&
    operator<<(std::ostream&                                                                            os,
               DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointCompare elem) {
    return FloatingPointCompareStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                            os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointCompare elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointImmediate elem) {
    return FloatingPointImmediateStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                              os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointImmediate elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare elem) {
    return FloatingPointConditionalCompareStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                       os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource elem) {
    return FloatingPointDataProcessingTwoSourceStringMap.At(elem);
}

template <>
std::ostream&
    operator<<(std::ostream&                                                                            os,
               DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalSelect elem) {
    return FloatingPointConditionalSelectStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream&                                                                      os,
                         DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalSelect elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view
    Enum::ToChar(DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource elem) {
    return FloatingPointDataProcessingThreeSourceStringMap.At(elem);
}

template <>
std::ostream&
    operator<<(std::ostream&                                                                              os,
               DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource elem) {
    os << Enum::ToChar(elem);
    return os;
}

END_NAMESPACE
