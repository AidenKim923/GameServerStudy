#pragma once

#include "Define.h"
#include <stdio.h>

class stClientInfo
{
public:
	stClientInfo()
	{
		ZeroMemory(&mRecvOverlappedEx, sizeof(stOverlappedEx));
		mSock = INVALID_SOCKET;
	}

	void Init(const UINT32 index)
	{
		mIndex = index;
	}

	UINT32 GetIndex() { return mIndex; }

	bool IsConnected() { return mSock != INVALID_SOCKET; }

	SOCKET GetSock() { return mSock; }

	char* RecvBuffer() { return mRecvBuf; }

	bool OnConnect(HANDLE iocpHandle_, SOCKET socket_)
	{
		mSock = socket_;

		Clear();

		// I/O Completion Port 객체와 소켓을 연결시킨다.
		if (false == BindIOCOmpletionPart(iocpHandle_))
		{
			return false;
		}

		return BindRecv();
	}

	void Close(bool bIsForce = false)
	{
		struct linger stLinger = { 0, 0 };
		if (true == bIsForce)
		{
			stLinger.l_onoff = 1;
		}

		shutdown(mSock, SD_BOTH);
		setsockopt(mSock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

		closesocket(mSock);
		mSock = INVALID_SOCKET;
	}

	void Clear()
	{
	}

	bool BindIOCOmpletionPart(HANDLE iocpHandle_)
	{
		auto hIOCP = CreateIoCompletionPort((HANDLE)GetSock()
			, iocpHandle_
			, ((ULONG_PTR)this), 0);

			if (NULL == CreateIoCompletionPort((HANDLE)mSock, iocpHandle_, (ULONG_PTR)this, 0))
			{
				return false;
			}

			return true;
	}

	bool BindRecv()
	{
		DWORD dwFlag = 0;
		DWORD dwRecvNumBytes = 0;

		mRecvOverlappedEx.m_wsaBuf.len = MAX_SOCKBUF;
		mRecvOverlappedEx.m_wsaBuf.buf = mRecvBuf;
		mRecvOverlappedEx.m_eOperation = IOOperation::RECV;

		int nRet = WSARecv(mSock,
			&(mRecvOverlappedEx.m_wsaBuf),
			1,
			&dwRecvNumBytes,
			&dwFlag,
			(LPWSAOVERLAPPED) & (mRecvOverlappedEx),
			NULL);
		// socket_error이면 client_socket이 끊어진 것으로 처리한다
		if ((SOCKET_ERROR == nRet) && (ERROR_IO_PENDING != WSAGetLastError()))
		{
			printf("[ERROR] WSARecv() 함수 실패 : %d\n", WSAGetLastError());
			return false;
		}

		return true;
	}

	// 1개의 스레드에서만 호출해야한다.
	bool SendMsg(const UINT32 dataSize_, char* pMsg_)
	{
		auto sendOverlappedEx = new stOverlappedEx();
		ZeroMemory(sendOverlappedEx, sizeof(stOverlappedEx));
		sendOverlappedEx->m_wsaBuf.len = dataSize_;
		sendOverlappedEx->m_wsaBuf.buf = new char[dataSize_];
		CopyMemory(sendOverlappedEx-> m_wsaBuf.buf, pMsg_, dataSize_);
		sendOverlappedEx->m_eOperation = IOOperation::SEND;

		DWORD dwRecvNumBytes = 0;
		int nRet = WSASend(mSock,
			&(sendOverlappedEx->m_wsaBuf),
			1,
			&dwRecvNumBytes,
			0,
			(LPWSAOVERLAPPED)sendOverlappedEx,
			NULL);

		// socket_error이면 client_socket이 끊어진 것으로 처리한다
		if ((SOCKET_ERROR == nRet) && (ERROR_IO_PENDING != WSAGetLastError()))
		{
			printf("[ERROR] WSASend() 함수 실패 : %d\n", WSAGetLastError());
			return false;
		}

		return true;
	}

	void SendCompleted(const UINT32 dataSize_)
	{
		printf("[송신 완료] bytes : %d\n", dataSize_);
	}

private:
	INT32				mIndex					=		0; 
	SOCKET				mSock;								// Client와 연결되는 소켓
	stOverlappedEx		mRecvOverlappedEx;					// Recv Overlapped I/O 작업을 위한 변수
	
	char				mRecvBuf[MAX_SOCKBUF];				// 데이터 버퍼	
};
