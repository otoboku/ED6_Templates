//--------------------------------------
//--- 010 Editor v4.0.3 Binary Template
//
// File:
// Author:
// Revision: 2015.09.04
// Purpose:
//--------------------------------------
#ifndef _ED6_common_
#define _ED6_common_

#include "010_def.bt.h"

#ifdef ED6_FC
#undef ED6_FC
#undef ED6_SC
#undef ED6_3RD
#undef ED_ZERO
#undef ED_AO
#define ED6_FC
#define ED_VERSION
#define ED6_ALL
#define ED6_FC_SC
#endif

#ifdef ED6_SC
#undef ED6_FC
#undef ED6_SC
#undef ED6_3RD
#undef ED_ZERO
#undef ED_AO
#define ED6_SC
#define ED_VERSION
#define ED6_ALL
#define ED6_FC_SC
#define ED6_SC_3RD
#endif

#ifdef ED6_3RD
#undef ED6_FC
#undef ED6_SC
#undef ED6_3RD
#undef ED_ZERO
#undef ED_AO
#define ED6_3RD
#define ED_VERSION
#define ED6_ALL
#define ED6_SC_3RD
#endif

#ifdef ED_ZERO
#undef ED6_FC
#undef ED6_SC
#undef ED6_3RD
#undef ED_ZERO
#undef ED_AO
#define ED_ZERO
#define ED_VERSION
#define ED_ZERO_AO
#endif

#ifdef ED_AO
#undef ED6_FC
#undef ED6_SC
#undef ED6_3RD
#undef ED_ZERO
#undef ED_AO
#define ED_AO
#define ED_VERSION
#define ED_ZERO_AO
#endif

#ifndef ED_VERSION
#warning No game version is defined before including "ED6_common.bt.h"
Printf("\n");
#endif


#ifdef ED_AO
    local const UINT    CRAFT_INFO_SIZE = 28;
#else
    local const UINT    CRAFT_INFO_SIZE = 32;
#endif

#ifdef ED6_FC
    local const UINT    SLOT_COUNT = 6;
#else
    local const UINT    SLOT_COUNT = 7;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
typedef union
{
    ushort value;
    struct _flag
    {
        ushort bit  :3;
        ushort index:13;
    } flag;
} TypeFlag <read=readTypeFlag, write=writeTypeFlag, optimize=true>;
void writeTypeFlag(TypeFlag &a, string s) {;}

