#pragma once
#include "common.h"

#define ROOM_NUMBER_MIN 10000
#define ROOM_NUMBER_MAX 99999

#define ROOM_PLAYER_MAX 10
#define ROOM_PLAYER_MIN 5

#define PLAYER_NICK_MAXLEN 32
#define PLAYER_AVATAR_MAXLEN 32
#define ROOM_PASSWORD_MAXLEN 32

// Role definition
#define ROLE_MERLIN   1 // ÷��
#define ROLE_PERCIVAL 2 // ����ά��
#define ROLE_ASSASSIN 3 // �̿�
#define ROLE_MORDRED  4 // Ī���׵�
#define ROLE_OBERON   5 // �²���
#define ROLE_MORGANA  6 // Ī����
#define ROLE_LOYALIST 7 // ��ɪ���ҳ�
#define ROLE_MINIONS  8 // Ī���׵µ�צ��

#define ENABLE_FAIRY_THRESHOLD 7 // fairy will be enabled when player >= ENABLE_FAIRY_THRESHOLD

typedef struct _CONNECTION_INFO CONNECTION_INFO, * PCONNECTION_INFO;

typedef struct _PLAYER_INFO
{
    PCONNECTION_INFO pConnInfo;
    UINT GameID;
    BOOL bIsRoomOwner;
    char NickName[PLAYER_NICK_MAXLEN + 1];
    char Avatar[PLAYER_AVATAR_MAXLEN + 1];
}PLAYER_INFO, *PPLAYER_INFO;

typedef struct _GAME_ROOM
{
    UINT RoomNumber;
    LONG64 RefCnt;

    BOOL bGaming; // is game running. (or waiting otherwise)
    UINT IDCount;

    char Password[ROOM_PASSWORD_MAXLEN + 1];

    SRWLOCK PlayerListLock; // Visiting / Writing following field needs this lock.

    UINT WaitingCount;
    UINT PlayingCount;
    PLAYER_INFO WaitingList[ROOM_PLAYER_MAX]; // Stores only online player info. If a user is offline, will be removed from this list.
    PLAYER_INFO PlayingList[ROOM_PLAYER_MAX]; // Copied from WaitingList when game starts, and not modified until game ends.
                                              //     except pConnInfo field (will be set to NULL if a player is offline)

    UINT RoleList[ROOM_PLAYER_MAX];

    UINT LeaderIndex; // current leader
    INT FairyIndex;   // -1 means fairy is not enabled.
}GAME_ROOM, * PGAME_ROOM;

VOID InitRoomManager(VOID);

BOOL CreateRoom(_Inout_ PCONNECTION_INFO pConnInfo, _In_z_ const char* NickName, _In_opt_z_ const char* Password);

BOOL JoinRoom(_In_ UINT RoomNum, _Inout_ PCONNECTION_INFO pConnInfo, _In_z_ const char* NickName, _In_opt_z_ const char* Password);

VOID LeaveRoom(_Inout_ PCONNECTION_INFO pConnInfo);

BOOL ChangeAvatar(_Inout_ PCONNECTION_INFO pConnInfo, _In_z_ const char* Avatar);

BOOL StartGame(_Inout_ PCONNECTION_INFO pConnInfo);
