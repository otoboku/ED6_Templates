//--------------------------------------
//--- 010 Editor v3.2 Binary Template
//
// File: zero_ao_scena.bt
// Author:acgngca
// Revision:2011-11-22
// Purpose:data\scena\xxxxx.bin
// Readme:
//
//--------------------------------------
#ifndef ED_VERSION
#define ED_ZERO
#endif
#include "ED6_common.bt.h"

// 场景文件和战斗信息初始化 部分
//-----------------------------------------------------------------------------------------------------------
typedef struct  // 敌人在地图上的外形
{
    FileIndex   CHFileIndexStand <format=hex>;
    FileIndex   CHFileIndexMove <format=hex>;
} EnemyAppearanceOnMap;

typedef struct  // 帧显示？具体不太清楚
{
    ushort  unknown;
    FSkip(1);
    ubyte   Amount;
    ubyte   byteArray[8];
} AppearanceSeqOnMap;   // Sequence

typedef struct
{
    while (FTell() + 0xC <= addrAddrAct + addrScenaFile)
    {
        AppearanceSeqOnMap  appearanceSeq;
    }
} AppearanceSeqOnMapGroup;

typedef struct
{
    ubyte   nothing;
    ubyte   HP_HEAL_10;
    ubyte   HP_HEAL_50;
    ubyte   EP_HEAL_10;
    ubyte   EP_HEAL_50;
    ubyte   CP_HEAL_10;
    ubyte   CP_HEAL_50;
    ubyte   SEPITH;
    ubyte   CRITICAL;
    ubyte   VANISH;
    ubyte   DEATH;
    ubyte   GUARD;
    ubyte   RUSH;
    ubyte   ARTS_GUARD; // 有图片，无效果
    ubyte   TEAMRUSH;
    ubyte   unknown;    // 没见过的图片，无效果
} ATBonusSet;

typedef struct
{
    local int count = 0;
    while (FTell() + 0x10 <= addrCharacterAppearanceSeq + addrScenaFile)    // 逆向，不准确
    {
        if (ReadUByte(FTell()) == 0x64)
        {
            ATBonusSet  ATBonus;
            continue;
        }

        for (i = 0, j = 0; i < 16; i++)
        {
            j += ReadUByte(FTell() + i);
        }

        if (j < 0xFF)
        {
            ATBonusSet  ATBonus;
            count++;
            continue;
        }
        else
        {
            break;
        }

        if (count > 0x8)
            break;
    }
} ATBonusGroup;

typedef struct  //敌方站位及朝向
{
    ubyte       x;      //左下角为00，左右为x，上下为y
    ubyte       y;
    ushort      degree; //朝上为0°，朝右为90°，以此类推。
} EnemyPosition <read=readEnemyPosition>;

string readEnemyPosition(EnemyPosition &a)
{
    string s;
    SPrintf(s,"%02d %02d %03d°", a.x, a.y, a.degree);
    return  s;
};

typedef struct
{
    EnemyPosition   enemyPosition[8] <optimize=false>;
} EnemyPositionGroup;

typedef struct
{
    ////while ((ReadUByte(FTell()) != 0) && (ReadUByte(FTell()+1) != 0) && (FTell() < addrCharacterAppearanceSeq) && (ReadUByte(FTell()) < 17))
    while (FTell() + 0x20 <= addrCharacterAppearanceSeq + addrScenaFile)    // 逆向，不准确
    {
        if (ReadUInt(FTell()+0x18) <= 0x30000000)
        {
            EnemyPositionGroup  enemyPosition;
        }
        else
        {
            break;
        }
    }
} EnemyPositionGroupAll;

// 战斗设定，对每种可能性单独设定，包括敌方及站位、BGM、AT奖励
typedef struct
{
    local int i = 0;
    for (i = 0; i < 8; i++)
        FileIndex   enemyMSFileIndex <format=hex>;

    ushort      addrEnemyPosition;      // 敌人站位及朝向，除 被偷袭 外时.
    ushort      addrEnemyPositionEA;    // 敌人站位及朝向，被偷袭 时. EA=EnemyAdvantage
    ushort      BGM_Safe;
    ushort      BGM_Danger;
    uint        addrATBonus;

    local uint  addrTemp1 = FTell();
    FSeek(addrEnemyPosition + addrScenaFile);
    EnemyPositionGroup  enemyPosition;
    FSeek(addrEnemyPositionEA + addrScenaFile);
    EnemyPositionGroup  enemyPositionEA;
    FSeek(addrATBonus + addrScenaFile);
    ATBonusSet  ATBonus;
    FSeek(addrTemp1);
} EnemyGroup;

