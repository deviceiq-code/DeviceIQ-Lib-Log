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

    // Configure log file path
    logger.LogFile("/logs/system.log");

    // Configure Syslog server (optional)
    logger.SyslogServerURL("192.168.1.100");
    logger.SyslogServerPort(514);
    logger.BeginSyslog();

    // Enable multiple endpoints: Serial, Syslog, File
    logger.Endpoint(ENDPOINT_SERIAL | ENDPOINT_SYSLOG | ENDPOINT_FILE);

    // Set log level
    logger.Level(LOGLEVEL_INFO);

    // Log startup message
    logger.Write("System initialized", LOGLEVEL_INFO);
}

void loop() {
    // Example log message
    logger.Write("Heartbeat message", LOGLEVEL_DEBUG);
    delay(5000);
}
