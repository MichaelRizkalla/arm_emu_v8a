
#include <InstructionSet/A64InstructionSet.h>
#include <Utility/StreamableEnum.h>
#include <ostream>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< A64LoadStoreGroup, std::string_view, enum_size_v< A64LoadStoreGroup > >
        A64LoadStoreGroupStringMap { { {
            { A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES,
              "ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES"sv },
            { A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES_POST_INDEXED,
              "ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES_POST_INDEXED"sv },
            { A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE,
              "ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE"sv },
            { A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE_POST_INDEXED,
              "ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE_POST_INDEXED"sv },
            { A64LoadStoreGroup::LOAD_STORE_MEMORY_TAG, "LOAD_STORE_MEMORY_TAG"sv },
            { A64LoadStoreGroup::LOAD_STORE_EXCLUSIVE, "LOAD_STORE_EXCLUSIVE"sv },
            { A64LoadStoreGroup::LDAPR_STLR_UNSCALED_IMMEDIATE, "LDAPR_STLR_UNSCALED_IMMEDIATE"sv },
            { A64LoadStoreGroup::LOAD_REGISTER_LITERAL, "LOAD_REGISTER_LITERAL"sv },
            { A64LoadStoreGroup::LOAD_STORE_NO_ALLOCATE_PAIR_OFFSET, "LOAD_STORE_NO_ALLOCATE_PAIR_OFFSET"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_POST_INDEXED, "LOAD_STORE_REGISTER_PAIR_POST_INDEXED"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_OFFSET, "LOAD_STORE_REGISTER_PAIR_OFFSET"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_PRE_INDEXED, "LOAD_STORE_REGISTER_PAIR_PRE_INDEXED"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_UNSCALED_IMMEDIATE, "LOAD_STORE_REGISTER_UNSCALED_IMMEDIATE"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED,
              "LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_UNPRIVILEGED, "LOAD_STORE_REGISTER_UNPRIVILEGED"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_IMMEDIATE_PRE_INDEXED,
              "LOAD_STORE_REGISTER_IMMEDIATE_PRE_INDEXED"sv },
            { A64LoadStoreGroup::ATOMIC_MEMORY_OPERATION, "ATOMIC_MEMORY_OPERATION"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_REGISTER_OFFSET, "LOAD_STORE_REGISTER_REGISTER_OFFSET"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_PAC, "LOAD_STORE_REGISTER_PAC"sv },
            { A64LoadStoreGroup::LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE, "LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures, std::string_view,
                                    enum_size_v< LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures > >
        AdvancedSIMDLoadStoreMultipleStructuresStringMap { { {
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::ST4_MULTI, "ST4_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::ST1_4R_MULTI, "ST1_4R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::ST3_MULTI, "ST3_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::ST1_3R_MULTI, "ST1_3R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::ST1_1R_MULTI, "ST1_1R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::ST2_MULTI, "ST2_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::ST1_2R_MULTI, "ST1_2R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::LD4_MULTI, "LD4_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::LD1_4R_MULTI, "LD1_4R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::LD3_MULTI, "LD3_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::LD1_3R_MULTI, "LD1_3R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::LD1_1R_MULTI, "LD1_1R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::LD2_MULTI, "LD2_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures::LD1_2R_MULTI, "LD1_2R_MULTI"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed,
                                    std::string_view,
                                    enum_size_v< LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed > >
        AdvancedSIMDLoadStoreMultipleStructuresPostIndexedStringMap { { {
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST4r_MULTI, "ST4r_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST1r_4R_MULTI, "ST1r_4R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST3r_MULTI, "ST3r_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST1r_3R_MULTI, "ST1r_3R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST1r_1R_MULTI, "ST1r_1R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST2r_MULTI, "ST2r_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST1r_2R_MULTI, "ST1r_2R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST4i_MULTI, "ST4i_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST1i_4R_MULTI, "ST1i_4R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST3i_MULTI, "ST3i_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST1i_3R_MULTI, "ST1i_3R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST1i_1R_MULTI, "ST1i_1R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST2i_MULTI, "ST2i_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::ST1i_2R_MULTI, "ST1i_2R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD4r_MULTI, "LD4r_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD1r_4R_MULTI, "LD1r_4R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD3r_MULTI, "LD3r_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD1r_3R_MULTI, "LD1r_3R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD1r_1R_MULTI, "LD1r_1R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD2r_MULTI, "LD2r_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD1r_2R_MULTI, "LD1r_2R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD4i_MULTI, "LD4i_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD1i_4R_MULTI, "LD1i_4R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD3i_MULTI, "LD3i_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD1i_3R_MULTI, "LD1i_3R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD1i_1R_MULTI, "LD1i_1R_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD2i_MULTI, "LD2i_MULTI"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed::LD1i_2R_MULTI, "LD1i_2R_MULTI"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure, std::string_view,
                                    enum_size_v< LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure > >
        AdvancedSIMDLoadStoreSingleStructureStringMap { { {
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST1_8BIT_SINGLE, "ST1_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST3_8BIT_SINGLE, "ST3_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST1_16BIT_SINGLE, "ST1_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST3_16BIT_SINGLE, "ST3_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST1_32BIT_SINGLE, "ST1_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST1_64BIT_SINGLE, "ST1_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST3_32BIT_SINGLE, "ST3_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST3_64BIT_SINGLE, "ST3_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST2_8BIT_SINGLE, "ST2_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST4_8BIT_SINGLE, "ST4_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST2_16BIT_SINGLE, "ST2_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST4_16BIT_SINGLE, "ST4_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST2_32BIT_SINGLE, "ST2_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST2_64BIT_SINGLE, "ST2_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST4_32BIT_SINGLE, "ST4_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::ST4_64BIT_SINGLE, "ST4_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD1_8BIT_SINGLE, "LD1_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD3_8BIT_SINGLE, "LD3_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD1_16BIT_SINGLE, "LD1_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD3_16BIT_SINGLE, "LD3_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD1_32BIT_SINGLE, "LD1_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD1_64BIT_SINGLE, "LD1_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD3_32BIT_SINGLE, "LD3_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD3_64BIT_SINGLE, "LD3_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD1R, "LD1R"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD3R, "LD3R"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD2_8BIT_SINGLE, "LD2_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD4_8BIT_SINGLE, "LD4_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD2_16BIT_SINGLE, "LD2_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD4_16BIT_SINGLE, "LD4_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD2_32BIT_SINGLE, "LD2_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD2_64BIT_SINGLE, "LD2_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD4_32BIT_SINGLE, "LD4_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD4_64BIT_SINGLE, "LD4_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD2R, "LD2R"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure::LD4R, "LD4R"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed, std::string_view,
                                    enum_size_v< LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed > >
        AdvancedSIMDLoadStoreSingleStructurePostIndexedStringMap { { {
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST1r_8BIT_SINGLE, "ST1r_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST3r_8BIT_SINGLE, "ST3r_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST1r_16BIT_SINGLE,
              "ST1r_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST3r_16BIT_SINGLE,
              "ST3r_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST1r_32BIT_SINGLE,
              "ST1r_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST1r_64BIT_SINGLE,
              "ST1r_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST3r_32BIT_SINGLE,
              "ST3r_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST3r_64BIT_SINGLE,
              "ST3r_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST1i_8BIT_SINGLE, "ST1i_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST3i_8BIT_SINGLE, "ST3i_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST1i_16BIT_SINGLE,
              "ST1i_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST3i_16BIT_SINGLE,
              "ST3i_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST1i_32BIT_SINGLE,
              "ST1i_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST1i_64BIT_SINGLE,
              "ST1i_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST3i_32BIT_SINGLE,
              "ST3i_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST3i_64BIT_SINGLE,
              "ST3i_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST2r_8BIT_SINGLE, "ST2r_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST4r_8BIT_SINGLE, "ST4r_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST2r_16BIT_SINGLE,
              "ST2r_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST4r_16BIT_SINGLE,
              "ST4r_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST2r_32BIT_SINGLE,
              "ST2r_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST2r_64BIT_SINGLE,
              "ST2r_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST4r_32BIT_SINGLE,
              "ST4r_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST4r_64BIT_SINGLE,
              "ST4r_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST2i_8BIT_SINGLE, "ST2i_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST4i_8BIT_SINGLE, "ST4i_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST2i_16BIT_SINGLE,
              "ST2i_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST4i_16BIT_SINGLE,
              "ST4i_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST2i_32BIT_SINGLE,
              "ST2i_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST2i_64BIT_SINGLE,
              "ST2i_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST4i_32BIT_SINGLE,
              "ST4i_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::ST4i_64BIT_SINGLE,
              "ST4i_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1r_8BIT_SINGLE, "LD1r_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3r_8BIT_SINGLE, "LD3r_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1r_16BIT_SINGLE,
              "LD1r_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3r_16BIT_SINGLE,
              "LD3r_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1r_32BIT_SINGLE,
              "LD1r_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1r_64BIT_SINGLE,
              "LD1r_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3r_32BIT_SINGLE,
              "LD3r_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3r_64BIT_SINGLE,
              "LD3r_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1Rr, "LD1Rr"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3Rr, "LD3Rr"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1i_8BIT_SINGLE, "LD1i_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3i_8BIT_SINGLE, "LD3i_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1i_16BIT_SINGLE,
              "LD1i_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3i_16BIT_SINGLE,
              "LD3i_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1i_32BIT_SINGLE,
              "LD1i_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1i_64BIT_SINGLE,
              "LD1i_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3i_32BIT_SINGLE,
              "LD3i_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3i_64BIT_SINGLE,
              "LD3i_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD1Ri, "LD1Ri"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD3Ri, "LD3Ri"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2r_8BIT_SINGLE, "LD2r_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4r_8BIT_SINGLE, "LD4r_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2r_16BIT_SINGLE,
              "LD2r_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4r_16BIT_SINGLE,
              "LD4r_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2r_32BIT_SINGLE,
              "LD2r_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2r_64BIT_SINGLE,
              "LD2r_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4r_32BIT_SINGLE,
              "LD4r_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4r_64BIT_SINGLE,
              "LD4r_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2Rr, "LD2Rr"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4Rr, "LD4Rr"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2i_8BIT_SINGLE, "LD2i_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4i_8BIT_SINGLE, "LD4i_8BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2i_16BIT_SINGLE,
              "LD2i_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4i_16BIT_SINGLE,
              "LD4i_16BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2i_32BIT_SINGLE,
              "LD2i_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2i_64BIT_SINGLE,
              "LD2i_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4i_32BIT_SINGLE,
              "LD4i_32BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4i_64BIT_SINGLE,
              "LD4i_64BIT_SINGLE"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD2Ri, "LD2Ri"sv },
            { LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed::LD4Ri, "LD4Ri"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreMemoryTag, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreMemoryTag > >
        LoadStoreMemoryTagStringMap { { {
            { LoadStoreGroup::LoadStoreMemoryTag::STG_ENCODNIG, "STG_ENCODNIG"sv },
            { LoadStoreGroup::LoadStoreMemoryTag::STZGM, "STZGM"sv },
            { LoadStoreGroup::LoadStoreMemoryTag::LDG, "LDG"sv },
            { LoadStoreGroup::LoadStoreMemoryTag::STZG_ENCODNIG, "STZG_ENCODNIG"sv },
            { LoadStoreGroup::LoadStoreMemoryTag::ST2G_ENCODNIG, "ST2G_ENCODNIG"sv },
            { LoadStoreGroup::LoadStoreMemoryTag::STGM, "STGM"sv },
            { LoadStoreGroup::LoadStoreMemoryTag::STZ2G_ENCODNIG, "STZ2G_ENCODNIG"sv },
            { LoadStoreGroup::LoadStoreMemoryTag::LDGM, "LDGM"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreExclusive, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreExclusive > >
        LoadStoreExclusiveStringMap { { {
            { LoadStoreGroup::LoadStoreExclusive::STXRB, "STXRB"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLXRB, "STLXRB"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASP_32BIT, "CASP_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASPL_32BIT, "CASPL_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDXRB, "LDXRB"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDAXRB, "LDAXRB"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASPA_32BIT, "CASPA_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASPAL_32BIT, "CASPAL_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLLRB, "STLLRB"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLRB, "STLRB"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASB, "CASB"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASLB, "CASLB"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDLARB, "LDLARB"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDARB, "LDARB"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASAB, "CASAB"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASALB, "CASALB"sv },
            { LoadStoreGroup::LoadStoreExclusive::STXRH, "STXRH"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLXRH, "STLXRH"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASP_64BIT, "CASP_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASPL_64BIT, "CASPL_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDXRH, "LDXRH"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDAXRH, "LDAXRH"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASPA_64BIT, "CASPA_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASPAL_64BIT, "CASPAL_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLLRH, "STLLRH"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLRH, "STLRH"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASH, "CASH"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASLH, "CASLH"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDLARH, "LDLARH"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDARH, "LDARH"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASAH, "CASAH"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASALH, "CASALH"sv },
            { LoadStoreGroup::LoadStoreExclusive::STXR_32BIT, "STXR_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLXR_32BIT, "STLXR_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STXP_32BIT, "STXP_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLXP_32BIT, "STLXP_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDXR_32BIT, "LDXR_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDAXR_32BIT, "LDAXR_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDXP_32BIT, "LDXP_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDAXP_32BIT, "LDAXP_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLLR_32BIT, "STLLR_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLR_32BIT, "STLR_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CAS_32BIT, "CAS_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASL_32BIT, "CASL_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDLAR_32BIT, "LDLAR_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDAR_32BIT, "LDAR_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASA_32BIT, "CASA_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASAL_32BIT, "CASAL_32BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STXR_64BIT, "STXR_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLXR_64BIT, "STLXR_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STXP_64BIT, "STXP_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLXP_64BIT, "STLXP_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDXR_64BIT, "LDXR_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDAXR_64BIT, "LDAXR_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDXP_64BIT, "LDXP_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDAXP_64BIT, "LDAXP_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLLR_64BIT, "STLLR_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::STLR_64BIT, "STLR_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CAS_64BIT, "CAS_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASL_64BIT, "CASL_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDLAR_64BIT, "LDLAR_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::LDAR_64BIT, "LDAR_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASA_64BIT, "CASA_64BIT"sv },
            { LoadStoreGroup::LoadStoreExclusive::CASAL_64BIT, "CASAL_64BIT"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LdaprStlrUnscaledImmediate, std::string_view,
                                    enum_size_v< LoadStoreGroup::LdaprStlrUnscaledImmediate > >
        LdaprStlrUnscaledImmediateStringMap { { {
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::STLURB, "STLURB"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPURB, "LDAPURB"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPURSB_64BIT, "LDAPURSB_64BIT"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPURSB_32BIT, "LDAPURSB_32BIT"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::STLURH, "STLURH"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPURH, "LDAPURH"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPURSH_64BIT, "LDAPURSH_64BIT"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPURSH_32BIT, "LDAPURSH_32BIT"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::STLUR_32BIT, "STLUR_32BIT"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPUR_32BIT, "LDAPUR_32BIT"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPURSW, "LDAPURSW"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::STLUR_64BIT, "STLUR_64BIT"sv },
            { LoadStoreGroup::LdaprStlrUnscaledImmediate::LDAPUR_64BIT, "LDAPUR_64BIT"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadRegisterLiteral, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadRegisterLiteral > >
        LoadRegisterLiteralStringMap { { {
            { LoadStoreGroup::LoadRegisterLiteral::LDR_32BIT_LITERAL, "LDR_32BIT_LITERAL"sv },
            { LoadStoreGroup::LoadRegisterLiteral::LDR_32BIT_LITERAL_SIMD, "LDR_32BIT_LITERAL_SIMD"sv },
            { LoadStoreGroup::LoadRegisterLiteral::LDR_64BIT_LITERAL, "LDR_64BIT_LITERAL"sv },
            { LoadStoreGroup::LoadRegisterLiteral::LDR_64BIT_LITERAL_SIMD, "LDR_64BIT_LITERAL_SIMD"sv },
            { LoadStoreGroup::LoadRegisterLiteral::LDRSW_LITERAL, "LDRSW_LITERAL"sv },
            { LoadStoreGroup::LoadRegisterLiteral::LDR_128BIT_LITERAL_SIMD, "LDR_128BIT_LITERAL_SIMD"sv },
            { LoadStoreGroup::LoadRegisterLiteral::PRFM_LITERAL, "PRFM_LITERAL"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreNoAllocatePairOffset, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreNoAllocatePairOffset > >
        LoadStoreNoAllocatePairOffsetStringMap { { {
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::STNP_32BIT, "STNP_32BIT"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::LDNP_32BIT, "LDNP_32BIT"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::STNP_32BIT_SIMD, "STNP_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::LDNP_32BIT_SIMD, "LDNP_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::STNP_64BIT_SIMD, "STNP_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::LDNP_64BIT_SIMD, "LDNP_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::STNP_64BIT, "STNP_64BIT"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::LDNP_64BIT, "LDNP_64BIT"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::STNP_128BIT_SIMD, "STNP_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreNoAllocatePairOffset::LDNP_128BIT_SIMD, "LDNP_128BIT_SIMD"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterPairPostIndexed, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterPairPostIndexed > >
        LoadStoreRegisterPairPostIndexedStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::STP_32BIT, "STP_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::LDP_32BIT, "LDP_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::STP_32BIT_SIMD, "STP_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::LDP_32BIT_SIMD, "LDP_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::STGP, "STGP"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::LDPSW, "LDPSW"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::STP_64BIT_SIMD, "STP_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::LDP_64BIT_SIMD, "LDP_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::STP_64BIT, "STP_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::LDP_64BIT, "LDP_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::STP_128BIT_SIMD, "STP_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPostIndexed::LDP_128BIT_SIMD, "LDP_128BIT_SIMD"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterPairOffset, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterPairOffset > >
        LoadStoreRegisterPairOffsetStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterPairOffset::STP_32BIT, "STP_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::LDP_32BIT, "LDP_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::STP_32BIT_SIMD, "STP_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::LDP_32BIT_SIMD, "LDP_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::STGP, "STGP"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::LDPSW, "LDPSW"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::STP_64BIT_SIMD, "STP_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::LDP_64BIT_SIMD, "LDP_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::STP_64BIT, "STP_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::LDP_64BIT, "LDP_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::STP_128BIT_SIMD, "STP_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairOffset::LDP_128BIT_SIMD, "LDP_128BIT_SIMD"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterPairPreIndexed, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterPairPreIndexed > >
        LoadStoreRegisterPairPreIndexedStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::STP_32BIT, "STP_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::LDP_32BIT, "LDP_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::STP_32BIT_SIMD, "STP_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::LDP_32BIT_SIMD, "LDP_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::STGP, "STGP"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::LDPSW, "LDPSW"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::STP_64BIT_SIMD, "STP_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::LDP_64BIT_SIMD, "LDP_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::STP_64BIT, "STP_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::LDP_64BIT, "LDP_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::STP_128BIT_SIMD, "STP_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterPairPreIndexed::LDP_128BIT_SIMD, "LDP_128BIT_SIMD"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterUnscaledImmediate, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterUnscaledImmediate > >
        LoadStoreRegisterUnscaledImmediateStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STURB, "STURB"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDURB, "LDURB"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDURSB_64BIT, "LDURSB_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDURSB_32BIT, "LDURSB_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STUR_8BIT_SIMD, "STUR_8BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDUR_8BIT_SIMD, "LDUR_8BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STUR_128BIT_SIMD, "STUR_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDUR_128BIT_SIMD, "LDUR_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STURH, "STURH"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDURH, "LDURH"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDURSH_64BIT, "LDURSH_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDURSH_32BIT, "LDURSH_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STUR_16BIT_SIMD, "STUR_16BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDUR_16BIT_SIMD, "LDUR_16BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STUR_32BIT, "STUR_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDUR_32BIT, "LDUR_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDURSW, "LDURSW"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STUR_32BIT_SIMD, "STUR_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDUR_32BIT_SIMD, "LDUR_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STUR_64BIT, "STUR_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDUR_64BIT, "LDUR_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::PRFUM, "PRFUM"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::STUR_64BIT_SIMD, "STUR_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnscaledImmediate::LDUR_64BIT_SIMD, "LDUR_64BIT_SIMD"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed > >
        LoadStoreRegisterImmediatePostIndexedStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRBi, "STRBi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRBi, "LDRBi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSBi_64BIT, "LDRSBi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSBi_32BIT, "LDRSBi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_8BIT_SIMD, "STRi_8BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_8BIT_SIMD, "LDRi_8BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_128BIT_SIMD, "STRi_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_128BIT_SIMD, "LDRi_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRHi, "STRHi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRHi, "LDRHi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSHi_64BIT, "LDRSHi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSHi_32BIT, "LDRSHi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_16BIT_SIMD, "STRi_16BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_16BIT_SIMD, "LDRi_16BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_32BIT, "STRi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_32BIT, "LDRi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSWi, "LDRSWi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_32BIT_SIMD, "STRi_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_32BIT_SIMD, "LDRi_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_64BIT, "STRi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_64BIT, "LDRi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_64BIT_SIMD, "STRi_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_64BIT_SIMD, "LDRi_64BIT_SIMD"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterUnprivileged, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterUnprivileged > >
        LoadStoreRegisterUnprivilegedStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::STTRB, "STTRB"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTRB, "LDTRB"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTRSB_64BIT, "LDTRSB_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTRSB_32BIT, "LDTRSB_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::STTRH, "STTRH"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTRH, "LDTRH"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTRSH_64BIT, "LDTRSH_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTRSH_32BIT, "LDTRSH_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::STTR_32BIT, "STTR_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTR_32BIT, "LDTR_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTRSW, "LDTRSW"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::STTR_64BIT, "STTR_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnprivileged::LDTR_64BIT, "LDTR_64BIT"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed > >
        LoadStoreRegisterImmediatePreIndexedStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRBi, "STRBi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRBi, "LDRBi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRSBi_64BIT, "LDRSBi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRSBi_32BIT, "LDRSBi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRi_8BIT_SIMD, "STRi_8BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRi_8BIT_SIMD, "LDRi_8BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRi_128BIT_SIMD, "STRi_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRi_128BIT_SIMD, "LDRi_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRHi, "STRHi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRHi, "LDRHi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRSHi_64BIT, "LDRSHi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRSHi_32BIT, "LDRSHi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRi_16BIT_SIMD, "STRi_16BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRi_16BIT_SIMD, "LDRi_16BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRi_32BIT, "STRi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRi_32BIT, "LDRi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRSWi, "LDRSWi"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRi_32BIT_SIMD, "STRi_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRi_32BIT_SIMD, "LDRi_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRi_64BIT, "STRi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRi_64BIT, "LDRi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::STRi_64BIT_SIMD, "STRi_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed::LDRi_64BIT_SIMD, "LDRi_64BIT_SIMD"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::AtomicMemoryOperation, std::string_view,
                                    enum_size_v< LoadStoreGroup::AtomicMemoryOperation > >
        AtomicMemoryOperationStringMap { { {
            { LoadStoreGroup::AtomicMemoryOperation::LDADDB, "LDADDB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRB, "LDCLRB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORB, "LDEORB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETB, "LDSETB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXB, "LDSMAXB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINB, "LDSMINB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXB, "LDUMAXB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINB, "LDUMINB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPB, "SWPB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDLB, "LDADDLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRLB, "LDCLRLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORLB, "LDEORLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETLB, "LDSETLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXLB, "LDSMAXLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINLB, "LDSMINLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXLB, "LDUMAXLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINLB, "LDUMINLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPLB, "SWPLB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDAB, "LDADDAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRAB, "LDCLRAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORAB, "LDEORAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETAB, "LDSETAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXAB, "LDSMAXAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINAB, "LDSMINAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXAB, "LDUMAXAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINAB, "LDUMINAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPAB, "SWPAB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDAPRB, "LDAPRB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDALB, "LDADDALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRALB, "LDCLRALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORALB, "LDEORALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETALB, "LDSETALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXALB, "LDSMAXALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINALB, "LDSMINALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXALB, "LDUMAXALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINALB, "LDUMINALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPALB, "SWPALB"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDH, "LDADDH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRH, "LDCLRH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORH, "LDEORH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETH, "LDSETH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXH, "LDSMAXH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINH, "LDSMINH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXH, "LDUMAXH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINH, "LDUMINH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPH, "SWPH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDLH, "LDADDLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRLH, "LDCLRLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORLH, "LDEORLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETLH, "LDSETLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXLH, "LDSMAXLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINLH, "LDSMINLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXLH, "LDUMAXLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINLH, "LDUMINLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPLH, "SWPLH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDAH, "LDADDAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRAH, "LDCLRAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORAH, "LDEORAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETAH, "LDSETAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXAH, "LDSMAXAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINAH, "LDSMINAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXAH, "LDUMAXAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINAH, "LDUMINAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPAH, "SWPAH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDAPRH, "LDAPRH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDALH, "LDADDALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRALH, "LDCLRALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORALH, "LDEORALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETALH, "LDSETALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXALH, "LDSMAXALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINALH, "LDSMINALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXALH, "LDUMAXALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINALH, "LDUMINALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPALH, "SWPALH"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADD_32BIT, "LDADD_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLR_32BIT, "LDCLR_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEOR_32BIT, "LDEOR_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSET_32BIT, "LDSET_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAX_32BIT, "LDSMAX_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMIN_32BIT, "LDSMIN_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAX_32BIT, "LDUMAX_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMIN_32BIT, "LDUMIN_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWP_32BIT, "SWP_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDL_32BIT, "LDADDL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRL_32BIT, "LDCLRL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORL_32BIT, "LDEORL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETL_32BIT, "LDSETL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXL_32BIT, "LDSMAXL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINL_32BIT, "LDSMINL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXL_32BIT, "LDUMAXL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINL_32BIT, "LDUMINL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPL_32BIT, "SWPL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDA_32BIT, "LDADDA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRA_32BIT, "LDCLRA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORA_32BIT, "LDEORA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETA_32BIT, "LDSETA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXA_32BIT, "LDSMAXA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINA_32BIT, "LDSMINA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXA_32BIT, "LDUMAXA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINA_32BIT, "LDUMINA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPA_32BIT, "SWPA_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDAPR_32BIT, "LDAPR_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDAL_32BIT, "LDADDAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRAL_32BIT, "LDCLRAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORAL_32BIT, "LDEORAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETAL_32BIT, "LDSETAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXAL_32BIT, "LDSMAXAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINAL_32BIT, "LDSMINAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXAL_32BIT, "LDUMAXAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINAL_32BIT, "LDUMINAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPAL_32BIT, "SWPAL_32BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADD_64BIT, "LDADD_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLR_64BIT, "LDCLR_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEOR_64BIT, "LDEOR_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSET_64BIT, "LDSET_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAX_64BIT, "LDSMAX_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMIN_64BIT, "LDSMIN_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAX_64BIT, "LDUMAX_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMIN_64BIT, "LDUMIN_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWP_64BIT, "SWP_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDL_64BIT, "LDADDL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRL_64BIT, "LDCLRL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORL_64BIT, "LDEORL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETL_64BIT, "LDSETL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXL_64BIT, "LDSMAXL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINL_64BIT, "LDSMINL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXL_64BIT, "LDUMAXL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINL_64BIT, "LDUMINL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPL_64BIT, "SWPL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDA_64BIT, "LDADDA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRA_64BIT, "LDCLRA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORA_64BIT, "LDEORA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETA_64BIT, "LDSETA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXA_64BIT, "LDSMAXA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINA_64BIT, "LDSMINA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXA_64BIT, "LDUMAXA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINA_64BIT, "LDUMINA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPA_64BIT, "SWPA_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDAPR_64BIT, "LDAPR_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDADDAL_64BIT, "LDADDAL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDCLRAL_64BIT, "LDCLRAL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDEORAL_64BIT, "LDEORAL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSETAL_64BIT, "LDSETAL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMAXAL_64BIT, "LDSMAXAL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDSMINAL_64BIT, "LDSMINAL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMAXAL_64BIT, "LDUMAXAL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::LDUMINAL_64BIT, "LDUMINAL_64BIT"sv },
            { LoadStoreGroup::AtomicMemoryOperation::SWPAL_64BIT, "SWPAL_64BIT"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterRegisterOffset, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterRegisterOffset > >
        LoadStoreRegisterRegisterOffsetStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRBr_EXTENDED, "STRBr_EXTENDED"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRBr_SHIFTED, "STRBr_SHIFTED"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRBr_EXTENDED, "LDRBr_EXTENDED"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRBr_SHIFTED, "LDRBr_SHIFTED"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSBr_64BIT_EXTENDED, "LDRSBr_64BIT_EXTENDED"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSBr_64BIT_SHIFTED, "LDRSBr_64BIT_SHIFTED"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSBr_32BIT_EXTENDED, "LDRSBr_32BIT_EXTENDED"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSBr_32BIT_SHIFTED, "LDRSBr_32BIT_SHIFTED"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRr_SIMD, "STRr_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRr_SIMD, "LDRr_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRHr, "STRHr"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRHr, "LDRHr"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSHr_64BIT, "LDRSHr_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSHr_32BIT, "LDRSHr_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRr_32BIT, "STRr_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRr_32BIT, "LDRr_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSWr, "LDRSWr"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRr_64BIT, "STRr_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRr_64BIT, "LDRr_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterRegisterOffset::PRFMr, "PRFMr"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterPAC, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterPAC > >
        LoadStoreRegisterPACStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterPAC::LDRAA_OFFSET, "LDRAA_OFFSET"sv },
            { LoadStoreGroup::LoadStoreRegisterPAC::LDRAA_PREINDEXED, "LDRAA_PREINDEXED"sv },
            { LoadStoreGroup::LoadStoreRegisterPAC::LDRAB_OFFSET, "LDRAB_OFFSET"sv },
            { LoadStoreGroup::LoadStoreRegisterPAC::LDRAB_PREINDEXED, "LDRAB_PREINDEXED"sv },
        } } };

    static constexpr SmallConstMap< LoadStoreGroup::LoadStoreRegisterUnsignedImmediate, std::string_view,
                                    enum_size_v< LoadStoreGroup::LoadStoreRegisterUnsignedImmediate > >
        LoadStoreRegisterUnsignedImmediateStringMap { { {
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRBi, "STRBi"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRBi, "LDRBi"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSBi_64BIT, "LDRSBi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSBi_32BIT, "LDRSBi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_8BIT_SIMD, "STRi_8BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_8BIT_SIMD, "LDRi_8BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_128BIT_SIMD, "STRi_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_128BIT_SIMD, "LDRi_128BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRHi, "STRHi"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRHi, "LDRHi"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSHi_64BIT, "LDRSHi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSHi_32BIT, "LDRSHi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_16BIT_SIMD, "STRi_16BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_16BIT_SIMD, "LDRi_16BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_32BIT, "STRi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_32BIT, "LDRi_32BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSWi, "LDRSWi"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_32BIT_SIMD, "STRi_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_32BIT_SIMD, "LDRi_32BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_64BIT, "STRi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_64BIT, "LDRi_64BIT"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::PRFMi, "PRFMi"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_64BIT_SIMD, "STRi_64BIT_SIMD"sv },
            { LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_64BIT_SIMD, "LDRi_64BIT_SIMD"sv },
        } } };

} // namespace

template <>
ARMEMU_API static std::string_view Enum::ToChar(A64LoadStoreGroup elem) {
    return A64LoadStoreGroupStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, A64LoadStoreGroup elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures elem) {
    return AdvancedSIMDLoadStoreMultipleStructuresStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view
    Enum::ToChar(LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed elem) {
    return AdvancedSIMDLoadStoreMultipleStructuresPostIndexedStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream&                                                      os,
                                    LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure elem) {
    return AdvancedSIMDLoadStoreSingleStructureStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed elem) {
    return AdvancedSIMDLoadStoreSingleStructurePostIndexedStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream&                                                   os,
                                    LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreMemoryTag elem) {
    return LoadStoreMemoryTagStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreMemoryTag elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreExclusive elem) {
    return LoadStoreExclusiveStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreExclusive elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LdaprStlrUnscaledImmediate elem) {
    return LdaprStlrUnscaledImmediateStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LdaprStlrUnscaledImmediate elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadRegisterLiteral elem) {
    return LoadRegisterLiteralStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadRegisterLiteral elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreNoAllocatePairOffset elem) {
    return LoadStoreNoAllocatePairOffsetStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreNoAllocatePairOffset elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterPairPostIndexed elem) {
    return LoadStoreRegisterPairPostIndexedStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterPairPostIndexed elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterPairOffset elem) {
    return LoadStoreRegisterPairOffsetStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterPairOffset elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterPairPreIndexed elem) {
    return LoadStoreRegisterPairPreIndexedStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterPairPreIndexed elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterUnscaledImmediate elem) {
    return LoadStoreRegisterUnscaledImmediateStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterUnscaledImmediate elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed elem) {
    return LoadStoreRegisterImmediatePostIndexedStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterUnprivileged elem) {
    return LoadStoreRegisterUnprivilegedStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterUnprivileged elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed elem) {
    return LoadStoreRegisterImmediatePreIndexedStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::AtomicMemoryOperation elem) {
    return AtomicMemoryOperationStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::AtomicMemoryOperation elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterRegisterOffset elem) {
    return LoadStoreRegisterRegisterOffsetStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterRegisterOffset elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterPAC elem) {
    return LoadStoreRegisterPACStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterPAC elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
ARMEMU_API static std::string_view Enum::ToChar(LoadStoreGroup::LoadStoreRegisterUnsignedImmediate elem) {
    return LoadStoreRegisterUnsignedImmediateStringMap.At(elem);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LoadStoreGroup::LoadStoreRegisterUnsignedImmediate elem) {
    os << Enum::ToChar(elem);
    return os;
}

END_NAMESPACE
