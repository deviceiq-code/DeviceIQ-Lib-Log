#include "DevIQ_Log.h"

using namespace DeviceIQ_Log;
using namespace DeviceIQ_FileSystem;
using namespace DeviceIQ_DateTime;

void Log::WriteToSerial(const String& message) {
    if (mSerialPort) {
        mSerialPort->println(message);
    }
}

void Log::WriteToSyslog(const String& message) {
    if (WiFi.isConnected() && !mSyslogServerURL.isEmpty() && mSyslogServerPort > 0) {
        mUDPClient.beginPacket(mSyslogServerURL.c_str(), mSyslogServerPort);
        mUDPClient.print(message);
        mUDPClient.endPacket();
    }
}

void Log::WriteToFile(const String& message) {
    if (mFileSystem && !mLogFile.isEmpty()) {
        if (mFileSystem->PercentFree() >= 5.0f) {
            fs::File file = mFileSystem->OpenFile(mLogFile, "a");
            if (file) {
                file.println(message);
                file.close();
            }
        }
    }
}

void Log::Write(const String& message, LogLevels logtype) {
    char levelChar = 'U';

    if ((logtype == LOGLEVEL_ERROR) && (mLogLevel & LOGLEVEL_ERROR)) levelChar = 'E';
    if ((logtype == LOGLEVEL_WARNING) && (mLogLevel & LOGLEVEL_WARNING)) levelChar = 'W';
    if ((logtype == LOGLEVEL_INFO) && (mLogLevel & LOGLEVEL_INFO)) levelChar = 'I';
    if ((logtype == LOGLEVEL_DEBUG) && (mLogLevel & LOGLEVEL_DEBUG)) levelChar = 'D';
    if ((mLogLevel & LOGLEVEL_ALL) == LOGLEVEL_ALL) {
        if (logtype == LOGLEVEL_ERROR) levelChar = 'E';
        if (logtype == LOGLEVEL_WARNING) levelChar = 'W';
        if (logtype == LOGLEVEL_INFO) levelChar = 'I';
        if (logtype == LOGLEVEL_DEBUG) levelChar = 'D';
    }

    if (levelChar == 'U') return;
    
    String msg = String("[") + levelChar + "]";
    if (!mClock) {
        char ts[22];
        sprintf(ts, "0000-00-00 %08lu", (unsigned long)millis());
        msg += String("[") + ts + "] " + message;
    } else {
        msg += String("[") + mClock->CurrentDate().toString() + " " + mClock->CurrentTime().toString() + "] " + message;
    }

    if (mEndpoint & ENDPOINT_SERIAL) WriteToSerial(msg);
    if (mEndpoint & ENDPOINT_SYSLOG) WriteToSyslog(msg);
    if (mEndpoint & ENDPOINT_FILE) WriteToFile(msg);
}