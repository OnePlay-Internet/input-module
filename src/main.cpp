// Created by loki on 5/30/19.

#include <iostream>

#include <boost/log/attributes/clock.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/severity_logger.hpp>

using namespace std::literals;
namespace bl = boost::log;

bl::sources::severity_logger<int> verbose(0); // Dominating output
bl::sources::severity_logger<int> debug(1);   // Follow what is happening
bl::sources::severity_logger<int> info(2);    // Should be informed about
bl::sources::severity_logger<int> warning(3); // Strange events
bl::sources::severity_logger<int> error(4);   // Recoverable errors
bl::sources::severity_logger<int> fatal(5);   // Unrecoverable errors


using text_sink = bl::sinks::asynchronous_sink<bl::sinks::text_ostream_backend>;
boost::shared_ptr<text_sink> sink;

struct NoDelete {
  void operator()(void *) {}
};

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", int)

void log_flush() {
  sink->flush();
}

int log_init() {
  sink = boost::make_shared<text_sink>();

  boost::shared_ptr<std::ostream> stream { &std::cout, NoDelete {} };
  sink->locked_backend()->add_stream(stream);
  sink->set_filter(severity >= 0);

  sink->set_formatter([message = "Message"s, severity = "Severity"s](const bl::record_view &view, bl::formatting_ostream &os) {
    constexpr int DATE_BUFFER_SIZE = 21 + 2 + 1; // Full string plus ": \0"

    auto log_level = view.attribute_values()[severity].extract<int>().get();

    std::string_view log_type;
    switch(log_level) {
    case 0:
      log_type = "Verbose: "sv;
      break;
    case 1:
      log_type = "Debug: "sv;
      break;
    case 2:
      log_type = "Info: "sv;
      break;
    case 3:
      log_type = "Warning: "sv;
      break;
    case 4:
      log_type = "Error: "sv;
      break;
    case 5:
      log_type = "Fatal: "sv;
      break;
    };

    char _date[DATE_BUFFER_SIZE];
    std::time_t t = std::time(nullptr);
    strftime(_date, DATE_BUFFER_SIZE, "[%Y:%m:%d:%H:%M:%S]: ", std::localtime(&t));

    os << _date << log_type << view.attribute_values()[message].extract<std::string>();
  });

  // Flush after each log record to ensure log file contents on disk isn't stale.
  // This is particularly important when running from a Windows service.
  sink->locked_backend()->auto_flush(true);

  bl::core::get()->add_sink(sink);

  // TODO: revisit later if platf::init() has sth to do with input
  // auto deinit_guard = platf::init();
  // if(!deinit_guard) {
  //   return 4;
  // }

  return 0;
}