// 战斗设定，包括敌方4种可能性、战斗参数、战斗地图、怪在地图上的等级及地图上敲掉可以获得的耀晶片
typedef struct
{
    ushort      parameter <format=hex,fgcolor=cBlue>;   // 0200 不能逃走, 1000 包含分身
    ushort      levelOnMap;
    FSkip(1);
    ubyte       detectionDistance;  // 敌方侦察距离，多远发现我方，改内存可以实现阳炎
    ubyte       distancePerMove;    // 一次移动的最大距离
    ubyte       boolCanMove;        // 能否移动，1能0不能; 不能移动时不侦察敌方
    ////uint        moveRangeOnMap <format=hex>; // 06 00 00 00 不动
    ushort      moveSpeedOnMap;
    ushort      specialEffectForParty <hidden=true>;  // 战斗教学用，02 无法使用魔法战技 03 无法使用战技 04 无效果 05 战斗开始全员cp98
    uint        addrBattleMap;

    if (addrBattleMap < FileSize())
    {
        local uint  addrTemp1 = FTell();
        FSeek(addrBattleMap + addrScenaFile);
        string   battleMap;
        FSeek(addrTemp1);
    }

    uint        addrSepithGetOnMap;
    if(addrSepithGetOnMap != 0 && addrSepithGetOnMap + addrScenaFile + 7 <= FileSize())
    {
        local uint  addrTemp2 = FTell();
        FSeek(addrSepithGetOnMap + addrScenaFile);
        ubyte   sepithGetOnMap[7];
        FSeek(addrTemp2);
    }

    ubyte       probability1;
    ubyte       probability2;
    ubyte       probability3;
    ubyte       probability4;
    if(probability1 > 0)
        EnemyGroup  enemyGroup1;
    if(probability2 > 0)
        EnemyGroup  enemyGroup2;
    if(probability3 > 0)
        EnemyGroup  enemyGroup3;
    if(probability4 > 0)
        EnemyGroup  enemyGroup4;
} BattleSet;

// 设定场景中每一个可见的敌人，每个0x20
typedef struct  // (int parEnemyArrange)
{
    local int   enemyIndex = i;
    local int   scenaCharacterIndex = 0x8 + npcAmount + enemyIndex; // 场景中角色编号，0-7为我方，然后NPC，最后怪物

    uint   xOnMap <format=hex>;    // 西东
    uint   zOnMap <format=hex>;    // 下上
    uint   yOnMap <format=hex>;    // 南北
    uint   specialAppearTime <format=hex,fgcolor=cRed>;
    // 特定情况下才会出现，通缉魔兽xx 00 85 01; 正常 00 00 01 01; 序章战斗教学xx 00 81 01
    ushort  addrBattleSet <fgcolor=cDkGreen>;  // 战场设定地址

    local uint  addrTemp1 = FTell();
    FSeek(addrBattleSet + addrScenaFile);
    BattleSet   battle;
    FSeek(addrTemp1);

    ushort  scenaAfterBattle <format=hex>;  // 必须完成才能继续剧情，测试：序章战斗教学 去掉后无法去下一场景
    ushort  characterAppearanceIndex;

    local uint  addrTemp2 = FTell();
    FSeek(addrCharacterAppearance + characterAppearanceIndex * 4 + addrScenaFile);
    EnemyAppearanceOnMap    enemyAppearance;
    FSeek(addrTemp2);

    FSkip(2);   // FF
    if (IsProcess())
    {
        int addrAppearanceSeq1;
        int addrAppearanceSeq2;
        FSeek(addrAppearanceSeq1 + addrScenaFile);
        AppearanceSeqOnMap  appearanceSeq1;
        FSeek(addrAppearanceSeq2 + addrScenaFile);
        AppearanceSeqOnMap  appearanceSeq2;
    }
    else
    {
        int addrAppearanceSeq1Index;
        int addrAppearanceSeq2Index;
        FSeek(addrAppearanceSeq1Index * 0xC + addrCharacterAppearanceSeq + addrScenaFile);
        AppearanceSeqOnMap  appearanceSeq1;
        FSeek(addrAppearanceSeq2Index * 0xC + addrCharacterAppearanceSeq + addrScenaFile);
        AppearanceSeqOnMap  appearanceSeq2;
    }
    FSeek(addrTemp2 + 0xA);
    ////int     enemyStillFrameFrequency <format=hex>;  // 帧 刷新频率？
    ////int     enemyMoveFrameFrequency <format=hex>;
/*
    if (parEnemyArrange == 2)
    {}
    else if (parEnemyArrange == 0)
    {
        byte    dummy;
        local ushort    addrBattleSet = 0;
    }
*/
} EnemyArrange <read=readEnemyArrange, write=writeEnemyArrange>;

