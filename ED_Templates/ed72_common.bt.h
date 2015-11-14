#ifndef _ed72_common_bt_h_
#define _ed72_common_bt_h_

local const UINT        CHR_BATTLE_INF_SIZE     = 0x2424;
local const UINT        CHR_BATTLE_INF2_SIZE    = 0x31C;

typedef struct
{
    ushort              SoldierNo;                  // 我方0-7，敌方8-15，支援16-19，待分身20-21
    ubyte               unknownFlags1 <format=hex, hidden=true>;
    ubyte               Flags <format=hex>;                      // 10 敌方 40 己方 ...
    ubyte               DeathFlags <format=hex>;                 // 02 不参与战场胜利判定 04 死后留在战场上
    ubyte               UnderAttackFlags <format=hex>;           // 08 Resist ATDelay 02 不被击退 01 无法移动(移动会卡死)  10 无法被攻击到  20 强制miss
    FSkip(4);
    ushort              CharacterIndex;
    FSkip(2);
    USHORT              ATShiftUpCount;             // AT条动多少次，初始为0
    FileIndex           SYFileIndex;                // 0x10
    FileIndex           MSFileIndex;                // 0x14
    FileIndex           ASFileIndex;                // 0x18

        DUMMY_STRUCT(1);                                    // 0x1C
        BYTE                    TeamRushAddition;           // 0x1D
        USHORT                  MasterQuartzUsedFlag;       // 0x1E

        DUMMY_STRUCT(0x16C - 0x20);

        USHORT                  CurrentActionType;          // 0x16C

        DUMMY_STRUCT(2);

        USHORT                  PreviousActionType;         // 0x170
        USHORT                  SelectedActionType;         // 0x172
        USHORT_H                Unknown_174;
        USHORT_H                Unknown_176;
        USHORT_H                Unknown_178;
        USHORT_H                Unknown_17A;
        USHORT                  WhoAttackMe;                // 0x17C
        USHORT                  CurrentCraftIndex;          // 0x17E
        USHORT                  LastActionIndex;            // 0x180
        USHORT                  CurrentAiIndex;             // 0x182

        DUMMY_STRUCT(0x1AA - 0x184);

        USHORT                  Target[0x10];               // 0x1AA
        BYTE                    TargetCount;                // 0x1CA
        BYTE                    SelectedTargetIndex;        // 0x1CB
        COORD                   SelectedTargetPos;          // 0x1CC

        //DUMMY_STRUCT(0x234 - 0x1D0);
        DUMMY_STRUCT(0x20C - 0x1D0);
        BYTE                    IsHitMiss[0x10];            // 0x20C
        BYTE                    IsHitJudged[0x10];          // 0x21C
        DUMMY_STRUCT(0x234 - 0x22C);

    //FSeek(startof(this) + 0x234);
    STATUS              StatusBasic;                // 0x234
    STATUS              StatusSum;                  // 0x268
    ushort              MoveSPD;
    ushort              MoveAfterAttack;
    CONDITION           Condition[32];
    FSkip(24);
    int                 AT;
    int                 AT2;

    ushort              AIType;
    ushort              EXPGet;                     // 战斗胜利 结算经验前暂存
    ushort              DropIndex[2];               // 掉落物 物品代码
    ubyte               DropProbability[2];         // 掉落概率
    ubyte               Sex;
    ubyte               DisplayHighLevelArtsAttResistance <hidden=true>;
                                                    // 是否开启时空幻有效率显示，0/8 不开启 1/9 开启，应该是标志位，可能含有更多信息
    ushort              Equip[5];                   // 装备
    ushort              Orb[7];                     // 回路   7个

    ED7_AI_INFO         NormalAttack;
    ED7_AI_INFO         Arts[80];
    ED7_AI_INFO         Craft[15];
    ED7_AI_INFO         SCraft[5];
    ED7_AI_INFO         SupportCraft[3];
    CraftLastUsed_INFO  CraftLastUsed;
    RUNAWAY_INFO        RunawayInfo;
    CRAFT_INFO_GROUP    CraftDefinition;
    CRAFT_INTRO         CraftIntro[0x10];

    //FSeek(startof(this) + 0x22EC);
    ubyte               Sepith[7];                  // 掉落耀晶片
    FSkip(1);
    ushort              ArtsAttResistance[8];       // 七曜属性有效率
    RESISTANCE          Resistance;                 // 0x2304


    FSeek(addrCBattleLocal + 0x660 + CHR_BATTLE_INF2_SIZE * SoldierNo + 0xE0);
    String              ChrName;

    FSeek(startof(this) + 0x2380);
    string              ChrIntro;

    FSeek(startof(this) + 0x2408);
    ULONG               SummonCount;                // 0x2408

    FSeek(addrCBattleLocal + 0x5F00 * SoldierNo + 0x3A800);
    USHORT tempAS;

    FSeek(startof(this) + CHR_BATTLE_INF_SIZE);
} ED6_CHARACTER_BATTLE_INF <read=readCHR_BATTLE_INF, write=writeCHR_BATTLE_INF>;

typedef struct  // 0x78
{
    DUMMY_STRUCT(0x60);
    PTR32       MSData;     // 0x60
    PTR32       addrCBattle;// 0x64
    INT         IconAT;     // 0x68
    UHEX8       Flags;      // 0x6C 不含20 空  含04 行动、delay后的([20A]0销毁); 含40 当前行动的(1销毁)
    UCHAR       Sequence;   // 0x6E
    DUMMY_STRUCT(2);
    UCHAR       RNo;        // 0x71
    DUMMY_STRUCT(1);
    UCHAR       Pri;        // 0x73 pri
    UCHAR       IsSBreaking;// 0x74
    DUMMY_STRUCT(3);
} AT_BAR_ENTRY <read=readAT_BAR_ENTRY, optimize=true>;

#define AT_BAR_ENTRY_COUNT          0x3C
#define AT_BAR_ENTRY_SIZE           0x78
#define AT_BAR_ENTRY_OFFSET_FLAGS   0x6C

#endif