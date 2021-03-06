#
# values.pl
#
# Usage:
#   perl values.pl inputfile1.h inputfile2.h ...
#
# The input files must be in a uniform format. Output is to stdout, so you'll
# probably want to redirect it to a file.
#
# typedef A B;
#
#   This adds B to the "valid types" array, assuming A was a valid type
#
# const type1 name.*
#
#   This adds the value of type1 to the output file.
#
# template<typename A, ...>
# struct/class name
# {
#   typedef A B;
#
#   const type1 name1*
#   type2 name2;
#   type3 name3;
# };
#
#   A, the template parameter, is considered a valid type while parsing this
#   definition. We would pick up the typedef as well, and also consider B a 
#   valid type. 
#
#   We would pick up the size of the class, the offset of name2 and name3,
#   and the value of name1 in the output file.
#
# enum enumname
# {
#   name1.*
#   name2.*
# };
#
#   This adds name1 and it's value as well as name2 and it's value to the 
#   output file.
#
# It's important to note that the end of any class, struct, or enum definition
# must be a "};" on a single line.

use strict;

# This hash grows with global typedefs
my %valid_types = (
    'char' => undef,
    'unsigned char' => undef,
    'bool' => undef,
    'short' => undef,
    'unsigned short' => undef,
    'int' => undef,
    'unsigned int' => undef,
    'long' => undef,
    'unsigned long' => undef,
    'long long' => undef,
    'unsigned long long' => undef,
    'float' => undef,
    'double' => undef,
    'long double' => undef,
    'wchar_t' => undef,
    'size_t' => undef,
    'boost::uint32_t' => undef,
    'boost::uint32_t' => undef,
    'boost::int32_t' => undef,
    'boost::uint64_t' => undef,
    'boost::int64_t' => undef,
    'boost::uint8_t' => undef,
    'boost::uint16_t' => undef,
);
my %valid_types_local = ();

# output the header
print "//\n";
print "// This file is automatically generated, do not hand edit!\n";
print "//\n";
print "\n";
foreach (0 ... $#ARGV) {
    print "#include \"$ARGV[$_]\"\n";
};
print "\n";
print "using namespace fairport;\n";
print "using namespace fairport::disk;\n";
print "\n";
print "namespace fairport {\n";
print "\n";
print "struct pstvalue\n";
print "{\n";
print "    const wchar_t* name;\n";
print "    int v;\n";
print "};\n";
print "\n";
print "const pstvalue values[] = {\n";

# further lines are of the format
# { L"name", value },\n
# ... then the ending trailer.

my $struct_name;
my $template_param;
my $non_trivial;
my $param_name;

my $varregex = "[A-Za-z_][A-Za-z_0-9]*";

sub clear_state() {
    $struct_name = undef;
    $template_param = undef;
    $non_trivial = undef;
    $param_name = undef;
}