string readEnemyArrange(EnemyArrange &a)
{
    string  temp;
    SPrintf(temp, "%02X 0x%04X", a.scenaCharacterIndex, a.addrBattleSet);
    return  temp;
}

void writeEnemyArrange(EnemyArrange &a, string s)
{
    SScanf(s, "%X", a.addrBattleSet);
    ////a.addrBattleSet = Atoi(s);
}


// 设定Event，每个0x60
typedef struct
{
    ubyte   unknown1[0x54];
    ubyte   actIndex;
    if (actIndex < actAmount)
    {
        local uint  addrTemp1 = FTell();
        FSeek(act.act[actIndex].addrActStart);
        ubyte   seeActIsWhat[act.act[actIndex].actSize];
        FSeek(addrTemp1);
    }

    ubyte   unknown2[0xB];
} EventArrange <read=readEventArrange>;

string readEventArrange(EventArrange &a)
{
    string  temp;
    SPrintf(temp, "%d", a.actIndex);
    return  temp;
}

typedef struct
{
    EventArrange    eventBT[eventAmount] <optimize=false>;
} EventArrangeGroup;

// 设定Talkment，每个0x24
typedef struct
{
    ubyte   unknown1[0x20];
    ubyte   actIndex;
    if (actIndex < actAmount)
    {
        local uint  addrTemp1 = FTell();
        FSeek(act.act[actIndex].addrActStart);
        ubyte   seeActIsWhat[act.act[actIndex].actSize];
        FSeek(addrTemp1);
    }

    ubyte   unknown2[0x3];
} TalkmentArrange <read=readTalkmentArrange>;

string readTalkmentArrange(TalkmentArrange &a)
{
    string  temp;
    SPrintf(temp, "%d", a.actIndex);
    return  temp;
}

typedef struct
{
    TalkmentArrange talkment[talkmentAmount] <optimize=false>;
} TalkmentArrangeGroup;

// NPC名字
typedef struct
{
    string  s;
} StringStruct <read=readStringStruct>;

string readStringStruct(StringStruct &a)
{
    return  a.s;
}

typedef struct
{
    StringStruct    NPCName[npcAmount+1] <optimize=false>;
} NPCNameGroup;

