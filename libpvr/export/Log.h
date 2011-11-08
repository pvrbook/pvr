//-*-c++-*--------------------------------------------------------------------//

/*! \file Log.h
  Contains the Log class and related functions.
 */

//----------------------------------------------------------------------------//

#ifndef __INCLUDED_PVR_LOG_H__
#define __INCLUDED_PVR_LOG_H__

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// System includes


// Library includes

#include <boost/date_time/posix_time/posix_time.hpp>

#include <Field3D/Log.h>

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

namespace pvr {
namespace Util {

//----------------------------------------------------------------------------//
// Timer
//----------------------------------------------------------------------------//

//! Keeps track of wall-time
class Timer
{
public:
  Timer()
    : m_start(boost::posix_time::microsec_clock::local_time())
  { /* Empty */ }
  void reset()
  {
    m_start = boost::posix_time::microsec_clock::local_time();
  }
  float elapsed() const
  {
    using namespace boost::posix_time;
    ptime now = microsec_clock::local_time();
    time_duration diff = now - m_start;
    return diff.total_milliseconds() / 1000.0f;
  }
private:
  boost::posix_time::ptime m_start;
};
  
//----------------------------------------------------------------------------//
// ProgressReporter
//----------------------------------------------------------------------------//

class ProgressReporter
{
public:
  ProgressReporter(const float interval, 
                   const std::string &indentation = std::string(""));
  void update(const float fractionDone);
private:
  const float m_interval;
  std::string m_indentation;
  Timer m_timer;
};

//----------------------------------------------------------------------------//
// Utils
//----------------------------------------------------------------------------//

//! Converts any type to string using operator <<.
template <typename T>
std::string str(const T &t)
{
  return boost::lexical_cast<std::string>(t);
}

//----------------------------------------------------------------------------//

std::string timeStamp();

//----------------------------------------------------------------------------//
// Log
//----------------------------------------------------------------------------//

namespace Log {
  
  //! Severity of log output
  enum Severity {
    SevMessage, 
    SevWarning
  };
  
  //! Prints a log message with severity level
  void print(const Severity severity, const std::string &msg);
  
  //! Prints a log message at SevMessage level
  void print(const std::string &msg);
  
  //! Prints a log message SevWarning level
  void warning(const std::string &msg);
  
}

//----------------------------------------------------------------------------//

} // namespace Util
} // namespace pvr

//----------------------------------------------------------------------------//

#endif // Include guard
