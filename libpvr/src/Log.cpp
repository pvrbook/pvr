//----------------------------------------------------------------------------//

/*! \file Log.cpp
  Contains implementations of Log namespace and functions
 */

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

// Header include

#include "pvr/Log.h"

// System includes

// Library includes

#include <boost/format.hpp>

// Project headers

#include "pvr/Strings.h"
#include "pvr/Types.h"

//----------------------------------------------------------------------------//
// Namespaces
//----------------------------------------------------------------------------//

using namespace boost;
using namespace std;

using namespace Field3D;

//----------------------------------------------------------------------------//

namespace pvr {
namespace Util {

//----------------------------------------------------------------------------//
// ProgressReporter
//----------------------------------------------------------------------------//

ProgressReporter::ProgressReporter(const float interval, 
                                   const std::string &indentation)
  : m_interval(interval), m_indentation(indentation)
{ 
  
}

//----------------------------------------------------------------------------//

void ProgressReporter::update(const float fractionDone)
{
  if (m_timer.elapsed() > m_interval) {
    m_timer.reset();
    const float percentDone = fractionDone * 100.0f;
    boost::format percentStr = boost::format("%.2f") % percentDone;
    Log::print(m_indentation + percentStr.str() + "%");
  }
}

//----------------------------------------------------------------------------//
// Utils
//----------------------------------------------------------------------------//

std::string timeStamp()
{
  using namespace boost::posix_time;
  ptime now = second_clock::local_time();
  return to_simple_string(now.time_of_day());
}
  
//----------------------------------------------------------------------------//
// Log
//----------------------------------------------------------------------------//

namespace Log {

//----------------------------------------------------------------------------//

void print(const Severity severity, const std::string &msg)
{
  cout << timeStamp() << " " << Str::pvrLogStamp;

  switch (severity) {
  case SevWarning:
    cout << "WARNING: ";
    break;
  case SevMessage:
  default:
    break;
  }

  cout << msg << endl;
}

//----------------------------------------------------------------------------//
  
void print(const std::string &msg)
{
  print(SevMessage, msg);
}
  
//----------------------------------------------------------------------------//
  
void warning(const std::string &msg)
{
  print(SevWarning, msg);
}
  
//----------------------------------------------------------------------------//

} // namespace Log
} // namespace Util
} // namespace pvr

//----------------------------------------------------------------------------//