// 设定NPC，每个0x1C
typedef struct
{
    local int   NPCIndex = i;
    local int   scenaCharacterIndex = 0x8 + NPCIndex;   // 场景中角色编号，0-7为我方，然后NPC，最后怪物
    ubyte   unknown1[0x12];
    ubyte   characterAppearanceIndex;
    local uint  addrTemp1 = FTell();
    FSeek(addrCharacterAppearance + characterAppearanceIndex * 4 + addrScenaFile);
    FileIndex   characterAppearance <format=hex>;
    FSeek(startof(NPCName.NPCName[NPCIndex+1]));
    string  characterName;
    FSeek(addrTemp1);

    ubyte   unknown2[0x3];
    ubyte   childScenaIndex;
    ubyte   actIndex;
    if (childScenaIndex == 0 && actIndex != 0xFF)
    {
        FSeek(parentof(parentof(this)).act.act[actIndex].addrActStart);
        ubyte   seeActIsWhat[parentof(parentof(this)).act.act[actIndex].actSize];
    }
    else if (childScenaIndex == 1 && actIndex != 0xFF && exists(scenaFile1))
    {
        FSeek(scenaFile1.act.act[actIndex].addrActStart);
        ubyte   seeActIsWhat[scenaFile1.act.act[actIndex].actSize];
    }
    else if (childScenaIndex == 2 && actIndex != 0xFF && exists(scenaFile2))
    {
        FSeek(scenaFile2.act.act[actIndex].addrActStart);
        ubyte   seeActIsWhat[scenaFile2.act.act[actIndex].actSize];
    }
    else if (childScenaIndex == 3 && actIndex != 0xFF && exists(scenaFile3))
    {
        FSeek(scenaFile3.act.act[actIndex].addrActStart);
        ubyte   seeActIsWhat[scenaFile3.act.act[actIndex].actSize];
    }
    else if (childScenaIndex == 4 && actIndex != 0xFF && exists(scenaFile4))
    {
        FSeek(scenaFile4.act.act[actIndex].addrActStart);
        ubyte   seeActIsWhat[scenaFile4.act.act[actIndex].actSize];
    }
    else if (childScenaIndex == 5 && actIndex != 0xFF && exists(scenaFile5))
    {
        FSeek(scenaFile5.act.act[actIndex].addrActStart);
        ubyte   seeActIsWhat[scenaFile5.act.act[actIndex].actSize];
    }

    FSeek(addrTemp1 + 5);
    ubyte   unknown3[0x4];
} NPCArrange <read=readNPCArrange, write=writeNPCArrange>;

string readNPCArrange(NPCArrange &a)
{
    string  temp;
    SPrintf(temp, "%02X ", a.scenaCharacterIndex);
    return  temp + a.characterName;
}

void writeNPCArrange(NPCArrange &a, string s)
{
    // 复制用
}

typedef struct
{
    for (i = 0; i < npcAmount; i++)
    {
        NPCArrange  NPC;
    }
} NPCArrangeGroup;

// 设定act
typedef struct(int parACT)  // 参数为序号
{
    local int   actIndex = parACT;
    local int   actSize = 1;
    local uint  addrActStart = addrAct[actIndex] + addrScenaFile;

    FSeek(actIndex*4 + addrAddrAct + addrScenaFile);
    uint        addrActDup;

    if (actIndex < actAmount -1 && addrAct[actIndex+1] > addrAct[actIndex])
    {
        actSize = addrAct[actIndex+1] - addrAct[actIndex];
    }
    else if (actIndex == actAmount -1 && addrNPCName > addrAct[actIndex])
    {
        actSize = addrNPCName - addrAct[actIndex];
    }

    FSeek(addrActStart);
    ubyte   actArea[actSize];

} ACT;

typedef struct
{
    for (i = 0; i < actAmount; i++)
    {
        FSeek(addrAct[i] + addrScenaFile);
        ACT act(i);
    }
} ACTGroup;

