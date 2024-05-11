#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
#include "hw_config.h"
#include "stdio.h"
#include "string"
#include "gps.h"
#include "param.h"

class sdCardWritter {       // The class
  public:   
    sdCardWritter();
    void setGPS(GPS *gps);
    void setConfig(CONFIG *config);
    bool writeRecordToSdCard();
    uint32_t getFailedCounter();

  struct sdCardWritterLine
   {
     int32_t gpsDate;
     int32_t gpsTime;
     int32_t gpsLat;
     int32_t gpsLon;
     int32_t gpsAlt;
     int32_t gpsSpeed;
     int32_t sbusHold;
     int32_t sbusFailSafe;
     int32_t Ms5611Altitude;
     int32_t Ms5611Vario;
     int32_t Ms5611Temperature;
     int32_t Max6675Temperature;
   } line;
   
   
  private:
   bool m_isInicialized;
   bool m_SdCardReady;
   GPS *m_gps;
   CONFIG *m_config;
   FATFS m_fs;
   std::string m_fileName;
   uint8_t m_failedCounter;
   void trySetFileName();
   void uMountSdCard();
   void mountSdCard();
   uint8_t m_mountFailedCounter;
   bool m_wasOK;
   bool m_headerPrinted;
   bool m_headerWritten;

   void writeHeader(FIL *fil);

  const std::string baseHeader = "day;month;year;hour;minute;second;lat;lon;altitude;speed;sbushold;sbusfail";
  const std::string Ms5611Header = ";altitude;vario;temperature";
  const std::string Max6675Header = ";temperature";

};