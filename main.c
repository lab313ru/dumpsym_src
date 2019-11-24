#include <stdio.h>
#include <string.h>

unsigned char byte_310[] =
{
  0x6B, 0x6A, 0x69, 0x68, 0x67, 0x66, 0x65, 0x13, 0x12, 0x11,
  0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07,
  0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
};

void parse_class(int a1)
{
    unsigned int v1; // edx
    int v2; // ecx
    char* v3; // edi
    int v4; // zf

    v1 = a1;
    printf("class ");
    if (v1 + 1 > 0x6B)
    {
    LABEL_34:
        printf("?%d? ", v1);
    }
    else
    {
        v2 = 28;
        v3 = byte_310;
        do
        {
            if (!v2)
                break;
            v4 = *v3++ == (char)(v1 + 1);
            --v2;
        } while (!v4);
        switch (v2)
        {
        case 0:
            goto LABEL_34;
        case 1:
            printf("EFCN ");
            break;
        case 2:
            printf("NULL ");
            break;
        case 3:
            printf("AUTO ");
            break;
        case 4:
            printf("EXT ");
            break;
        case 5:
            printf("STAT ");
            break;
        case 6:
            printf("REG ");
            break;
        case 7:
            printf("EXTDEF ");
            break;
        case 8:
            printf("LABEL ");
            break;
        case 9:
            printf("ULABEL ");
            break;
        case 10:
            printf("MOS ");
            break;
        case 11:
            printf("ARG ");
            break;
        case 12:
            printf("STRTAG ");
            break;
        case 13:
            printf("MOU ");
            break;
        case 14:
            printf("UNTAG ");
            break;
        case 15:
            printf("TPDEF ");
            break;
        case 16:
            printf("USTATIC ");
            break;
        case 17:
            printf("ENTAG ");
            break;
        case 18:
            printf("MOE ");
            break;
        case 19:
            printf("REGPARM ");
            break;
        case 20:
            printf("FIELD ");
            break;
        case 21:
            printf("BLOCK ");
            break;
        case 22:
            printf("FCN ");
            break;
        case 23:
            printf("EOS ");
            break;
        case 24:
            printf("FILE ");
            break;
        case 25:
            printf("LINE ");
            break;
        case 26:
            printf("ALIAS ");
            break;
        case 27:
            printf("HIDDEN ");
            break;
        }
    }
}

void parse_type(int a1)
{
    int v1; // edx
    unsigned int v2; // eax

    v1 = a1;
    printf("type ");
    while (v1 & 0xFFF0)
    {
        v2 = (v1 >> 4) & 3;
        if (v2 < 2)
        {
            if (v2 == 1)
                printf("PTR ");
        }
        else if (v2 <= 2)
        {
            printf("FCN ");
        }
        else
        {
            printf("ARY ");
        }
        v1 = ((v1 >> 2) & 0xFFF0) + (v1 & 0xF);
    }
    switch (v1)
    {
    case 0:
        printf("NULL ");
        break;
    case 1:
        printf("VOID ");
        break;
    case 2:
        printf("CHAR ");
        break;
    case 3:
        printf("SHORT ");
        break;
    case 4:
        printf("INT ");
        break;
    case 5:
        printf("LONG ");
        break;
    case 6:
        printf("FLOAT ");
        break;
    case 7:
        printf("DOUBLE ");
        break;
    case 8:
        printf("STRUCT ");
        break;
    case 9:
        printf("UNION ");
        break;
    case 10:
        printf("ENUM ");
        break;
    case 11:
        printf("MOE ");
        break;
    case 12:
        printf("UCHAR ");
        break;
    case 13:
        printf("USHORT ");
        break;
    case 14:
        printf("UINT ");
        break;
    case 15:
        printf("ULONG ");
        break;
    default:
        return;
    }
}

void read_string(FILE* input) {
    unsigned char count = fgetc(input);

    for (int i = 0; i < count; ++i) {
        printf("%c", fgetc(input));
    }
}

unsigned int read_word(FILE* a1)
{
    int v2 = fgetc(a1);
    return v2 + (fgetc(a1) << 8);
}

unsigned int read_dword(FILE* a1)
{
    int v2 = fgetc(a1);
    int v3 = (fgetc(a1) << 8) + v2;
    int v4 = (fgetc(a1) << 16) + v3;
    return v4 + (fgetc(a1) << 24);
}

void show_help()
{
    printf("dumpsym 2.02 (c) 1997 SN Systems Software Ltd\n");
    printf("Usage: dumpsym sym_file\n");
}

