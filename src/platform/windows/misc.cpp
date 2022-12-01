#include <filesystem>
#include <iomanip>
#include <sstream>


// prevent clang format from "optimizing" the header include order
// clang-format off
#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>
#include <winuser.h>
#include <ws2tcpip.h>
// clang-format on

#include "src/main.h"
#include "src/utility.h"

using namespace std::literals;
namespace platf {
HDESK syncThreadDesktop() {
  auto hDesk = OpenInputDesktop(DF_ALLOWOTHERACCOUNTHOOK, FALSE, GENERIC_ALL);
  if(!hDesk) {
    auto err = GetLastError();
    BOOST_LOG(error) << "Failed to Open Input Desktop [0x"sv << util::hex(err).to_string_view() << ']';

    return nullptr;
  }

  if(!SetThreadDesktop(hDesk)) {
    auto err = GetLastError();
    BOOST_LOG(error) << "Failed to sync desktop to thread [0x"sv << util::hex(err).to_string_view() << ']';
  }

  CloseDesktop(hDesk);

  return hDesk;
}

void print_status(const std::string_view &prefix, HRESULT status) {
  char err_string[1024];

  DWORD bytes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr,
    status,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    err_string,
    sizeof(err_string),
    nullptr);

  BOOST_LOG(error) << prefix << ": "sv << std::string_view { err_string, bytes };
}
} // namespace platf