typedef struct(uint parScenaFile) // 参数为开始地址，重名参数优先
{
    local uint      addrScenaFile = parScenaFile;   // 参数无法传给子结构
    local int       i = 0, j = 0, battleAmount = 0;
    local uint      addrEnemyArrangeStart = 0;

    FSeek(ReadUInt(0x34 + addrScenaFile) + addrScenaFile);
    string          scenaFileName;
    if (exists(miscs.placeTable))
    {
        FSeek(miscs.placeTable.place[ReadUShort(0x14 + addrScenaFile)].addrPlaceNameReal);
        string      scenaName;
    }
    else if (exists(placeTable))
    {
        FSeek(startof(placeTable.place[ReadUShort(0x14 + addrScenaFile)]));
        string      scenaName;
    }
    else
    {
        local string    scenaName = "";
    }

    FSeek(addrScenaFile);
    string          scenaFileName1;
    FSeek(0xA + addrScenaFile);
    string          scenaFileName2 <hidden=true>;
    FSeek(0x14 + addrScenaFile);
    ushort          scenaNameIndex;
    ushort          scenaBGMIndex;
    uint            flag <format=hex>;
    //FSeek(0x1C + addrScenaFile);
    uint            childScenaCode[6] <format=hex>;
    FSeek(0x34 + addrScenaFile);
    uint            addrNPCName;
    ushort          addrCharacterAppearance;   // 角色外形
    ushort          addrNPCArrange;     // NPC布置
    ushort          addrEnemyArrange;   // 敌人在地图上的布置
    ushort          addrEventArrange;   // 事件，多与战斗关联
    ushort          addrTalkmentArrange;// 可交谈 物布置
    ushort          addrAddrAct;        // 剧情、对话地址数组
    ushort          ActTotalSize;
    local int       actAmount = ActTotalSize / 4;
    ushort          addrCharacterAppearanceSeq;
    FSeek(0x4C + addrScenaFile);
    ubyte           appearanceAmount;   // 质疑，内存中会变化
    ubyte           npcAmount;
    ubyte           enemyAmount;    // 场景中可见敌人 数量
    ubyte           eventAmount;    // push 00B3D2F8 EventStart Actor %d Scp %d Func %d
    ubyte           talkmentAmount;
    FSeek(0x53 + addrScenaFile);
    ubyte           PreInitFunction;

    FSeek(0x90 + addrScenaFile);
    BYTEXY          InitFunction;
    BYTEXY          ReinitFunction;

    if (actAmount > 0)
    {
        FSeek(addrAddrAct + addrScenaFile);
        uint        addrAct[actAmount];
        ACTGroup    act;
    }

    if (npcAmount > 0)
    {
        FSeek(addrNPCName + addrScenaFile);
        NPCNameGroup    NPCName <hidden=true>;
    }

    if (eventAmount > 0)
    {
        FSeek(addrEventArrange + addrScenaFile);
        EventArrangeGroup   eventBT;
    }

    if (talkmentAmount > 0)
    {
        FSeek(addrTalkmentArrange + addrScenaFile);
        TalkmentArrangeGroup    talkment;
    }

    if (addrCharacterAppearance < addrNPCArrange && appearanceAmount > 0)
    {
        FSeek(addrCharacterAppearance + addrScenaFile);
        FileIndex   appearance[(addrNPCArrange-addrCharacterAppearance)/4] <format=hex>;
        ////FileIndex   appearance[appearanceAmount] <format=hex>;
    }

    if (npcAmount > 0)
    {
        FSeek(addrNPCArrange + addrScenaFile);
        NPCArrangeGroup NPC;
    }

    FSeek(addrEnemyArrange + addrScenaFile);
    if (enemyAmount > 0)
    {
/*      for (i = 0; i < 8 + npcAmount; i++)
        {
            addrEnemyArrangeStart = FTell();
            FSeek(addrScenaFile);
            EnemyArrange    enemyArrange(0) <hidden=true>;
            FSeek(addrEnemyArrangeStart);
        }
*/
        for (i = 0; i < enemyAmount; i++)
        {
            EnemyArrange    enemyArrange;
        }
        ////EnemyArrange    enemyArrange[enemyAmount] <optimize=false>;
    }
/*
    if (FTell() + 0x10 <= addrCharacterAppearanceSeq + addrScenaFile)
    {
        ATBonusGroup            ATBonus;
    }

    if (FTell() + 0x20 <= addrCharacterAppearanceSeq + addrScenaFile)
    {
        EnemyPositionGroupAll   enemyPosition;
    }

    while (FTell() + 0x18 <= addrCharacterAppearanceSeq + addrScenaFile)
    {
        BattleSet   battle;
        battleAmount++;
    }

    if (addrCharacterAppearanceSeq + 0xC <= addrAddrAct)
    {
        FSeek(addrCharacterAppearanceSeq + addrScenaFile);
        AppearanceSeqOnMap  appearanceSeq[(addrAddrAct - addrCharacterAppearanceSeq) / 0xC] <optimize=true>;
    }
*/
    FSeek(addrNPCName + addrScenaFile);
} ScenaFile <read=readScenaFile>;

