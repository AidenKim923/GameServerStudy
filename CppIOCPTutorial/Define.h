#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

const UINT32 MAX_SOCKBUF = 256; // ��Ŷ ũ��
const UINT32 MAX_WORKERTHREAD = 4; // ������ Ǯ�� ���� ������ ��

enum class IOOperation
{
	RECV,
	SEND
};

// WSAOVERLAPPED ����ü�� Ȯ�� ���Ѽ� �ʿ��� ������ �� �־���
struct stOverlappedEx
{
	WSAOVERLAPPED	m_wsaOverlapped;	// Overlapped I/O����ü
	SOCKET			m_socketClient;		// Ŭ���̾�Ʈ ����
	WSABUF 			m_wsaBuf;			// Overlapped I/O�۾� ����
	IOOperation		m_eOperation;		// Overlapped I/O�۾� ����
};

// Ŭ���̾�Ʈ ������ ��� ���� ����ü
struct stClientInfo
{
	SOCKET			m_socketClient;		// Ŭ���̾�Ʈ�� ����Ǵ� ����
	stOverlappedEx	m_stRecvOverlappedEx; // Recv Overlapped I/O ����
	stOverlappedEx	m_stSendOverlappedEx; // Send Overlapped I/O ����

	char			m_RecvBuf[MAX_SOCKBUF];	// Recv ������ ����
	char			m_SendBuf[MAX_SOCKBUF];	// Send ������ ����
	stClientInfo()
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}
};


