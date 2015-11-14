#ifndef _ed71_common_bt_h_
#define _ed71_common_bt_h_

local const UINT        CHR_BATTLE_INF_SIZE     = 0x243C;
local const UINT        CHR_BATTLE_INF2_SIZE    = 0x320;

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

    ubyte               DUMMY_STRUCT_00[312] <hidden=true>;
    ubyte               DUMMY_STRUCT_001[40] <hidden=true>;
    ubyte               dieAfterSoldierNoDie <hidden=true>;   // 分身用，正常FF
    ubyte               DUMMY_STRUCT_002[85] <hidden=true>;
    ubyte               attackStatus[8] <hidden=true,hidden=true>;
    // 最近八次的攻击与被攻击情况 队列。从AT=0开始累计，下次AT=0时结果后移，位置0清空。
    // 01 受到攻击掉血大于0; 02 发动攻击命中掉血; 04 上次未使用,对应AiTime09;
    // 08 受到攻击掉血大于1/10; 10 受到物理攻击命中掉血
    ubyte               DUMMY_STRUCT_003[51] <hidden=true>;
    ubyte               IsLastFightMissToSoldierNo[16];// <hidden=true>;
    ubyte               IsLastFightTargetToSoldierNo[16];// <hidden=true>;
    ubyte               DUMMY_STRUCT_01[11] <hidden=true>;

    //FSeek(startof(this) + 0x238);
    STATUS              StatusBasic;                // 0x238
    STATUS              StatusSum;                  // 0x26C
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
    ED7_AI_INFO         Craft[10];
    ED7_AI_INFO         SCraft[5];
    ED7_AI_INFO         SupportCraft[3];
    CraftLastUsed_INFO  CraftLastUsed;
    RUNAWAY_INFO        RunawayInfo;
    CRAFT_INFO_GROUP    CraftDefinition;
    CRAFT_INTRO         CraftIntro[0x10];
    FSkip(0x40);

    //FSeek(startof(this) + 0x22F8);
    ubyte               Sepith[7];                  // 掉落耀晶片
    FSkip(1);
    ushort              ArtsAttResistance[8];       // 七曜属性有效率
    RESISTANCE          Resistance;                 // 0x2310

    FSeek(addrCBattleLocal + 0x660 + CHR_BATTLE_INF2_SIZE * SoldierNo + 0xE0);
    String              ChrName;
    // 9112
    FSeek(startof(this) + 0x2398);
    string              ChrIntro;

    FSeek(startof(this) + CHR_BATTLE_INF_SIZE);
} ED6_CHARACTER_BATTLE_INF <read=readCHR_BATTLE_INF, write=writeCHR_BATTLE_INF>;

#endif