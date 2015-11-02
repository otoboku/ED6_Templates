//--------------------------------------
//--- 010 Editor v6.0.1 Binary Template
//
// File: 010_def.bt.h
// Author: acgngca
// Revision:
// Purpose:
//--------------------------------------
#ifndef _010_def_bt_h_1AFFED7B_3480_447E_94A9_8BAAD8B032A1
#define _010_def_bt_h_1AFFED7B_3480_447E_94A9_8BAAD8B032A1

//local string    ss <hidden=true>;
#define ERROR_SUCCESS       0
#define ERROR_NULLPTR       1
#define ERROR_SEEK_ERROR    2

/*******************************************************************************
    common types
*******************************************************************************/

// typedef $1\t\t$1\t\t<open=suppress>;\r\n
typedef char        char        <open=suppress>;
typedef byte        byte        <open=suppress>;
typedef CHAR        CHAR        <open=suppress>;
typedef BYTE        BYTE        <open=suppress>;

typedef uchar       uchar       <open=suppress>;
typedef ubyte       ubyte       <open=suppress>;
typedef UCHAR       UCHAR       <open=suppress>;
typedef UBYTE       UBYTE       <open=suppress>;

typedef short       short       <open=suppress>;
typedef int16       int16       <open=suppress>;
typedef SHORT       SHORT       <open=suppress>;
typedef INT16       INT16       <open=suppress>;

typedef ushort      ushort      <open=suppress>;
typedef uint16      uint16      <open=suppress>;
typedef USHORT      USHORT      <open=suppress>;
typedef UINT16      UINT16      <open=suppress>;
typedef WORD        WORD        <open=suppress>;

typedef int         int         <open=suppress>;
typedef int32       int32       <open=suppress>;
typedef long        long        <open=suppress>;
typedef INT         INT         <open=suppress>;
typedef INT32       INT32       <open=suppress>;
typedef LONG        LONG        <open=suppress>;

typedef uint        uint        <open=suppress>;
typedef uint32      uint32      <open=suppress>;
typedef ulong       ulong       <open=suppress>;
typedef UINT        UINT        <open=suppress>;
typedef UINT32      UINT32      <open=suppress>;
typedef ULONG       ULONG       <open=suppress>;
typedef DWORD       DWORD       <open=suppress>;

typedef int64       int64       <open=suppress>;
typedef quad        quad        <open=suppress>;
typedef QUAD        QUAD        <open=suppress>;
typedef INT64       INT64       <open=suppress>;
typedef __int64     __int64     <open=suppress>;

typedef uint64      uint64      <open=suppress>;
typedef uquad       uquad       <open=suppress>;
typedef UQUAD       UQUAD       <open=suppress>;
typedef UINT64      UINT64      <open=suppress>;
typedef QWORD       QWORD       <open=suppress>;
typedef __uint64    __uint64    <open=suppress>;

typedef float       float       <open=suppress>;
typedef FLOAT       FLOAT       <open=suppress>;

typedef double      double      <open=suppress>;
typedef DOUBLE      DOUBLE      <open=suppress>;

typedef hfloat      hfloat      <open=suppress>;
typedef HFLOAT      HFLOAT      <open=suppress>;


typedef string      string      <open=suppress>;
typedef wstring     wstring     <open=suppress>;
typedef wchar_t     wchar_t     <open=suppress>;

typedef UINT32      PTR32       <format=hex, fgcolor=cPurple>;
typedef UINT64      PTR64       <format=hex, fgcolor=cPurple>;

/**
* from wincon.h
*
* @name:    COORD
* @member:  SHORT           X;
* @member:  SHORT           Y;
*/
typedef struct _COORD {
    SHORT X;
    SHORT Y;
} COORD <read=readCOORD, write=writeCOORD, open=suppress>;

string readCOORD(COORD &a)
{
    string  str;
    SPrintf(str, "%d %d", a.X, a.Y);
    return  str;
}

void writeCOORD(COORD &a, string s) {};

/**
* char[2]
*
* @name:    BYTEXY
* @member:  CHAR            X;
* @member:  CHAR            Y;
*/
typedef struct _BYTEXY {
    CHAR X;
    CHAR Y;
} BYTEXY <read=readBYTEXY, write=writeBYTEXY, open=suppress>;

string readBYTEXY(BYTEXY &a)
{
    string  str;
    SPrintf(str, "%d %d", a.X, a.Y);
    return  str;
}

void writeBYTEXY(BYTEXY &a, string s) {};

/**
* 字节数组，顺序显示用
*
* @name:    ArrayBytes
* @member:  UCHAR[]         data;
*/
#ifndef TypeArrayBytes
#define TypeArrayBytes
typedef struct(UINT parCount)   // 字节数组，顺序显示用
{
    UCHAR           data[parCount] <open=suppress>;
} ArrayBytes <read=readArrayBytes, write=writeArrayBytes, optimize=false, open=suppress>;

string readArrayBytes(ArrayBytes &a)
{
    if (sizeof(a) == 0)
    {
        return "";
    }
    local uint      count = sizeof(a) / sizeof(a.data[0]);
    local uint      i;
    local string    str;
    local string    stmp;

    SPrintf(str, "%02X", a.data[0]);
    for (i = 1; i < count; ++i)
    {
        SPrintf(stmp, " %02X", a.data[i]);
        str += stmp;
    }
    return  str;
}

void writeArrayBytes(ArrayBytes &a, string s) {};

#define ReadArrayBytes readArrayBytes
#endif

