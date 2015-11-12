#ifndef _ed61_common_bt_h_
#define _ed61_common_bt_h_

local const UINT        CHR_BATTLE_INF_SIZE     = 0x23C8;
local const UINT        CHR_BATTLE_INF2_SIZE    = 0x208;

typedef struct
{
    ushort              SoldierNo;                  // 我方0-7，敌方8-15，支援16-19，待分身20-21
    ubyte               unknownFlags1 <format=hex, hidden=true>;
    ubyte               Flags <format=hex>;                      // 10 敌方 40 己方 ...
    ubyte               DeathFlags <format=hex>;                 // 02 不参与战场胜利判定 04 死后留在战场上
    ubyte               UnderAttackFlags <format=hex>;           // 08 Resist ATDelay 02 不被击退 01 被攻击不转身(3D)  10 无法被攻击到  20 强制miss
    FSkip(4);
    ushort              CharacterIndex;
    //FSkip(2);
    //ushort              ATActTime;  // AT条动多少次，初始为0
    FileIndex           SYFileIndex;
    FileIndex           MSFileIndex;
    FileIndex           ASFileIndex;

    FSeek(startof(this) + 0x1C8);
    PCHAR               ChrName;                    // 0x1C8
    DUMMY_STRUCT(4);                                // 0x1CC
    FSeek(startof(this) + 0x1D0);
    STATUS              StatusBasic;                // normal难度基础值
    STATUS              StatusSum;                  // 算上难度、装备、回路
    ushort              MoveSPD;                    // 移动速度，我方人员也从ms文件中读取
    ushort              MoveAfterAttack;            // 从ms文件中读取
    CONDITION           Condition[29];
    FSkip(24);
    int                 AT;
    int                 AT2;
    FSkip(6);
    ushort              AIType;
    FSkip(2);
    ushort              DropIndex[2];               // 掉落物 物品代码
    ubyte               DropProbability[2];         // 掉落概率
    ubyte               Sex;
    ubyte               DisplayHighLevelArtsAttResistance <hidden=true>;
                                                    // 是否开启时空幻有效率显示，0/8 不开启 1/9 开启，应该是标志位，可能含有更多信息
    ushort              Equip[5];                   // 装备
    ushort              Orb[6];                     // 回路   7个
    ED6_AI_INFO         NormalAttack;
    ED6_AI_INFO         Art[80];
    ED6_AI_INFO         Craft[10];
    ED6_AI_INFO         SCraft[4];
//    ED6_AI_INFO         SupportCraft[3];
    CraftLastUsed_INFO  CraftLastUsed;
    RUNAWAY_INFO        RunawayInfo;
    CRAFT_INFO_GROUP    CraftDefinition;
    CRAFT_INTRO         CraftIntro[0x10];

    FSeek(startof(this) + 0x21C4);
    ubyte               Sepith[7];                  // 掉落耀晶片
    FSkip(1);
    ushort              EXPGet;                     // 战斗胜利 结算经验前暂存
    FSkip(2);
    ushort              ArtsAttResistance[4];       // 七曜属性有效率

    FSeek(startof(this) + 0x225C);
    RESISTANCE          Resistance;                 // 0x225C

    FSeek(startof(this) + 0x22A8);
    string              ChrIntro;                   // 0x22A8

    FSeek(startof(this) + CHR_BATTLE_INF_SIZE);
} ED6_CHARACTER_BATTLE_INF <read=readCHR_BATTLE_INF, write=writeCHR_BATTLE_INF>;

#ifndef ED6_FC_STEAM
typedef struct  // 0x94
{
    DUMMY_STRUCT(0x84);
    UCHAR       Flags;      // 0x84
    DUMMY_STRUCT(3);
    PTR32       MSData;     // 0x88
    ULONG       IconAT;     // 0x8C
    UCHAR       Sequence;   // 0x90
    DUMMY_STRUCT(3);
} AT_BAR_ENTRY <read=readAT_BAR_ENTRY, optimize=true>;

#define AT_BAR_ENTRY_COUNT          0x3C
#define AT_BAR_ENTRY_SIZE           0x94
#define AT_BAR_ENTRY_OFFSET_FLAGS   0x84
#else
typedef struct  // 0x98
{
    DUMMY_STRUCT(0x88);
    UCHAR       Flags;      // 0x88
    DUMMY_STRUCT(3);
    PTR32       MSData;     // 0x8C
    ULONG       IconAT;     // 0x90
    UCHAR       Sequence;   // 0x94
    DUMMY_STRUCT(3);
} AT_BAR_ENTRY <read=readAT_BAR_ENTRY, optimize=true>;

#define AT_BAR_ENTRY_COUNT          0x3C
#define AT_BAR_ENTRY_SIZE           0x98
#define AT_BAR_ENTRY_OFFSET_FLAGS   0x88
#endif

#endif