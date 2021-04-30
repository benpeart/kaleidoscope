#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

// https://github.com/PaulStoffregen/Time/
#include <TimeLib.h>
#include <WiFiUdp.h>

// From https://github.com/PaulStoffregen/Time/blob/master/examples/TimeNTP_ESP8266WiFi/TimeNTP_ESP8266WiFi.ino

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

//const int timeZone = 1;     // Central European Time
//const int timeZone = -5; // Eastern Standard Time (USA)
const int timeZone = -4; // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)

WiFiUDP Udp;
unsigned int remotePort = 8888; // port to listen for UDP packets

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; // buffer to hold incoming & outgoing packets

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); // NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

time_t getNtpTime()
{
    IPAddress ntpServerIP; // NTP server's ip address

    while (Udp.parsePacket() != 0)
        Udp.flush(); // discard any previously received packets
    DB_PRINTLN("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    DB_PRINT(ntpServerName);
    DB_PRINT(": ");
    DB_PRINTLN(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            DB_PRINTLN("Receive NTP Response");
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    DB_PRINTLN("No NTP Response :-(");
    return 0; // return 0 if unable to get the time
}

class RealTimeClock
{
public:
    void setup()
    {
        DB_PRINTLN(F("RealTimeClock.setup"));

        // start UDP
        DB_PRINTLN("Starting UDP");
        Udp.begin(remotePort);
        DB_PRINTLN("waiting for sync");

        // set the Time library to use NTP to keep time
        setSyncProvider(getNtpTime);
        setSyncInterval(SECS_PER_DAY);

        if (timeStatus() != timeSet)
            DB_PRINTLN(F("Unable to sync the clock"));
    }

    void loop()
    {
        static int lastSecond = -99;

        // if we haven't set the clock yet, keep trying.
        if (timeStatus() != timeSet)
        {
            time_t t = getNtpTime();
            if (t)
                setTime(t);
            else
                return;
        }

        // only display the time every second
        time_t t = now();
        if (lastSecond != second(t))
        {
            lastSecond = second(t);

            DB_PRINTF("%s, %d/%d/%4d %d:%02d:%02d %s\r\n", daysOfTheWeek[weekday(t)-1], month(t), day(t), year(t),
                      hourFormat12(t), minute(t), second(t), isAM(t) ? "AM" : "PM");
        }
    }

private:
    const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
};

void mode_select_clock_face()
{
}

void mode_set_clock()
{
}

#endif // REALTIMECLOCK_H
