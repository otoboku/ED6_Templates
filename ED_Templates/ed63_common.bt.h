#ifndef _ed63_common_bt_h_
#define _ed63_common_bt_h_

local const UINT        CHR_BATTLE_INF_SIZE     = 0x2490;
local const UINT        CHR_BATTLE_INF2_SIZE    = 0x2D8;

typedef struct
{
    ushort              SoldierNo;                  // 我方0-7，敌方8-15，支援16-19，待分身20-21
    ubyte               unknownFlags1 <format=hex, hidden=true>;
    ubyte               Flags <format=hex>;                      // 10 敌方 40 己方 ...
    ubyte               DeathFlags <format=hex>;                 // 02 不参与战场胜利判定 04 死后留在战场上
    ubyte               UnderAttackFlags <format=hex>;           // 08 Resist ATDelay 02 不被击退 01 被攻击不转身(3D)  10 无法被攻击到  20 强制miss
    FSkip(4);
    ushort              CharacterIndex;
    FSkip(2);
    USHORT              ATShiftUpCount;             // AT条动多少次，初始为0
    FileIndex           SYFileIndex;                // 0x10
    FileIndex           MSFileIndex;                // 0x14
    FileIndex           ASFileIndex;                // 0x18
    DUMMY_STRUCT(0x162);
    USHORT              CurrentActionType;          // 0x17E 0 攻击；1 移动；2 魔法；3战技；4 S技；5 道具；9 待选择
    DUMMY_STRUCT(0xA);
    USHORT              CurrentCraftIndex;          // 0x18A
    DUMMY_STRUCT(0x2C);
    USHORT              Target[0x10];               // 0x1B8
    BYTE                TargetCount;                // 0x1D8
    BYTE                SelectedTargetIndex;        // 0x1D9
    COORD               SelectedTargetPos;          // 0x1DA
    DUMMY_STRUCT(0x32);
    BYTE                IsHitMiss[0x10];            // 0x210
    DUMMY_STRUCT(4);
    PCHAR               ChrName;                    // 0x224
    DUMMY_STRUCT(4);                                // 0x228
    FSeek(startof(this) + 0x22C);
    STATUS              StatusBasic;                // normal难度基础值
    STATUS              StatusSum;                  // 算上难度、装备、回路
    ushort              MoveSPD;                    // 移动速度，我方人员也从ms文件中读取
    ushort              MoveAfterAttack;            // 从ms文件中读取
    CONDITION           Condition[32];
    FSkip(24);
    int                 AT;
    int                 AT2;
    FSkip(4);
    ushort              AIType;
    ushort              DropIndex[2];               // 掉落物 物品代码
    ubyte               DropProbability[2];         // 掉落概率
    ubyte               Sex;
    ubyte               DisplayHighLevelArtsAttResistance <hidden=true>;
                                                    // 是否开启时空幻有效率显示，0/8 不开启 1/9 开启，应该是标志位，可能含有更多信息
    ushort              Equip[5];                   // 装备
    ushort              Orb[7];                     // 回路   7个
    ubyte               DUMMY_STRUCT_021[2] <hidden=true>;
    ED6_AI_INFO         NormalAttack;
    ED6_AI_INFO         Art[80];
    ED6_AI_INFO         Craft[10];
    ED6_AI_INFO         SCraft[5];
//    ED6_AI_INFO         SupportCraft[3];
    CraftLastUsed_INFO  CraftLastUsed;
    RUNAWAY_INFO        RunawayInfo;
    CRAFT_INFO_GROUP    CraftDefinition;
    CRAFT_INTRO         CraftIntro[0x10];

    FSeek(startof(this) + 0x22C3);
    ubyte               Sepith[7];                  // 掉落耀晶片
    FSkip(2);
    ushort              EXPGet;                     // 战斗胜利 结算经验前暂存
    FSkip(2);
    ushort              ArtsAttResistance[7];       // 七曜属性有效率
    FSkip(2);
    RESISTANCE          Resistance;                 // 0x22E0

    FSeek(startof(this) + 0x2370);
    string              ChrIntro;                   // 0x2370

    FSeek(startof(this) + 0x1FC);
    float               TowardDegree;

    FSeek(startof(this) + CHR_BATTLE_INF_SIZE);
} ED6_CHARACTER_BATTLE_INF <read=readCHR_BATTLE_INF, write=writeCHR_BATTLE_INF>;

typedef struct  // 0x98
{
    DUMMY_STRUCT(0x88);
    UCHAR       Sequence;   // 0x88
    DUMMY_STRUCT(1);
    UCHAR       Flags;      // 0x8A
    DUMMY_STRUCT(1);
    PTR32       MSData;     // 0x8C
    ULONG       IconAT;     // 0x90
    DUMMY_STRUCT(4);
} AT_BAR_ENTRY <read=readAT_BAR_ENTRY, optimize=true>;

#define AT_BAR_ENTRY_COUNT          0x3C
#define AT_BAR_ENTRY_SIZE           0x98
#define AT_BAR_ENTRY_OFFSET_FLAGS   0x8A

#endif