#ifndef DevIQ_Log_h
#define DevIQ_Log_h

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <DevIQ_DateTime.h>
#include <DevIQ_FileSystem.h>

namespace DeviceIQ_Log {
    enum Endpoints { ENDPOINT_NOLOG = 0b00000000, ENDPOINT_SERIAL = 0b00000001, ENDPOINT_SYSLOG = 0b00000010, ENDPOINT_FILE = 0b00000100 };
    enum LogLevels { LOGLEVEL_ERROR = 0b00000001, LOGLEVEL_WARNING = 0b00000010, LOGLEVEL_INFO = 0b00000100, LOGLEVEL_DEBUG = 0b00001000, LOGLEVEL_ALL = 0b11111111};

    class Log {
        private:
            WiFiUDP mUDPClient;
            DeviceIQ_DateTime::Clock* mClock = nullptr;
            DeviceIQ_FileSystem::FileSystem* mFileSystem = nullptr;
            HardwareSerial* mSerialPort = &Serial;
            Endpoints mEndpoint = ENDPOINT_SERIAL;
            LogLevels mLogLevel = LOGLEVEL_ALL;
            String mSyslogServerURL;
            uint16_t mSyslogServerPort = 0;
            String mLogFile;

            void WriteToSerial(const String& message);
            void WriteToSyslog(const String& message);
            void WriteToFile(const String& message);
        public:
            Log() = default;
            explicit Log(DeviceIQ_FileSystem::FileSystem* fs, DeviceIQ_DateTime::Clock* clk = nullptr) : mClock(clk), mFileSystem(fs) {}

            void LogFile(const String& value) { mLogFile = value; }
            String LogFile() const { return mLogFile; }
            void SyslogServerURL(const String& value) { mSyslogServerURL = value; }
            String SyslogServerURL() const { return mSyslogServerURL; }
            void SyslogServerPort(uint16_t value) { mSyslogServerPort = value; }
            uint16_t SyslogServerPort() const { return mSyslogServerPort; }
            void SerialPort(HardwareSerial* serialport, unsigned long baud = 115200) { mSerialPort = serialport; if (mSerialPort) { mSerialPort->begin(baud); mSerialPort->println(); }}
            Endpoints Endpoint() const { return mEndpoint; }
            void Endpoint(Endpoints value) { mEndpoint = value; }
            LogLevels Level() const { return mLogLevel; }
            void Level(LogLevels value) { mLogLevel = value; }
            void BeginSyslog(uint16_t localPort = 2390) { mUDPClient.begin(localPort); }
            void Write(const String& message, LogLevels logtype);
    };
}

#endif