string readTypeFlag(TypeFlag &a)
{
    string s;
    SPrintf(s, "%X %d %d", a.value, a.flag.index, a.flag.bit);
    return s;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//if (ED6_NO & (ED6_FC | ED6_SC | ED6_3RD))
#ifdef ED6_ALL
    typedef struct  //DAT DIR文件索引
    {
        HEX32   file;
        FSkip(-sizeof(file));
        ushort  Index;
        ushort  DatNo <format=hex>;

        //local string    _str_;
        //SPrintf(_str_, "DT%02X  %04d", DatNo, Index);
        //SPrintf(_str_, "%#x", file);
    } FileIndex <read=readFileIndex, write=writeFileIndex, open=suppress, optimize=true>;

    string readFileIndex(FileIndex &a)
    {
        string  str;
        SPrintf(str, "%#x", a.file);
        return  str;
    }

    void writeFileIndex(FileIndex &a, string s)
    {
        SScanf(s, "%x", a.file);
    }
#else
    typedef struct
    {
        HEX32   file;
        FSkip(-sizeof(file));
        HEX32   Index    :20;
        //HEX32   FileType :12;
        enum <HEX32>
        {
            ms          = 0x300,
            as          = 0x301,
            bs          = 0x302,
            sy          = 0x310,
            chr_ch      = 0x7,
            apl_ch      = 0x8,
            monster_ch  = 0x9,
        } FileType :12 <format=hex>;
    //  300:MS  data\battle\dat\ms12345.dat
    //  301:AS  data\battle\dat\as12345.dat
    //  302:BS  data\battle\dat\bs12345.dat
    //  310:SY  data\battle\symbol\sy12345.itp (SymbolTexture AT条头像)
    //  007:CH  data\chr\ch12345.itc    //人物素材，如在队时 站立移动时的形象
    //  008:CH  data\apl\ch12345.itc
    //  009:CH  data\monster\ch12345.itc//怪物素材
    } FileIndex <read=readFileIndex, write=writeFileIndex, open=suppress, optimize=true>;
    local uint MS = 0;

    string readFileIndex(FileIndex &a)
    {
        local string    format, str;
        switch (a.FileType)
        {
            case 0x300: format = "ms%05x.dat";          break;
            case 0x301: format = "as%05x.dat";          break;
            case 0x302: format = "bs%05x.dat";          break;
            case 0x310: format = "sy%05x.itp";          break;
            case 0x007: format = "chr\\ch%05x.itc";     break;
            case 0x008: format = "apl\\ch%05x.itc";     break;
            case 0x007: format = "monster\\ch%05x.itc"; break;
            default:
                SPrintf(str, "%#x", a.file);
                return  str;
        }
        SPrintf(str, format, a.Index);
        return  str;
    }

    void writeFileIndex(FileIndex &a, string s)
    {
        //SScanf(s, "%x", a.file);
    }
    /*
    typedef int FileIndex <format=hex, read=readFileIndex, write=writeFileIndex>;

    string readFileIndex(FileIndex &a)
    {
        string  str;
        SPrintf(str, "%#x", a);
        return  str;
    }

    void writeFileIndex(FileIndex &a, string s)
    {
        SScanf(s, "%x", a);
    }*/
#endif
#define ReadFileIndex readFileIndex

////////////////////////////////////////////////////////////////////////////////////////////////////
//  RESISTANCE/CONDITION
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef union  //抗性
{
    int condition <format = hex>;
    struct _condition_bits
    {
        ULONG   POISON              : 1 <name="　毒">       ;   // 0x00000001;  // 毒
        ULONG   FREEZE              : 1 <name="冻结">       ;   // 0x00000002;  // 冻结 FROZEN
        ULONG   PETRIFY             : 1 <name="石化">       ;   // 0x00000004;  // 石化 LANDIFICATION STONE
        ULONG   SLEEP               : 1 <name="睡眠">       ;   // 0x00000008;  // 睡眠 SLEEPING
        ULONG   DISABLE_ARTS        : 1 <name="封魔">       ;   // 0x00000010;  // 封魔 MUTE
        ULONG   BLIND               : 1 <name="黑暗">       ;   // 0x00000020;  // 黑暗 DARKNESS
        ULONG   DISABLE_CRAFT       : 1 <name="封技">       ;   // 0x00000040;  // 封技 SEAL
        ULONG   CONFUSE             : 1 <name="混乱">       ;   // 0x00000080;  // 混乱 CHAOS
        ULONG   FAINT               : 1 <name="气绝">       ;   // 0x00000100;  // 气绝 STUN
        ULONG   ONE_HIT_KILL        : 1 <name="即死">       ;   // 0x00000200;  // 即死 SECOND_KILL ONE_HIT_KILL INSTANT_DEATH

// EMPTY FC:    0x00008000 0x20000000-
#ifdef ED6_FC
        ULONG   DEF_DOWN_FORCE      : 1                     ;   // 0x00000400;  // 绝对降防
        ULONG   RAGE                : 1 <name="愤怒">       ;   // 0x00000800;  // 愤怒
        ULONG   ARTS_GUARD          : 1                     ;   // 0x00001000;  // ArtsGuard
        ULONG   CRAFT_GUARD         : 1                     ;   // 0x00002000;  // CraftGuard
        ULONG   MOV_UP              : 1                     ;   // 0x00004000;
        ULONG   empty16             : 1                     ;   // 0x00008000;
        ULONG   STR_UP              : 1                     ;   // 0x00010000;
        ULONG   STR_DOWN            : 1                     ;   // 0x00020000;
        ULONG   DEF_UP              : 1                     ;   // 0x00040000;
        ULONG   DEF_DOWN            : 1                     ;   // 0x00080000;
        ULONG   SPD_UP              : 1                     ;   // 0x00100000;
        ULONG   SPD_DOWN            : 1                     ;   // 0x00200000;
        ULONG   DEX_UP              : 1                     ;   // 0x00400000;
        ULONG   DEX_DOWN            : 1                     ;   // 0x00800000;
        ULONG   AGL_UP              : 1                     ;   // 0x01000000;
        ULONG   AGL_DOWN            : 1                     ;   // 0x02000000;
        ULONG   MAX_GUARD           : 1                     ;   // 0x04000000;  // 墙 虚无领域 IMMUNE
        ULONG   VANISH_GUARD        : 1                     ;   // 0x08000000;  // 隐身GUARD
        ULONG   DEATH               : 1 <name="战斗不能">   ;   // 0x10000000;  // 战斗不能 00 00 00 10
#endif

// EMPTY SC:    0x40000000
#ifdef ED6_SC_3RD
        ULONG   DEF_DOWN_FORCE      : 1                     ;   // 0x00000400;  // 绝对降防
        ULONG   RAGE                : 1 <name="愤怒">       ;   // 0x00000800;  // 愤怒
        ULONG   ARTS_GUARD          : 1                     ;   // 0x00001000;  // ArtsGuard
        ULONG   CRAFT_GUARD         : 1                     ;   // 0x00002000;  // CraftGuard
        ULONG   MOV_UP              : 1                     ;   // 0x00004000;
        ULONG   MOV_DOWN            : 1                     ;   // 0x00008000;
        ULONG   STR_UP              : 1                     ;   // 0x00010000;
        ULONG   STR_DOWN            : 1                     ;   // 0x00020000;
        ULONG   DEF_UP              : 1                     ;   // 0x00040000;
        ULONG   DEF_DOWN            : 1                     ;   // 0x00080000;
        ULONG   SPD_UP              : 1                     ;   // 0x00100000;
        ULONG   SPD_DOWN            : 1                     ;   // 0x00200000;
        ULONG   ADF_UP              : 1                     ;   // 0x00400000;
        ULONG   ADF_DOWN            : 1                     ;   // 0x00800000;
        ULONG   AGL_UP              : 1                     ;   // 0x01000000;
        ULONG   AGL_DOWN            : 1                     ;   // 0x02000000;
        ULONG   MAX_GUARD           : 1                     ;   // 0x04000000;  // 墙 虚无领域 IMMUNE
        ULONG   VANISH              : 1                     ;   // 0x08000000;  // Vanish
        ULONG   CONDITION_GUARD     : 1                     ;   // 0x10000000;  // 全状态抵抗
        ULONG   BODY_ABNORMAL       : 1 <name="体型变化">   ;   // 0x20000000;  // 变胖/变小/青椒 MORPH
        ULONG   ATS_UP              : 1                     ;   // 0x40000000;
        ULONG   DEATH               : 1 <name="战斗不能">   ;   // 0x80000000;  // 战斗不能
#endif

// EMPTY ZERO:  0x20000000
// EMPTY AO:    0x00000800 0x20000000
#ifdef ED_ZERO_AO
        ULONG   BURN                : 1 <name="炎伤">       ;   // 0x00000400;  // 炎伤
        ULONG   RAGE                : 1 <name="愤怒">       ;   // 0x00000800;  // 愤怒
        ULONG   ARTS_GUARD          : 1                     ;   // 0x00001000;  // ArtsGuard
        ULONG   CRAFT_GUARD         : 1                     ;   // 0x00002000;  // CraftGuard
        ULONG   MAX_GUARD           : 1                     ;   // 0x00004000;  // 墙 虚无领域 IMMUNE
        ULONG   VANISH              : 1                     ;   // 0x00008000;  // Vanish
        ULONG   STR_UP_DOWN         : 1                     ;   // 0x00010000;
        ULONG   DEF_UP_DOWN         : 1                     ;   // 0x00020000;
        ULONG   ATS_UP_DOWN         : 1                     ;   // 0x00040000;
        ULONG   ADF_UP_DOWN         : 1                     ;   // 0x00080000;
        ULONG   DEX_UP_DOWN         : 1                     ;   // 0x00100000;
        ULONG   AGL_UP_DOWN         : 1                     ;   // 0x00200000;
        ULONG   MOV_UP_DOWN         : 1                     ;   // 0x00400000;
        ULONG   SPD_UP_DOWN         : 1                     ;   // 0x00800000;
        ULONG   HP_RECOVERY         : 1                     ;   // 0x01000000;  // HP 按回合回复
        ULONG   CP_RECOVERY         : 1                     ;   // 0x02000000;  // CP 按回合回复
        ULONG   STEALTH             : 1                     ;   // 0x04000000;  // 隐身
        ULONG   MAGIC_REFLECT       : 1                     ;   // 0x08000000;  // 魔法反射
        ULONG   Burning_Heart       : 1                     ;   // 0x10000000;  // 罗伊德燃烧之心第四个状态:全异常状态避免，n回合后气绝。包括vanish和fat，但fat的-20CP效果避免不了。
        ULONG   empty30             : 1                     ;   // 0x20000000;
        ULONG   BODY_ABNORMAL       : 1 <name="体型变化">   ;   // 0x40000000;  // 变胖/变小/青椒 MORPH
        ULONG   DEATH               : 1 <name="战斗不能">   ;   // 0x80000000;  // 战斗不能
#endif
    } condition_bits <open=suppress>;
} RESISTANCE <read=readRESISTANCE, write=writeRESISTANCE, open=suppress>;

string readRESISTANCE(RESISTANCE &a)
{
    string  temp;
    SPrintf(temp,"%08X",a.condition);
    return  temp;
}

void writeRESISTANCE(RESISTANCE &a, string s)
{
    SScanf(s, "%x", a.condition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  type
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef union
{
    int int_signed;
    uint int_unsigned;
    RESISTANCE condition;
} AI_PAR <read=readAI_PAR, write=writeAI_PAR>;

string readAI_PAR(AI_PAR &a)
{
    string  temp;
    SPrintf(temp,"%d",a.int_signed);
    return  temp;
}

void writeAI_PAR(AI_PAR &a, string s)
{
    SScanf(s, "%d", a.int_signed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ED_VERSION
#ifdef ED6_FC
    typedef struct
    {
        USHORT              Level;
        USHORT              HPMax;
        USHORT              HP;

        USHORT              EPMax;
        USHORT              EP;
        USHORT              CP;
        int                 EXP;
        SHORT               STR;
        SHORT               DEF;
        SHORT               ATS;
        SHORT               ADF;
        SHORT               DEX;
        SHORT               AGL;
        SHORT               MOV;
        SHORT               SPD;
        USHORT              CPMAX;
        DUMMY_STRUCT(4);
        USHORT              RNG;
        DUMMY_STRUCT(4);
        RESISTANCE          condition;  // RESISTANCE类型，进程中有效
        USHORT              CharacterIndex;
        DUMMY_STRUCT(2);
    } ED6_CHARACTER_STATUS <optimize=true>;
#endif

#ifdef ED6_SC_3RD
    typedef struct
    {
        UINT                Level;
        UINT                HPMax;
        UINT                HP;
        USHORT              EPMax;
        USHORT              EP;
        USHORT              CP;
        DUMMY_STRUCT(2);
        UINT                EXP;
        SHORT               STR;
        SHORT               DEF;
        SHORT               ATS;
        SHORT               ADF;
        SHORT               DEX;
        SHORT               AGL;
        SHORT               MOV;
        SHORT               SPD;
        USHORT              CPMAX;
        DUMMY_STRUCT(4);
        USHORT              RNG;
        DUMMY_STRUCT(4);
        RESISTANCE          condition;  // RESISTANCE类型，进程中有效
        USHORT              CharacterIndex;
        DUMMY_STRUCT(2);
    } ED6_CHARACTER_STATUS <optimize=true>;
#endif

#ifdef ED_ZERO_AO
    typedef struct
    {
        int                 HPMax;
        int                 HP;
        ushort              Level;
        ushort              EPMax;
        ushort              EP;
        ushort              CP;
        uint                EXP;
        short               STR;
        short               DEF;
        short               ATS;
        short               ADF;
        short               DEX;
        short               AGL;
        short               MOV;
        short               SPD;
        short               DEXRate;
        short               AGLRate;
        ushort              CPMAX;
        ubyte               DUMMY_STRUCT_01[2]  <hidden=true>;
        ushort              RNG;        // 内存中改StatusBasic中的才有效
        ubyte               DUMMY_STRUCT_02[2]  <hidden=true>;
        RESISTANCE          condition;  // RESISTANCE类型，进程中有效; 战斗外包括存档中 DEATH有效
    } ED6_CHARACTER_STATUS;
#endif
typedef ED6_CHARACTER_STATUS    ED7_CHARACTER_STATUS, STATUS, CHR_STATUS;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct  // 0x14
{
    RESISTANCE          ConditionFlags;
    PTR32               Effect;
    // 1 回合; 2 次数; 3 AT条动多少次; 4 永久
    enum <UCHAR>
    {
        ByAT        = 0,
        ByRounds    = 1,
        ByTimes     = 2,
        ByActions   = 3,
        Infinite    = 4,
    }                   CounterType <format=hex>;
    UCHAR               Flags <format=hex>;
    SHORT               ConditionRate;
    ULONG               ATLeft;
    ULONG               ULong_10;
} CONDITION <read=readCONDITION, write=writeCONDITION>;

string readCONDITION(CONDITION &a)
{
    string  str;
    if (a.ConditionFlags.condition != 0)
    {
        SPrintf(str, "%08X", a.ConditionFlags.condition);
    }
    return  str;
}

void writeCONDITION(CONDITION &a, string s) {};

// 逃跑参数 03320000 每次行动50%概率逃跑; 01320A00 血量10%以下50%概率逃跑; 02320000 头头死亡后50%概率逃跑
typedef UCHAR RUNAWAY_INFO[4]  <read=readRUNAWAY_INFO, write=writeRUNAWAY_INFO>;
typedef RUNAWAY_INFO    FleeParameter;

wstring readRUNAWAY_INFO(RUNAWAY_INFO a)
{
    string  str;
    switch (a[0])
    {
        case 0:
            return L"不逃跑";
        case 1:
            SPrintf(str, "%d%%(HP<=%d%%)", a[1], a[2]);
            break;
        case 2:
            SPrintf(str, "%d%%(头目阵亡)", a[1]);
            break;
        case 3:
            SPrintf(str, "%d%%(每次行动)", a[1]);
            break;
        default:
            SPrintf(str, "未知类型 %02X %02X %02X %02X", a[0], a[1], a[2], a[3]);
    }
    return L"逃跑概率(条件): " + StringToWString(str, CHARSET_UTF8);
}

void writeRUNAWAY_INFO(RUNAWAY_INFO &a, string s) {};

typedef struct  // Ai格式
{
    ubyte   Time <format=hex>;
    ubyte   Probability;
    ubyte   Target <format=hex>;
    ubyte   DUMMY_STRUCT_01;        // 01
    ubyte   CraftAnimation1;        // 动画代码，在AS文件中定义。蓄魔法06、放魔法07、C技10-19，S技1A-1F
    ubyte   CraftAnimation2;
    ushort  CraftIndex;             // 技能代码，常规查t_magic._dt，自定义技能从E8 03开始
    AI_PAR  TimeParameter1;
    AI_PAR  TimeParameter2;
    AI_PAR  TargetParameter1;
    AI_PAR  TargetParameter2;
} ED6_AI_INFO <read=readCraftIndex>;
typedef ED6_AI_INFO ED7_AI_INFO, AI_INFO;

string readCraftIndex(ED6_AI_INFO &a)
{
    string  temp;
    SPrintf(temp,"%d",a.CraftIndex);
    return  temp;
}

typedef struct
{
    CHAR Intro[0x100];
    CHAR Name[0x20];
} CRAFT_INTRO <read=readCRAFT_INTRO, write=writeCRAFT_INTRO>;
typedef CRAFT_INTRO ED6_CRAFT_INTRO, ED7_CRAFT_INTRO;

string readCRAFT_INTRO(CRAFT_INTRO &a)
{
    return a.Name;
}

void writeCRAFT_INTRO(CRAFT_INTRO &a, string s)
{
    a.Name = s;
}

typedef struct  // 自定义技能格式
{
    ushort  AnimationType;
    ubyte   Target;
    ubyte   SpecialEffect;      // 01 强制命中 02 驱动时无法被打断
    ubyte   Att;                // 属性
    // 空轨FCSC       0无 1地 2水 3火 4风 5空 6幻 0时 // FCSC没有 时空幻有效率，所以无所谓
    // 空轨3rd        0无 1地 2水 3火 4风 5空 6幻 7时
    // 零轨碧轨     0无 1地 2水 3火 4风 5时 6空 7幻
    ubyte   ShapeScope;         // 范围形状
    ubyte   Effect1;
    ubyte   Effect2;
    if (CRAFT_INFO_SIZE == 32)
    {
        ushort  RNG;
        ushort  ScopeRadius;        // 半径
        ushort  STCharge;
        ushort  STCoolDown;
        ushort  CP_EP;
        ushort  DisplayIndex;       // 魔法列表中的显示先后，t_magic._dt中的才有效，ms文件中的一般为1或0。
    }
    else if (CRAFT_INFO_SIZE == 28)
    {
        ubyte   DisplayIndex;       // 魔法列表中的显示先后，t_magic._dt中的才有效，ms文件中的一般为1或0。
        ubyte   RNG;
        ubyte   STCharge;
        ubyte   STCoolDown;
        ushort  CP_EP;
        ushort  ScopeRadius;        // 半径
    }
    short   Effect1Parameter;
    ushort  Effect1ST;
    short   Effect2Parameter;
    ushort  Effect2ST;
    if (!exists(parentof(this).addrCraftDefinition))
    {
        string  CraftName;          // max 32 bytes
        string  CraftIntro;         // max 256 bytes
    }
    else
    {
        DUMMY_STRUCT(4);/*
        local int   addrTemp1 = FTell();
        local int   addrCraftName = addrCraftDefinition + CRAFT_INFO_SIZE*16 + 288*((addrTemp1-addrCraftDefinition)/(CRAFT_INFO_SIZE)-1) + 256;
        local int   addrCraftIntro = addrCraftDefinition + CRAFT_INFO_SIZE*16 + 288*((addrTemp1-addrCraftDefinition)/(CRAFT_INFO_SIZE)-1);
        FSeek(addrCraftName);
        string  CraftName;
        FSeek(addrCraftIntro);
        string  CraftIntro;*/
        FSeek(startof(parentof(this)) + CRAFT_INFO_SIZE * 16 + index * sizeof(CRAFT_INTRO));
        CHAR CraftIntro[0x100];
        CHAR CraftName[0x20];
        FSeek(startof(this) + CRAFT_INFO_SIZE);
    }
} ED6_CRAFT_INFO <read=readCraftName,write=writeCraftName>;
typedef ED6_CRAFT_INFO ED7_CRAFT_INFO, CRAFT_INFO;

string readCraftName(ED6_CRAFT_INFO &a)
{
    return  a.CraftName;
}

void writeCraftName(ED6_CRAFT_INFO &a, string s)
{
    // 复制用
}

typedef struct  // 合在一块
{
    local ULONG addrCraftDefinition = FTell();
    local ULONG index;
    for (index = 0; index < 16; index++)
        ED6_CRAFT_INFO   CraftInfo;
} CRAFT_INFO_GROUP;
typedef CRAFT_INFO_GROUP ED6_CRAFT_DEFINITION_GROUP, ED7_CRAFT_DEFINITION_GROUP;

// 最后一次(正在)使用的战技
typedef struct
{
    ushort  CraftIndex;
    ubyte   CraftAnimation1;
    ubyte   CraftAnimation2;
} CraftLastUsed_INFO <read=readCraftLastUsedIndex>;

string readCraftLastUsedIndex(CraftLastUsed_INFO &a)
{
    string  temp;
    SPrintf(temp,"%d %d %d",a.CraftIndex, a.CraftAnimation1, a.CraftAnimation2);
    return  temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    ushort  Equip[5] <optimize=false>;
} EQUIP;

typedef struct
{
    ushort  SlotQuartz;
    ushort  SlotLevel;
} SLOT;

typedef struct
{
    SLOT    Slot[SLOT_COUNT] <optimize=false>;
} ORBMENT <optimize=true>;

typedef struct
{
    ushort  Art[80] <optimize=false>;
} ARTS;

typedef struct
{
#ifdef ED_ZERO
    ushort  Craft[10] <optimize=false>;
#endif

#ifdef ED_AO
    ushort  Craft[15] <optimize=false>;
#endif
} CRAFT;

typedef struct
{
    ushort  SCraft[5] <optimize=false>;
} SCRAFT;

typedef struct  // 站位
{
    ubyte       x;  // 左下角为00，左右为x，上下为y
    ubyte       y;
} POSITION <read=readPosition>;

string readPosition(POSITION &a)
{
    string s;
    SPrintf(s,"%d %d", a.x, a.y);
    return  s;
};

#ifdef ED6_3RD
typedef struct
{
    ushort  Party1;
    ushort  Party2;
    ushort  Party3;
    ushort  Party4;
} TEAM_GROUP;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ED6_FC
#include "ed61_common.bt.h"
#endif

#ifdef ED6_SC
#include "ed62_common.bt.h"
#endif

#ifdef ED6_3RD
#include "ed63_common.bt.h"
#endif

#ifdef ED_ZERO
#include "ed71_common.bt.h"
#endif

#ifdef ED_AO
#include "ed72_common.bt.h"
#endif

/*******************************************************************************
    CHR_BATTLE_INF common
*******************************************************************************/
typedef ED6_CHARACTER_BATTLE_INF    CHR_BATTLE_INF, MONSTER_STATUS;

string readCHR_BATTLE_INF(CHR_BATTLE_INF &a)
{
    return  readFileIndex(a.MSFileIndex) + " " + a.ChrName.str;
}
void writeCHR_BATTLE_INF(CHR_BATTLE_INF &a, string  s) {};

#ifdef AT_BAR_ENTRY_SIZE
/*******************************************************************************
    CBattleATBar common
*******************************************************************************/
string readAT_BAR_ENTRY(AT_BAR_ENTRY &entry)
{
    if (~entry.Flags & 0x20)
    {
        return "nul";
    }
    else
    {
        string  str;
        ULONG   id = ReadHeapUShort(entry.MSData);
        SPrintf(str, "%02d %02d:AT_%02d %02X ", entry.Sequence, id, entry.IconAT, entry.Flags);
        if (exists(ChrBattleInf[id].ChrName.str))
        {
            str += ChrBattleInf[id].ChrName.str;
        }
        return  str;
    }
}

// CActTurnWindow
typedef struct
{
#ifdef ED6_ALL
    ArrayBytes      LeadingBytes(4);
#endif
    //AT_BAR_ENTRY    Entry[AT_BAR_ENTRY_COUNT];
    DUMMY_STRUCT(AT_BAR_ENTRY_COUNT * AT_BAR_ENTRY_SIZE);
    PTR32           EntryPointer[AT_BAR_ENTRY_COUNT];

    local ULONG     i;
    for (i = 0; i < AT_BAR_ENTRY_COUNT; ++i)
    {
        if (FSeekHeap(EntryPointer[i]) != -1)
        {
            if(~ReadUShort(FTell() + AT_BAR_ENTRY_OFFSET_FLAGS) & 0x20)
            {
                AT_BAR_ENTRY    Ico <hidden=true>;
            }
            else
            {
                AT_BAR_ENTRY    Ico;
            }
        }
    }
    FSeek(startof(this) + sizeof(this));
} CBattleATBar <read=readCBattleATBar, size=sizeCBattleATBar>;

wstring readCBattleATBar(CBattleATBar &a)
{
    return L"AT条";
}

int sizeCBattleATBar(CBattleATBar &a)
{
    return 4 + AT_BAR_ENTRY_COUNT * AT_BAR_ENTRY_SIZE + AT_BAR_ENTRY_COUNT * sizeof(PTR32);
}
#endif

#endif