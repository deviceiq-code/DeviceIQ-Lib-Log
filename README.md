# DeviceIQ Lib Log

`DeviceIQ Lib Log` is a logging library designed for embedded systems based on ESP32/ESP8266. It allows you to record messages with different severity levels and send them to multiple endpoints such as the serial port, a Syslog server via UDP, and files stored in LittleFS.

The library can be integrated with `DevIQ_FileSystem` for persistent log storage and with `DevIQ_DateTime` for precise timestamping.

It supports log level filtering and multiple simultaneous output endpoints.

Features
--------
- Multiple log endpoints:
  - Serial port
  - Syslog server via UDP
  - Files in LittleFS
- Configurable log levels (ERROR, WARNING, INFO, DEBUG, ALL).
- Formatted messages with timestamp from `DevIQ_DateTime` or `millis()`.
- Direct integration with `DevIQ_FileSystem` for safe log writing.
- Simple configuration and consistent API.

Usage Example
-------------

```cpp
#include <DevIQ_FileSystem.h>
#include <DevIQ_DateTime.h>
#include <DevIQ_Log.h>

using namespace DeviceIQ_FileSystem;
using namespace DeviceIQ_Log;
using namespace DeviceIQ_DateTime;

FileSystem fs;
Clock clock;
Log logger(&fs, &clock);

void setup() {
    Serial.begin(115200);

    // Set log file path
    logger.LogFile("/logs/system.log");

    // Configure Syslog server (optional)
    logger.SyslogServerURL("192.168.1.100");
    logger.SyslogServerPort(514);
    logger.BeginSyslog();

    // Enable multiple endpoints (Serial + Syslog + File)
    logger.Endpoint(ENDPOINT_SERIAL | ENDPOINT_SYSLOG | ENDPOINT_FILE);

    // Set log level
    logger.Level(LOGLEVEL_INFO);

    // Test message
    logger.Write("System initialized", LOGLEVEL_INFO);
}

void loop() {
    // Example error log
    logger.Write("Connection error", LOGLEVEL_ERROR);
    delay(5000);
}
```

API Summary
-----------

### Configuration
| Method | Description |
|--------|-------------|
| `void LogFile(const String& value)` | Sets the log file path. |
| `String LogFile() const` | Returns the current log file path. |
| `void SyslogServerURL(const String& value)` | Sets the Syslog server address. |
| `String SyslogServerURL() const` | Returns the Syslog server address. |
| `void SyslogServerPort(uint16_t value)` | Sets the Syslog server port. |
| `uint16_t SyslogServerPort() const` | Returns the Syslog server port. |
| `void SerialPort(HardwareSerial* serialport, unsigned long baud = 115200)` | Sets the serial port and initializes it with the given baud rate. |
| `void Endpoint(Endpoints value)` | Sets active log endpoints. |
| `Endpoints Endpoint() const` | Returns active log endpoints. |
| `void Level(LogLevels value)` | Sets active log levels. |
| `LogLevels Level() const` | Returns active log levels. |
| `void BeginSyslog(uint16_t localPort = 2390)` | Initializes UDP client for Syslog. |

### Writing Logs
| Method | Description |
|--------|-------------|
| `void Write(const String& message, LogLevels logtype)` | Writes a log message to the configured endpoints. |

### Auxiliary Types
- `enum Endpoints { ENDPOINT_NOLOG, ENDPOINT_SERIAL, ENDPOINT_SYSLOG, ENDPOINT_FILE }`
- `enum LogLevels { LOGLEVEL_ERROR, LOGLEVEL_WARNING, LOGLEVEL_INFO, LOGLEVEL_DEBUG, LOGLEVEL_ALL }`

Notes
-----
## Dependencies
- [Arduino](https://www.arduino.cc/)
- [LittleFS](https://github.com/lorol/LITTLEFS)
- `DeviceIQ_FileSystem` library
- `DeviceIQ_DateTime` library (optional for accurate timestamps)

License
-------
This library is released under the MIT License.
