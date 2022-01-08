import argparse

parser = argparse.ArgumentParser(description='Header file to use.')

parser.add_argument("-f", "--file", help="Header file to use.", type=str, required=True)
parser.add_argument("-t", "--target", help="Source file to write.", type=str, required=True)

args = parser.parse_args()

header_prelog = "\n#include <ProcessingUnit/A64InstructionSet/A64InstructionSet.h>\n#include <Utility/StreamableEnum.h>\n#include <ostream>\n#include <string_view>\n\nBEGIN_NAMESPACE\n\n"
header_epilog = "END_NAMESPACE\n"
map_prelog = "\nstatic constexpr SmallConstMap< " 
map_name_epilog = "StringMap { { {"
map_epilog = "} } };\n"

ToChar_func_prelog = "template <>\nstatic std::string_view Enum::ToChar("
ToChar_func = " elem) { \n return "
ToChar_func_epilog = "StringMap.At(elem);\n}\n"

stream_func_prelog = "template<>\nstd::ostream& operator<<(std::ostream&os, "
stream_func_epilog = " elem) {\nos << Enum::ToChar(elem);\nreturn os;\n}"

enums = dict()
first_enum_name = ""

with open(args.file, "r") as f:
    lines = f.readlines()

    first_enum = True
    enum_found = False
    enum_just_found = False

    current_enum = ""

    for line in lines:
        if(enum_found):
            line = line.replace(" ", "")
            line = line.replace(",", "")
            line = line.replace("\n", "")

            if "//" in line:
                loc = line.find("//")
                line = line[0:loc]

            if line == "ARM_EMU_COUNT":
                enum_found = False
                continue
            else:
                if(len(line) != 0):
                    enums[current_enum].append(line)

        if(enum_just_found):
            enum_found = True
            enum_just_found = False
            continue

        if "enum class" in line and (not enum_found or not enum_just_found):
            enum_just_found = True
            line = line.replace(" ", "")
            line = line.removeprefix("enumclass")
            line = line.replace(":std::uint32_t\n", "")
            enums[line] = list()
            current_enum = line
            if(first_enum):
                first_enum_name = line
                first_enum_name = first_enum_name.replace("A64", "")
                first_enum = False

with open(args.target, "w") as f:
    f.write(header_prelog)
    f.write("namespace {\n\n")
    f.write("using namespace std::literals::string_view_literals;\n")

    for key, values in enums.items():
        f.write(map_prelog)
        tmp_key = key.replace("A64", "")

        if(first_enum_name != tmp_key):
            f.write(first_enum_name)
            f.write("::")
        f.write(key)

        f.write(", std::string_view, enum_size_v< ")
        if(first_enum_name != tmp_key):
            f.write(first_enum_name)
            f.write("::")
        f.write(key)

        f.write(" > >")
        f.write(key)
        f.write(map_name_epilog)
        f.write("\n")
        for value in values:
            f.write("{ ")
            
            if(first_enum_name != tmp_key):
                f.write(first_enum_name)
                f.write("::")
            f.write(key)
            f.write("::")
            f.write(value)
            f.write(", \"")
            f.write(value)
            f.write("\"sv }, ")
        f.write(map_epilog)
    f.write("\n}\n\n")

    for key, _ in enums.items():
        f.write(ToChar_func_prelog)

        tmp_key = key.replace("A64", "")
        if(first_enum_name != tmp_key):
            f.write(first_enum_name)
            f.write("::")

        f.write(key)
        f.write(ToChar_func)
        f.write(key)
        f.write(ToChar_func_epilog)
        f.write("\n")

        f.write(stream_func_prelog)        
        tmp_key = key.replace("A64", "")
        if(first_enum_name != tmp_key):
            f.write(first_enum_name)
            f.write("::")
        f.write(key)
        f.write(stream_func_epilog)
        f.write("\n")
        f.write("\n")
    
    f.write(header_epilog)