string readScenaFile(ScenaFile &a)
{
    return  "场景" + " " + a.scenaFileName + " " + a.scenaName;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 场景中每个角色信息
typedef struct (int parStruct)  // ZERO 0x18C
                                // AO 0x188
{
    local uint  addrScenaCharacterInf1 = FTell();
    local uint  index = parStruct;

    FSeek(0x80 + addrScenaCharacterInf1);
    float       position_now[4];

    FSeek(0xA0 + addrScenaCharacterInf1);
    float       scale[3];
    FSeek(0xE0 + addrScenaCharacterInf1);
    string      characterName;
    FSeek(0x110 + addrScenaCharacterInf1);
    ubyte        correction[0x10];
    float       degree;

#ifdef ED_ZERO
    FSeek(0x16C + addrScenaCharacterInf1);
#else
#ifdef ED_AO
    FSeek(0x168 + addrScenaCharacterInf1);
#endif
#endif
    ushort      appearancePar;  // 外形参数，天使羊不可见or 0x80

    FSeek(0x16E + addrScenaCharacterInf1);
    ushort      ChrId;

#ifdef ED_ZERO
    FSeek(0x178 + addrScenaCharacterInf1);
 #else
#ifdef ED_AO
    FSeek(0x174 + addrScenaCharacterInf1);
#endif
#endif
    FileIndex   CHFileIndexStand <format=hex>;
    FileIndex   CHFileIndexMove <format=hex>;
    ushort      CHFileNoStand;
    ushort      CHFileNoMove;

#ifdef ED_ZERO
    FSeek(0x18C + addrScenaCharacterInf1);
#else
#ifdef ED_AO
    FSeek(0x188 + addrScenaCharacterInf1);
#endif
#endif
} ScenaCharacterInf1 <read=readScenaCharacterInf1, write=writeScenaCharacterInf1>;

string readScenaCharacterInf1(ScenaCharacterInf1 &a)
{
    return a.characterName;
}

void writeScenaCharacterInf1(ScenaCharacterInf1 &a, string s) {;}


typedef struct
{
    local int i = 0;
    for (i = 0; i < scenaCharacterAmount; i++)
    {
        ScenaCharacterInf1  scenaCharacterInf1(i);
    }
} ScenaCharacterInf1Group;

if (IsProcess())
{
    // 具体化的角色信息，1包括外形，2包括怪的战斗参数
    if (addrMemScenaStart1 != 0 && scenaCharacterAmount != 0)
    {
        FSeek(addrScenaStart1);
        ScenaCharacterInf1Group     scenaCharacterInf1;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct // 0xA0
{
    local int64 addrStart = FTell();
    USHORT  State;
    BYTE    ScenaIndex;

    DUMMY_STRUCT(1);

    ULONG   FunctionOffset;
    ULONG   CurrentOffset;

    FSeek(0x34 + addrStart);
    ushort  ChrIndex;
    ushort  ChrIndex;

    FSeek(0x40 + addrStart);
    float   position_now[4];
    float   position_target[4];
    float   float_60[2];

    int     distanceToWalk;
    int     distanceWalked;

    FSeek(0x90 + addrStart);
    BYTE    function;

    FSeek(0xA0 + addrStart);
} SCENA_ENV_BLOCK <read=readSCENA_ENV_BLOCK, write=writeSCENA_ENV_BLOCK>;

string readSCENA_ENV_BLOCK(SCENA_ENV_BLOCK &a)
{
    string s;
    if (a.State)
        SPrintf(s, "%d %d", a.State, a.function);
    else
        s = "";
    return s;
}

void writeSCENA_ENV_BLOCK(SCENA_ENV_BLOCK &a, string s) {;}


typedef struct (int parStruct) // 每个0x2BC
{
    local uint  addrScenaCharacterInf2 = FTell();
    local int   index = parStruct;
    local int i;

    FSeek(0x14 + addrScenaCharacterInf2);
    float       float_14;

    FSeek(0x1C + addrScenaCharacterInf2);
    for (i=0; i<4; i++)
        SCENA_ENV_BLOCK thread;

    FSeek(0x29C + addrScenaCharacterInf2);
    ushort      battleParameterDup <format=hex,fgcolor=cBlue>;   // 0200 不能逃走, 1000 包含分身
    ushort      levelOnMap;
    FSkip(1);
    ubyte       detectionDistance;  // 敌方侦察距离，多远发现我方，改内存可以实现阳炎
    ubyte       distancePerMove;    // 一次移动的最大距离
    ubyte       boolCanMove;        // 能否移动，1能0不能; 不能移动时不侦察敌方
    ushort      moveSpeedOnMap;
    ushort      specialEffectForParty <hidden=true>;  // 战斗教学用，02 无法使用魔法战技 03 无法使用战技 04 无效果 05 战斗开始全员cp98
    uint        addrBattleMap;
    uint        addrSepithGetOnMap;
    ubyte       probability1;
    ubyte       probability2;
    ubyte       probability3;
    ubyte       probability4;
    FSeek(0x2BC + addrScenaCharacterInf2);
} ScenaCharacterInf2 <read=readScenaCharacterInf2, write=writeScenaCharacterInf2>;

string readScenaCharacterInf2(ScenaCharacterInf2 &a)
{
    string s, strTemp;
    int i;
    for (i=0; i<4; i++)
    {
        if (a.thread[i].State == 0)
        {
            s += " -";
        }
        else
        {
            SPrintf(strTemp, " %d", a.thread[i].function);
            s += strTemp;
        }

    }

    return scenaCharacterInf1.scenaCharacterInf1[a.index].characterName + s;
}

void writeScenaCharacterInf2(ScenaCharacterInf2 &a, string s) {;}

typedef struct
{
    local int i = 0;
    for (i = 0; i < scenaCharacterAmount; i++)
    {
        ScenaCharacterInf2  scenaCharacterInf2(i);
    }
} ScenaCharacterInf2Group;

if (IsProcess())
{
    if (addrMemScenaStart2 != 0 && scenaCharacterAmount != 0)
    {
        FSeek(addrScenaStart2);
        ScenaCharacterInf2Group     scenaCharacterInf2;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 战斗开始前生成的战斗信息，
typedef struct
{
    local uint  addrBattleInitStart = FTell();
    ushort      parameter <format=hex,fgcolor=cBlue>;   // 0200 不能逃走, 1000 包含分身
    ushort      levelOnMap;
    FSkip(1);
    ubyte       detectionDistance;  // 敌方侦察距离，多远发现我方，改内存可以实现阳炎
    ubyte       distancePerMove;    // 一次移动的最大距离
    ubyte       boolCanMove;        // 能否移动，1能0不能; 不能移动时不侦察敌方
    ushort      moveSpeedOnMap;
    ushort      specialEffectForParty <hidden=true>;  // 战斗教学用，02 无法使用魔法战技 03 无法使用战技 04 无效果 05 战斗开始全员cp98
    uint        addrBattleMap;
    uint        addrSepithGetOnMap;
    ubyte       probability1;
    ubyte       probability2;
    ubyte       probability3;
    ubyte       probability4;
    ATBonusSet  ATBonus;
    EnemyPositionGroup  enemyPosition;
    FileIndex   enemyMSFileIndex[8] <format=hex>;
    ushort      addrEnemyPosition;      // 敌人站位及朝向，除 被偷袭 外时.
    ushort      addrEnemyPositionEA;    // 敌人站位及朝向，被偷袭 时. EA=EnemyAdvantage
    ushort      BGM_Safe;
    ushort      BGM_Danger;
    uint        addrATBonus;
    string      battleMap;
    FSeek(addrBattleInitStart + 0x80);
} BattleInit;

typedef struct
{
    FSkip(1);
    ubyte       ChainedBattleAmount;    // 几连战
    FSkip(6);
    BattleInit  battle[ChainedBattleAmount] <optimize=false>;
} BattleInitGroup <read=readBattleInitGroup>;

string readBattleInitGroup(BattleInitGroup &a)
{
    return  "战斗初始化信息";
}

if (!IsProcess() && FTell() == 0)
{
    ScenaFile   scenaFile(0) <open=true>;
}
