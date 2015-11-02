//--------------------------------------
//--- 010 Editor v3.2 Binary Template
//
// File: zero_ao_scena.bt.h
// Author:acgngca
// Revision:2011-11-22
// Purpose:
// Readme:
//
//--------------------------------------

#include "ED6_common.bt.h"

// �����ļ���ս����Ϣ��ʼ�� ����
//-----------------------------------------------------------------------------------------------------------
typedef struct  // �����ڵ�ͼ�ϵ�����
{
    FileIndex   CHFileIndexStand <format=hex>;
    FileIndex   CHFileIndexMove <format=hex>;
} EnemyAppearanceOnMap;

typedef struct  // ֡��ʾ�����岻̫���
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
    ubyte   ARTS_GUARD; // ��ͼƬ����Ч��
    ubyte   TEAMRUSH;
    ubyte   unknown;    // û������ͼƬ����Ч��
} ATBonusSet;

typedef struct
{
    local int count = 0;
    while (FTell() + 0x10 <= addrCharacterAppearanceSeq + addrScenaFile)    // ���򣬲�׼ȷ
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

typedef struct  //�з�վλ������
{
    ubyte       x;      //���½�Ϊ00������Ϊx������Ϊy
    ubyte       y;
    ushort      degree; //����Ϊ0�㣬����Ϊ90�㣬�Դ����ơ�
} EnemyPosition <read=readEnemyPosition>;

string readEnemyPosition(EnemyPosition &a)
{
    string s;
    SPrintf(s,"%02d %02d %03d��", a.x, a.y, a.degree);
    return  s;
};

typedef struct
{
    EnemyPosition   enemyPosition[8] <optimize=false>;
} EnemyPositionGroup;

typedef struct
{
    ////while ((ReadUByte(FTell()) != 0) && (ReadUByte(FTell()+1) != 0) && (FTell() < addrCharacterAppearanceSeq) && (ReadUByte(FTell()) < 17))
    while (FTell() + 0x20 <= addrCharacterAppearanceSeq + addrScenaFile)    // ���򣬲�׼ȷ
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

// ս���趨����ÿ�ֿ����Ե����趨�������з���վλ��BGM��AT����
typedef struct
{
    local int i = 0;
    for (i = 0; i < 8; i++)
        FileIndex   enemyMSFileIndex <format=hex>;

    ushort      addrEnemyPosition;      // ����վλ�����򣬳� ��͵Ϯ ��ʱ.
    ushort      addrEnemyPositionEA;    // ����վλ�����򣬱�͵Ϯ ʱ. EA=EnemyAdvantage
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

// ս���趨�������з�4�ֿ����ԡ�ս��������ս����ͼ�����ڵ�ͼ�ϵĵȼ�����ͼ���õ����Ի�õ�ҫ��Ƭ
typedef struct
{
    ushort      parameter <format=hex,fgcolor=cBlue>;   // 0200 ��������, 1000 ��������
    ushort      levelOnMap;
    FSkip(1);
    ubyte       detectionDistance;  // �з������룬��Զ�����ҷ������ڴ����ʵ������
    ubyte       distancePerMove;    // һ���ƶ���������
    ubyte       boolCanMove;        // �ܷ��ƶ���1��0����; �����ƶ�ʱ�����з�
    ////uint        moveRangeOnMap <format=hex>; // 06 00 00 00 ����
    ushort      moveSpeedOnMap;
    ushort      specialEffectForParty <hidden=true>;  // ս����ѧ�ã�02 �޷�ʹ��ħ��ս�� 03 �޷�ʹ��ս�� 04 ��Ч�� 05 ս����ʼȫԱcp98
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

// �趨������ÿһ���ɼ��ĵ��ˣ�ÿ��0x20
typedef struct  // (int parEnemyArrange)
{
    local int   enemyIndex = i;
    local int   scenaCharacterIndex = 0x8 + npcAmount + enemyIndex; // �����н�ɫ��ţ�0-7Ϊ�ҷ���Ȼ��NPC��������

    uint   xOnMap <format=hex>;    // ����
    uint   zOnMap <format=hex>;    // ����
    uint   yOnMap <format=hex>;    // �ϱ�
    uint   specialAppearTime <format=hex,fgcolor=cRed>;
    // �ض�����²Ż���֣�ͨ��ħ��xx 00 85 01; ���� 00 00 01 01; ����ս����ѧxx 00 81 01
    ushort  addrBattleSet <fgcolor=cDkGreen>;  // ս���趨��ַ

    local uint  addrTemp1 = FTell();
    FSeek(addrBattleSet + addrScenaFile);
    BattleSet   battle;
    FSeek(addrTemp1);

    ushort  scenaAfterBattle <format=hex>;  // ������ɲ��ܼ������飬���ԣ�����ս����ѧ ȥ�����޷�ȥ��һ����
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
    ////int     enemyStillFrameFrequency <format=hex>;  // ֡ ˢ��Ƶ�ʣ�
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


// �趨Event��ÿ��0x60
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

// �趨Talkment��ÿ��0x24
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

// NPC����
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

// �趨NPC��ÿ��0x1C
typedef struct
{
    local int   NPCIndex = i;
    local int   scenaCharacterIndex = 0x8 + NPCIndex;   // �����н�ɫ��ţ�0-7Ϊ�ҷ���Ȼ��NPC��������
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
    // ������
}

typedef struct
{
    for (i = 0; i < npcAmount; i++)
    {
        NPCArrange  NPC;
    }
} NPCArrangeGroup;

// �趨act
typedef struct(int parACT)  // ����Ϊ���
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

typedef struct(uint parScenaFile) // ����Ϊ��ʼ��ַ��������������
{
    local uint      addrScenaFile = parScenaFile;   // �����޷������ӽṹ
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
    ushort          addrCharacterAppearance;   // ��ɫ����
    ushort          addrNPCArrange;     // NPC����
    ushort          addrEnemyArrange;   // �����ڵ�ͼ�ϵĲ���
    ushort          addrEventArrange;   // �¼�������ս������
    ushort          addrTalkmentArrange;// �ɽ�̸ �ﲼ��
    ushort          addrAddrAct;        // ���顢�Ի���ַ����
    ushort          ActTotalSize;
    local int       actAmount = ActTotalSize / 4;
    ushort          addrCharacterAppearanceSeq;
    FSeek(0x4C + addrScenaFile);
    ubyte           appearanceAmount;   // ���ɣ��ڴ��л�仯
    ubyte           npcAmount;
    ubyte           enemyAmount;    // �����пɼ����� ����
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
    return  "����" + " " + a.scenaFileName + " " + a.scenaName;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ÿ����ɫ��Ϣ
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
    ushort      appearancePar;  // ���β�������ʹ�򲻿ɼ�or 0x80

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
    // ���廯�Ľ�ɫ��Ϣ��1�������Σ�2�����ֵ�ս������
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


typedef struct (int parStruct) // ÿ��0x2BC
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
    ushort      battleParameterDup <format=hex,fgcolor=cBlue>;   // 0200 ��������, 1000 ��������
    ushort      levelOnMap;
    FSkip(1);
    ubyte       detectionDistance;  // �з������룬��Զ�����ҷ������ڴ����ʵ������
    ubyte       distancePerMove;    // һ���ƶ���������
    ubyte       boolCanMove;        // �ܷ��ƶ���1��0����; �����ƶ�ʱ�����з�
    ushort      moveSpeedOnMap;
    ushort      specialEffectForParty <hidden=true>;  // ս����ѧ�ã�02 �޷�ʹ��ħ��ս�� 03 �޷�ʹ��ս�� 04 ��Ч�� 05 ս����ʼȫԱcp98
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
// ս����ʼǰ���ɵ�ս����Ϣ��
typedef struct
{
    local uint  addrBattleInitStart = FTell();
    ushort      parameter <format=hex,fgcolor=cBlue>;   // 0200 ��������, 1000 ��������
    ushort      levelOnMap;
    FSkip(1);
    ubyte       detectionDistance;  // �з������룬��Զ�����ҷ������ڴ����ʵ������
    ubyte       distancePerMove;    // һ���ƶ���������
    ubyte       boolCanMove;        // �ܷ��ƶ���1��0����; �����ƶ�ʱ�����з�
    ushort      moveSpeedOnMap;
    ushort      specialEffectForParty <hidden=true>;  // ս����ѧ�ã�02 �޷�ʹ��ħ��ս�� 03 �޷�ʹ��ս�� 04 ��Ч�� 05 ս����ʼȫԱcp98
    uint        addrBattleMap;
    uint        addrSepithGetOnMap;
    ubyte       probability1;
    ubyte       probability2;
    ubyte       probability3;
    ubyte       probability4;
    ATBonusSet  ATBonus;
    EnemyPositionGroup  enemyPosition;
    FileIndex   enemyMSFileIndex[8] <format=hex>;
    ushort      addrEnemyPosition;      // ����վλ�����򣬳� ��͵Ϯ ��ʱ.
    ushort      addrEnemyPositionEA;    // ����վλ�����򣬱�͵Ϯ ʱ. EA=EnemyAdvantage
    ushort      BGM_Safe;
    ushort      BGM_Danger;
    uint        addrATBonus;
    string      battleMap;
    FSeek(addrBattleInitStart + 0x80);
} BattleInit;

typedef struct
{
    FSkip(1);
    ubyte       ChainedBattleAmount;    // ����ս
    FSkip(6);
    BattleInit  battle[ChainedBattleAmount] <optimize=false>;
} BattleInitGroup <read=readBattleInitGroup>;

string readBattleInitGroup(BattleInitGroup &a)
{
    return  "ս����ʼ����Ϣ";
}