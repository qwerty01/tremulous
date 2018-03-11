// Unit tests for MSG_ReadString, MSG_ReadBigString and MSG_ReadStringLine
// Victor Roemer (wtfbbqhax), <victor@badsec.org>.

// 
// Messages
//
//
//  Out Of Band: 
//     
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |              -1               |           RAW DATA          ...
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//
//  Sequence Number (s32)
//  QPort (u16):
//  
//

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define PARANOID
#include "qcommon/cvar.h"
#include "qcommon/huffman.h"
#include "qcommon/msg.h"
#include "qcommon/net.h"

using namespace std;

cvar_t *cl_shownet = nullptr;

void Com_Error( int code, const char *fmt, ... ) { exit(1); }
void Com_Printf( const char *fmt, ... ) { }

TEST_CASE("full message - MSG_ReadString")
{
    byte msg_buf[MAX_STRING_CHARS];
    msg_t msg;

    char str[MAX_STRING_CHARS];
    memset(&str, 'A', sizeof(str));
    str[sizeof(str)-1] = '\0';

    MSG_Init(&msg, msg_buf, sizeof(msg_buf));
    MSG_WriteString2(&msg, str, sizeof(msg_buf));

    MSG_BeginReading(&msg);
    string ret { MSG_ReadString(&msg) };

    REQUIRE(ret == str);
}

TEST_CASE("Read/Write OOB string")
{
    byte msg_buf[MAX_MSGLEN];
    msg_t msg;
    
    MSG_InitOOB(&msg, msg_buf, sizeof(msg_buf));
    MSG_WriteString(&msg, "Hello world!");

    // OOB messages are not compressed
    REQUIRE(!strcmp((const char*)msg.data, "Hello world!"));

    MSG_BeginReadingOOB(&msg);
    string ret { MSG_ReadStringLine(&msg) };

    REQUIRE(ret == "Hello world!");
}

TEST_CASE("Read/Write string")
{
    byte msg_buf[MAX_MSGLEN];
    msg_t msg;
    
    MSG_Init(&msg, msg_buf, sizeof(msg_buf));
    MSG_WriteString(&msg, "Hello world!");

    MSG_BeginReading(&msg);
    string ret { MSG_ReadStringLine(&msg) };

    REQUIRE(ret == "Hello world!");
}
