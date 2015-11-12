#ifndef _ed71_common_bt_h_
#define _ed71_common_bt_h_

local const UINT        CHR_BATTLE_INF_SIZE     = 0x243C;
local const UINT        CHR_BATTLE_INF2_SIZE    = 0x320;

typedef struct
{
    local int           addrStart = FTell();
    local int           soldierIndex = j;
    // 0
    ushort              SoldierNo;                  // 我方0-7，敌方8-15，支援16-19，待分身20-21
    ubyte               unknownFlags1 <format=hex, hidden=true>;
    ubyte               Flags <format=hex>;                      // 10 敌方 40 己方 ...
    ubyte               DeathFlags <format=hex>;                 // 02 不参与战场胜利判定 04 死后留在战场上
    ubyte               UnderAttackFlags <format=hex>;           // 08 Resist ATDelay 02 不被击退 01 无法移动(移动会卡死)  10 无法被攻击到  20 强制miss
    FSkip(4);   // 复制上面四个参数？？？
    ushort              CharacterIndex;
    FSkip(2);   // FF 00 常量？
    ushort              ATActTime;  // AT条动多少次，初始为0; AiTime1F
    FileIndex           SYFileIndex <format=hex>;
    FileIndex           MSFileIndex <format=hex>;
    FileIndex           ASFileIndex <format=hex>;

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
    // 568
    local uint          addrHP = FTell();
    STATUS              StatusBasic;    // normal难度基础值
    // 568 + 52
    STATUS              StatusSum;      // 算上难度、装备、回路
    ushort              MoveSPD <hidden=true>;                // 移动速度，我方人员也从ms文件中读取
    ushort              MoveAfterAttack <hidden=true>;        // 从ms文件中读取
    CONDITION           Condition[32];
    ubyte               DUMMY_STRUCT_02[24] <hidden=true>;
    uint                AT;
    uint                AT2;    // AiTime08用
    // 0x544
    ushort              AIType;
    ushort              EXPGet;                     // 战斗胜利 结算经验前暂存
    ushort              DropIndex1 <hidden=true>;                 // 掉落物 物品代码
    ushort              DropIndex2 <hidden=true>;
    ubyte               DropProbability1 <hidden=true>;           // 掉落概率
    ubyte               DropProbability2 <hidden=true>;
    ubyte               Sex <hidden=true>;
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
    FleeParameter       fleeParameter <hidden=true>;
    // 3768 + addrStart
    // 16条战技定义共512字节，（说明256 名字32）共288x
    local uint          addrCraftDefinition = FTell();
    local ushort        CRAFT_INFO_SIZE = 32;   // 魔法条长度 碧轨24+4 空轨零轨28+4
    ED7_CRAFT_DEFINITION_GROUP  CraftDefinition;    // 实际上只能放15个
    // 4280
    ////ubyte               DUMMY_STRUCT_06[32] <hidden=true>;
    ubyte               DUMMY_STRUCT_07[288*16] <hidden=true>;    // 战技说明
    ubyte               DUMMY_STRUCT_08[64] <hidden=true>;
    // FSkip(288*15 + 352);
    // 8952
    ubyte               Sepith[7] <hidden=true>;                  // 掉落耀晶片
    ubyte               DUMMY_STRUCT_09 <hidden=true>;
    ushort              ArtsAttResistance[8] <hidden=true>;       // 七曜属性有效率
    RESISTANCE          Resistance;
    ubyte               DUMMY_STRUCT_11[132] <hidden=true>;

    FSeek(addrBattleStart + 0x660 + 0x320*soldierIndex + 0xE0);
    string              CharacterName;
    // 9112
    FSeek(addrStart + 0x2398);
    string              CharacterIntro;
    FSeek(addrStart + 0x2418);
    ubyte               DUMMY_STRUCT_12[8] <hidden=true>;
    FSeek(addrStart + CHR_BATTLE_INF_SIZE);
    // 9112 + 164 = 9276 243C

} ED7_CHARACTER_BATTLE_INF <read = readMSFileIndex>;

string readMSFileIndex(ED7_CHARACTER_BATTLE_INF &a)
{
    string  temp;
    SPrintf(temp,"ms%x",a.MSFileIndex);
    temp = StrDel(temp,2,3);
    return  temp + " " + a.CharacterName;
}

#endif