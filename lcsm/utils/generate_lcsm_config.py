import argparse

def __t_or_f(s: str, flag_name: str) -> bool:
    s = s.lower()
    if s == 'true': return True
    elif s == 'false': return False
    else:
        print(f"usage: --{flag_name} [true/false]")
        exit(1)

def __main():
    parser = argparse.ArgumentParser()

    parser.add_argument("--system-name", type = str, required = True)
    parser.add_argument("--compiler-id", type = str, required = True)
    parser.add_argument("--shared-libs", type = str, required = True)
    parser.add_argument("--output-file", type = str, required = True)

    args = vars(parser.parse_args())

    system_name = str(args["system_name"])
    compiler_id = str(args["compiler_id"])
    shared_libs = __t_or_f(str(args["shared_libs"]), "shared-libs")
    output_file = str(args["output_file"])

    stream = open(output_file, "w")

    header_prefix = """
#ifndef LCSM_LCSMCONFIG_H
#define LCSM_LCSMCONFIG_H
"""
    header_suffix = """
#endif /* LCSM_LCSMCONFIG_H */
"""

    stream.write(header_prefix)

    if compiler_id == "MSVC":
        pragma = """
#pragma warning(disable : 4251)
"""
        stream.write(pragma)

    defines = """
#define LCSM_API
"""
    if system_name == "Windows" and shared_libs:
        defines = """
#ifdef LCSM_EXPORTS
#define LCSM_API __declspec(dllexport)
#else
#define LCSM_API __declspec(dllimport)
#endif
"""
    stream.write(defines)

    stream.write(header_suffix)

if __name__ == "__main__":
    __main()
