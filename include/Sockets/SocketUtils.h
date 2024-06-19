#pragma once

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define closeSocket close
typedef int SOCKET;
#define SHUTDOWN_BOTH SHUT_RDWR
#define SHUTDOWN_RD SHUT_RD
#define SHUTDOWN_WR SHUT_WR
#define SOCKET_ERROR -1
#define Sleep(n) sleep(n / 1000)

enum class ErrorStatus {
  E_NONE = 0,
  E_UnknownUserDataId,
  E_IncorrectUserDataId,
  E_TimeoutWaitingForResponse,
  E_TcpSendError,
  E_TcpReceiveError,
  E_InternalServerError,
  E_InvalidUserData,
  E_TooFewData,
  E_InvalidRole,
  E_InvalidDetails,
  E_Unauthorized,
};

void setTimeout(SOCKET socket, long sec, long uSec, int optionName);