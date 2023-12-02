#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

const UINT32 MAX_SOCKBUF = 256; // 패킷 크기
const UINT32 MAX_WORKERTHREAD = 4; // 쓰레드 풀에 넣을 쓰레드 수

enum class IOOperation
{
	RECV,
	SEND
};

// WSAOVERLAPPED 구조체를 확장 시켜서 필요한 정보를 더 넣었다
struct stOverlappedEx
{
	WSAOVERLAPPED	m_wsaOverlapped;	// Overlapped I/O구조체
	SOCKET			m_socketClient;		// 클라이언트 소켓
	WSABUF 			m_wsaBuf;			// Overlapped I/O작업 버퍼
	IOOperation		m_eOperation;		// Overlapped I/O작업 종류
};

// 클라이언트 정보를 담기 위한 구조체
struct stClientInfo
{
	SOCKET			m_socketClient;		// 클라이언트와 연결되는 소켓
	stOverlappedEx	m_stRecvOverlappedEx; // Recv Overlapped I/O 변수
	stOverlappedEx	m_stSendOverlappedEx; // Send Overlapped I/O 변수

	char			m_RecvBuf[MAX_SOCKBUF];	// Recv 데이터 버퍼
	char			m_SendBuf[MAX_SOCKBUF];	// Send 데이터 버퍼
	stClientInfo()
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}
};