int main(int argc, const char** argv, const char** envp)
{
    char header[4];

    int line_num = 0;
    if (argc != 2)
    {
        show_help();
        return 1;
    }
    FILE* input = fopen(argv[1], "rb");
    if (!input)
    {
        printf("Error: Can't open file '%s' for input\n", argv[1]);
        return 1;
    }
    header[0] = fgetc(input);
    header[1] = fgetc(input);
    header[2] = fgetc(input);
    header[3] = 0;
    if (strncmp(header, "MND", 3))
    {
        printf("Error: File '%s' is not in SN-SYM format\n", argv[1]);
        fclose(input);
        return 1;
    }
    printf("\nHeader : %s", header);
    int version = fgetc(input);
    printf(" version %d\n", version);
    int target_unit = fgetc(input);
    printf("Target unit %d\n", target_unit);
    fgetc(input);
    fgetc(input);
    fgetc(input);
    while (1)
    {
        int v8 = ftell(input);
        printf("%06lx: ", v8);
        unsigned int pos = read_dword(input);
        unsigned int tag = fgetc(input);

        int do_break = 0;

        switch (tag) {
        case EOF: do_break = 1; break;
        case 8: {
            printf("$%08lx %x MX-info %x\n", pos, tag, (unsigned char)fgetc(input));
        } break;
        default: {
            if (tag <= 0x7F)
            {
                printf("$%08lx %x ", pos, tag);
                read_string(input);
                printf("\n");
            }
            else {
                switch (tag) {
                case 0x80: {
                    unsigned int v73 = ++line_num;
                    printf("$%08lx %x Inc SLD linenum (to %ld)\n", pos, 0x80, v73);
                } break;
                case 0x82: {
                    unsigned char v13 = (unsigned char)fgetc(input);
                    line_num += v13;
                    printf("$%08lx %x Inc SLD linenum by byte %u (to %ld)\n", pos, tag, v13, line_num);
                } break;
                case 0x84: {
                    unsigned int v14 = read_word(input);
                    line_num += v14;
                    printf("$%08lx %x Inc SLD linenum by word %u (to %ld)\n", pos, tag, v14, line_num);
                } break;
                case 0x86: {
                    unsigned int v15 = read_dword(input);
                    line_num = v15;
                    printf("$%08lx %x Set SLD linenum to %lu\n", pos, tag, v15);
                } break;
                case 0x88: {
                    line_num = read_dword(input);
                    printf("$%08lx %x Set SLD to line %lu of file ", pos, tag, line_num);
                    read_string(input);
                    printf("\n");
                } break;
                case 0x8A: {
                    printf("$%08lx %x End SLD info\n", pos, tag);
                } break;
                case 0x8C: {
                    printf("$%08lx %x Function start\n", pos, 0x8C);
                    printf("    fp = %d\n", read_word(input));
                    printf("    fsize = %ld\n", read_dword(input));
                    printf("    retreg = %d\n", read_word(input));
                    printf("    mask = $%08lx\n", read_dword(input));
                    printf("    maskoffs = %ld\n", read_dword(input));
                    printf("    line = %ld\n", read_dword(input));
                    printf("    file = ");
                    read_string(input);
                    printf("\n    name = ");
                    read_string(input);
                    printf("\n");
                } break;
                case 0x8E: {
                    printf("$%08lx %x Function end   line %ld\n", pos, tag, read_dword(input));
                } break;
                case 0x90: {
                    printf("$%08lx %x Block start  line = %ld\n", pos, tag, read_dword(input));
                } break;
                case 0x92: {
                    printf("$%08lx %x Block end  line = %ld\n", pos, tag, read_dword(input));
                } break;
                case 0x94: {
                    printf("$%08lx %x Def ", pos, 0x94);
                    parse_class(read_word(input));
                    parse_type(read_word(input));
                    printf("size %ld ", read_dword(input));
                    printf("name ");
                    read_string(input);
                    printf("\n");
                } break;
                case 0x96: {
                    printf("$%08lx %x Def2 ", pos, tag);
                    parse_class(read_word(input));
                    parse_type(read_word(input));
                    printf("size %ld ", read_dword(input));
                    unsigned int v40 = read_word(input);
                    printf("dims %ld ", v40);
                    while (--v40 != -1)
                    {
                        printf("%ld ", read_dword(input));
                    }
                    printf("tag ");
                    read_string(input);
                    printf(" name ");
                    read_string(input);
                    printf("\n");
                } break;
                case 0x98: {
                    unsigned int length = read_dword(input);
                    unsigned int id = read_dword(input);
                    printf("$%08lx overlay length $%08lx id $%lx\n", pos, length, id);
                } break;
                case 0x9A: {
                    printf("$%08lx set overlay\n", pos);
                } break;
                case 0x9C: {
                    printf("$%08lx %x Function2 start\n", pos, tag);
                    printf("    fp = %d\n", read_word(input));
                    printf("    fsize = %ld\n", read_dword(input));
                    printf("    retreg = %d\n", read_word(input));
                    printf("    mask = $%08lx\n", read_dword(input));
                    printf("    maskoffs = %ld\n", read_dword(input));
                    printf("    fmask = $%08lx\n", read_dword(input));
                    printf("    fmaskoffs = %ld\n", read_dword(input));
                    printf("    line = %ld\n", read_dword(input));
                    printf("    file = ");
                    read_string(input);
                    printf("\n    name = ");
                    read_string(input);
                    printf("\n");
                } break;
                case 0x9E: {
                    printf("$%08lx %x Mangled name \"", pos, 0x9E);
                    read_string(input);
                    printf("\" is \"");
                    read_string(input);
                    printf("\n");
                } break;
                default: {
                    printf("??? %d ???\n\n", tag);
                } break;
                }
            }
        } break;
        }

        if (do_break) {
            break;
        }
    }
    fclose(input);
}
