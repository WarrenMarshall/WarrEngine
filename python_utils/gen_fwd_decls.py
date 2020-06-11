
import os

def Main():

    print( "Generating forward declarations..." )

    struct_name_set = set()
    num_structs_found = 0

    # loop through every ".h" file in the current directory

    print( "Scanning .h files..." )
    for filename in os.listdir( "." ):

        if filename.endswith(".h"):

            if filename != "master_fwd_decl.h":

                with open( filename, "rt" ) as fp:

                    # for every line that begins with the word "struct", parse out the
                    # struct name and add it to the list

                    for cnt, line in enumerate( fp ):

                        if line.startswith("struct "):

                            pos = line.find(" : ")
                            line = line[7:pos]

                            if line.endswith(";"):
                                line = line[:-1]

                            num_structs_found += 1
                            struct_name_set.add( line )

    # write out a new forward declarations file for this project

    print( "Found " + str(num_structs_found) + " structs!" );
    print( "Writing output file..." )
    with open( "master_fwd_decl.h", "wt" ) as fp:

        fp.write( "#pragma once\n" )
        fp.write( "\n" )
        fp.write( "// NOTE: file generated automatically by running\n" )
        fp.write( "// gen_fwd_decls.bat in the project directory.\n" )
        fp.write( "//\n" )
        fp.write( "// DO NOT EDIT MANUALLY\n" )
        fp.write( "\n" )

        for struct_name in sorted( struct_name_set ):

            fp.write( "struct " + struct_name + ";\n" )

    print( "Done!" )


# ----------------------------------------------------------------------

if __name__ == "__main__":
    Main()