foreach (0 ... $#ARGV) {
    open inputfile, "<", $ARGV[$_];

    # This loop is at the global scope

    while(<inputfile>) {
        my $valid_types_regex = '(' . join('|',keys %valid_types) . ')';

        # skip all comments
        if(m#^//#) {
            clear_state();
            next;
        }

        # global typedef, add to our array
        if(/typedef\s$valid_types_regex\s($varregex);/) {
            if(exists $valid_types{$1}) {
                $valid_types{$2} = undef;
                print "\t{ L\"sizeof($2)\", sizeof($2) },\n";
            }
        }

        # global const variable, print it out
        elsif(/const\s$valid_types_regex ($varregex) =/) {
            # hack: Ignore guids
            if($1 ne "guid") {
                print "\t{ L\"$2\", $2 },\n";
            }
        }

        # template definition
        elsif(/template<typename ($varregex)>/) {
            $template_param = $1;
            if($template_param ne "T") {
                while(<inputfile>) {
                    if(/^}.*;/) {
                        last;
                    }
                }
                $template_param = undef;
            }
            $valid_types_local{"$template_param"} = undef;
        }

        # skip templates of 2 params 
        elsif(/template<typename (\w*), typename ($varregex)>/) {
            while(<inputfile>) {
                if(/^}.*;/) {
                    last;
                }
            }
        }

        # a specialized class
        if(/(class|struct) ($varregex)<($varregex)>/) {
            $struct_name = $2;
            $param_name = $3;
            $valid_types{"$struct_name<$param_name>"} = undef;
            $valid_types_local{"$param_name"} = undef;
       }
       # non specialized class
       elsif(/(class|struct) ($varregex)/) {
            $struct_name = $2;
            if(defined($template_param)) {
                $valid_types{"$struct_name<ulonglong>"} = undef;
                $valid_types{"$struct_name<ulong>"} = undef;
                $valid_types{"$struct_name<T>"} = undef;
            } else {
                $valid_types{$struct_name} = undef;
            }
        }

        # process a struct/class definition
        if($struct_name) {

            if(defined($template_param)) {
                print "\n\t// struct $struct_name<$template_param>\n";
            }   
            elsif(defined($param_name)) {
                print "\n\t// struct $struct_name<$param_name>\n";
            }
            else {
                print "\n\t// struct $struct_name\n";
            }
            $non_trivial = 0;

            # eat the { on the next line
            my $line = <inputfile>;

            # process the body of the struct
            while(<inputfile>) {

                # regenerate valid types regex, considering local types
                if(keys %valid_types_local) {
                    $valid_types_regex = '(' . join('|',keys %valid_types) . '|' . join('|',keys %valid_types_local) . ')';
                } else {
                    $valid_types_regex = '(' . join('|',keys %valid_types) . ')';
                }

                # typedef, update valid types (local)
                if(/typedef\s$valid_types_regex ($varregex);/) {
                    $valid_types_local{"$2"} = undef;
                }

                # class static const variable, print it out
                elsif(/static const\s$valid_types_regex ($varregex) =/) {
                    if(defined($template_param)) {
                        print "\t{ L\"$struct_name<ulonglong>\::$2\", $struct_name<ulonglong>\::$2 },\n";
                        print "\t{ L\"$struct_name<ulong>\::$2\", $struct_name<ulong>\::$2 },\n";
                    }
                    elsif(defined($param_name)) {
                        print "\t{ L\"$struct_name<$param_name>\::$2\", $struct_name<$param_name>\::$2 },\n";
                    }
                    else {
                        print "\t{ L\"$struct_name\::$2\", $struct_name\::$2 },\n";
                    }
                }

                # data member, print offset
                elsif(/$valid_types_regex ($varregex);/) {
                    if(defined($template_param)) {
                        print "\t{ L\"offsetof($struct_name<ulonglong>, $2)\", offsetof($struct_name<ulonglong>, $2) },\n";
                        print "\t{ L\"offsetof($struct_name<ulong>, $2)\", offsetof($struct_name<ulong>, $2) },\n";
                    }
                    elsif(defined($param_name)) {
                        print "\t{ L\"offsetof($struct_name<$param_name>, $2)\", offsetof($struct_name<$param_name>, $2) },\n";
                    }
                    else {
                        print "\t{ L\"offsetof($struct_name, $2)\", offsetof($struct_name, $2) },\n";
                    }
                    $non_trivial = 1;
                }

                # break on "};", ending the struct
                elsif(/^}.*;/) {
                    last;
                }
            }

            # struct done, print the size
            if($non_trivial == 1) {
                if(defined($template_param)) {
                    print "\t{ L\"sizeof($struct_name<ulonglong>)\", sizeof($struct_name<ulonglong>) },\n";
                    print "\t{ L\"sizeof($struct_name<ulong>)\", sizeof($struct_name<ulong>) },\n";
                }
                elsif(defined($param_name)) {
                    print "\t{ L\"sizeof($struct_name<$param_name>)\", sizeof($struct_name<$param_name>) },\n";
                }
                else {
                    print "\t{ L\"sizeof($struct_name)\", sizeof($struct_name) },\n";
                }
            }
            clear_state();
            for(keys %valid_types_local) {
                delete $valid_types_local{$_};
            }
        }

        # enum
        if(/enum ($varregex)/) {
            print "\n\t// enum $1\n";
            # eat the { on the next line
            my $line = <inputfile>; 
            while(<inputfile>) {

                # enum value
                if(/($varregex).*,/) {
                    print "\t{ L\"$1\", $1 },\n";
                }

                # break on "};", ending the enum
                if(/^}.*;/) {
                    last;
                }
            }
            clear_state();
        }
    }

};

print "};\n";
print "\n";
print "}\n";
