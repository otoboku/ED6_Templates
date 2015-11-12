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
    FSkip(4);   // 复制上面四个参数？？？
    ushort              CharacterIndex;
    FSkip(2);   // FF 00 常量？
    USHORT              ATShiftCount;               // AT条动多少次，初始为0
    FileIndex           SYFileIndex <format=hex>;
    FileIndex           MSFileIndex <format=hex>;
    FileIndex           ASFileIndex <format=hex>;
        DUMMY_STRUCT(1);                                    // 0x1C
        BYTE                    TeamRushAddition;           // 0x1D
        USHORT                  MasterQuartzUsedFlag;       // 0x1E

        DUMMY_STRUCT(0x16C - 0x20);

        USHORT                  CurrentActionType;          // 0x16C

        DUMMY_STRUCT(2);

        USHORT                  PreviousActionType;         // 0x170
        USHORT                  SelectedActionType;         // 0x172
        USHORT                  Unknown_174;
        USHORT                  Unknown_176;
        USHORT                  Unknown_178;
        USHORT                  Unknown_17A;
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

    local uint          addrHP = FTell();
    STATUS              StatusBasic;    // normal难度基础值
    // 568 + 52
    STATUS              StatusSum;      // 算上难度、装备、回路
    ushort              MoveSPD <hidden=true>;                // 移动速度，我方人员也从ms文件中读取
    ushort              MoveAfterAttack ;        // 从ms文件中读取
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
    ED7_AI_INFO         Craft[15];
    ED7_AI_INFO         SCraft[5];
    ED7_AI_INFO         SupportCraft[3];
    CraftLastUsed_INFO  CraftLastUsed;
    FleeParameter       fleeParameter;
    // 3768 + startof(this)
    // 16条战技定义共512字节，（说明256 名字32）共288x
    //DUMMY_STRUCT(0x50);
    //DUMMY_STRUCT(0x38);
    local uint          addrCraftDefinition = FTell();
    local ushort        CRAFT_INFO_SIZE = 28;   // 魔法条长度 碧轨24+4 空轨零轨28+4
    ED7_CRAFT_DEFINITION_GROUP  CraftDefinition;    // 实际上只能放15个
    // 4280
    ////ubyte               DUMMY_STRUCT_06[32] <hidden=true>;
    ubyte               DUMMY_STRUCT_07[288*16];    // 战技说明
    //ubyte             DUMMY_STRUCT_08[64];
    // FSkip(288*15 + 352);
    // 8952
    ubyte               Sepith[7];                  // 掉落耀晶片
    ubyte               DUMMY_STRUCT_09 <hidden=true>;
    ushort              ArtsAttResistance[8];       // 七曜属性有效率
    RESISTANCE          Resistance;


    FSeek(addrBattleStart + 0x660 + 0x31C*SoldierNo + 0xE0);
    //FSeek(ProcessHeapToLocalAddress(ReadUInt(addrHP-8)));
    string              CharacterName;
    // 9112
    FSeek(startof(this) + 0x2380);
    string              CharacterIntro;

    FSeek(startof(this) + 0x2408);
    ULONG                       SummonCount;            // 0x2408

    FSeek(startof(this) + 0x2418);
    ubyte               DUMMY_STRUCT_12[8] <hidden=true>;

    FSeek(addrCBattle + 0x5F00 * SoldierNo + 0x3A800);
    USHORT tempAS;
    FSeek(startof(this) + CHR_BATTLE_INF_SIZE);
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