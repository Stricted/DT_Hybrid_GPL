
struct stringpool_t
{
    char stringpool_str1[sizeof("ASCII")];
    char stringpool_str2[sizeof("UTF-8")];
    char stringpool_str3[sizeof("ISO-8859-1")];
    char stringpool_str4[sizeof("GB2312")];    
    char stringpool_str5[sizeof("GBK")];
    char stringpool_str6[sizeof("BIG5")];
    char stringpool_str7[sizeof("CP936")];    
    char stringpool_str8[sizeof("UTF-16LE")];    
    char stringpool_str9[sizeof("UCS-2LE")];    
    char stringpool_str10[sizeof("UTF-16BE")];    
    char stringpool_str11[sizeof("UCS-2BE")];    
    char stringpool_str12[sizeof("IBM850")];    
};

static const struct stringpool_t stringpool_contents =
{
    "ASCII",
    "UTF-8",
    "ISO-8859-1",
    "GB2312",
    "GBK",
    "BIG5",
    "CP936",
    "UTF-16LE",
    "UCS-2LE",
    "UTF-16BE",
    "UCS-2BE",
    "IBM850",
};

#define stringpool ((const char *) &stringpool_contents)

struct alias { int name; unsigned int encoding_index; };

static const struct alias aliases[] =
{
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str1, ei_ascii},
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str2, ei_utf8},
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str3, ei_iso8859_1},
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str4, ei_gb2312},
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str5, ei_ces_gbk},
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str6, ei_ces_big5},
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str7, ei_cp936},    
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str8, ei_utf16le},    
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str9, ei_ucs2le},    
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str10, ei_utf16be},
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str11, ei_ucs2be},
    {(int)(long)&((struct stringpool_t *)0)->stringpool_str12, ei_cp850},

};

#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 45

const struct alias *aliases_lookup (register const char *str, register unsigned int len)
{
    int i;

    if (len > MAX_WORD_LENGTH || len < MIN_WORD_LENGTH)
    {
        return 0;
    }

    for (i = 0; i < sizeof(aliases)/sizeof(aliases[0]); i++)
    {
        register int o = aliases[i].name;
        register const char *s = o + stringpool;
        if (0 == memcmp(str, s, len))
        {
            return &aliases[i];
        }
    }

    return 0;
}

