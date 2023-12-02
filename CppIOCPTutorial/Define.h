#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

const UINT32 MAX_SOCKBUF		= 256; // 소켓 버퍼의 크기
const UINT32 MAX_SOCK_SENDBUF	= 4096; // 소켓 버퍼의 크기
const UINT32 MAX_WORKERTHREAD	= 4; // 쓰레드 풀에 넣을 쓰레드 수

enum class IOOperation
{
	RECV,
	SEND
};

// WSAOVERLAPPED 구조체를 확장 시켜서 필요한 정보를 더 넣었다.
struct stOverlappedEx
{
	WSAOVERLAPPED	m_wsaOverlapped;		// Overlapped I/O 구조체
	SOCKET			m_socketClient;						// 소켓
	WSABUF 			m_wsaBuf;				// Overlapped I/O 작업 버퍼
	IOOperation 	m_eOperation;			// Overlapped I/O 작업 동작 종류
};