/**
* String[]
*
* @name:    String
* @member:  string          str;
*/
#ifndef TypeString
#define TypeString
typedef struct _String
{
    //local ULONG     addr = FTell();
    string          str;
} String <read=readString, write=writeString, optimize=false, open=suppress>;

string readString(String &a)
{
    return a.str;
}

void writeString(String &a, string s)
{
    a.str = s;
}
#endif

/*******************************************************************************
    common function
*******************************************************************************/
int64 GetCursorPosEx()
{
    if(GetSelSize() > 0)
        return GetSelStart();
    else
        return GetCursorPos();
}

#define DUMMY_STRUCT    FSkip
/*
int DUMMY_STRUCT(int64 size)
{
    return FSkip(size);
}*/


/*******************************************************************************
    file offset to ... struct
*******************************************************************************/

/**
* file half pointer to string
*
* @name:    FHPSTR
* @member:  USHORT          offset;
* @member:  string          str;
* @require: addrFileBase
* @comment: exists(this._error_) when error
*/
typedef struct
{
    USHORT          offset;
    // avoid using extra local variable
    if (exists(addrFileBase))
    {
        if (FSeek(offset + addrFileBase) != -1 && !FEof())
        {
            string          str;
        }
    }
    else
    {
        if (FSeek(offset + startof(parentof(parentof(this)))) != -1 && !FEof())
        {
            string          str;
        }
    }

    if(!exists(this.str))
    {
        local string    str = "err";
        local char      _error_;
    }
    FSeek(startof(this) + sizeof(offset));
} FHPSTR <read=readFHPSTR, write=writeFHPSTR>;

string readFHPSTR(FHPSTR &a)
{
    return a.str;
}

void writeFHPSTR(FHPSTR &a, string s) {};

/*******************************************************************************
    process help function
*******************************************************************************/
int64 getPtrValue0H(int64 pos)
{
    return ReadUInt(pos);
}

int64 getPtrValueHH(int64 pos)
{
    return ReadUInt(ProcessHeapToLocalAddress(pos));
}

int64 getPtrValue00(int64 pos)
{
    return ProcessHeapToLocalAddress(ReadUInt(pos));
}

int64 getPtrValueH0(int64 pos)
{
    return ProcessHeapToLocalAddress(ReadUInt(ProcessHeapToLocalAddress(pos)));
}

int FSeekHeap(int64 pos)
{
    return FSeek(ProcessHeapToLocalAddress(pos));
}

// (\w+) Read(\w+).*
// $1\tReadHeap$2\t\(int64 pos\) { return Read$2\t\(ProcessHeapToLocalAddress\(pos\)\); }
char    ReadHeapByte    (int64 pos) { return ReadByte   (ProcessHeapToLocalAddress(pos)); }
double  ReadHeapDouble  (int64 pos) { return ReadDouble (ProcessHeapToLocalAddress(pos)); }
float   ReadHeapFloat   (int64 pos) { return ReadFloat  (ProcessHeapToLocalAddress(pos)); }
hfloat  ReadHeapHFloat  (int64 pos) { return ReadHFloat (ProcessHeapToLocalAddress(pos)); }
int     ReadHeapInt     (int64 pos) { return ReadInt    (ProcessHeapToLocalAddress(pos)); }
int64   ReadHeapInt64   (int64 pos) { return ReadInt64  (ProcessHeapToLocalAddress(pos)); }
int64   ReadHeapQuad    (int64 pos) { return ReadQuad   (ProcessHeapToLocalAddress(pos)); }
short   ReadHeapShort   (int64 pos) { return ReadShort  (ProcessHeapToLocalAddress(pos)); }
uchar   ReadHeapUByte   (int64 pos) { return ReadUByte  (ProcessHeapToLocalAddress(pos)); }
uint    ReadHeapUInt    (int64 pos) { return ReadUInt   (ProcessHeapToLocalAddress(pos)); }
uint64  ReadHeapUInt64  (int64 pos) { return ReadUInt64 (ProcessHeapToLocalAddress(pos)); }
uint64  ReadHeapUQuad   (int64 pos) { return ReadUQuad  (ProcessHeapToLocalAddress(pos)); }
ushort  ReadHeapUShort  (int64 pos) { return ReadUShort (ProcessHeapToLocalAddress(pos)); }

/*******************************************************************************
    type pointer
*******************************************************************************/

/**
* pointer to string in process
*
* @name:    PCHAR
* @member:  ULONG           addr;
* @member:  string          str;
* @require:
* @comment: exists(this._error_) when error
*/
typedef struct
{
    PTR32           addr;
    if (FSeekHeap(addr) != -1)
    {
        string      str;
    }
    else
    {
        local string    str = "err";
        local char      _error_;
    }
    FSeek(startof(this) + sizeof(addr));
} PCHAR <read=readPCHAR, write=writePCHAR>;

string readPCHAR(PCHAR &a)
{
    return a.str;
}

void writePCHAR(PCHAR &a, string s) {};


/*******************************************************************************
    archive
*******************************************************************************/

/* 内存貌似不会自动回收，用string没问题
void BytesToHexStr(CHAR dst[], UCHAR src[], uint size)
{
    if (size == 0)
    {
        return;
    }
    const char MAP[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    local uint i = 0, j = 0;
    for (i; i != size - 1; ++i)
    {
        dst[j++] = MAP[(src[i] >> 4) & 0xF];
        dst[j++] = MAP[(src[i]) & 0xF];
        dst[j++] = ' ';
    }
    dst[j++] = MAP[(src[i] >> 4) & 0xF];
    dst[j++] = MAP[(src[i]) & 0xF];
    dst[j] = '\0';
}*/


#endif