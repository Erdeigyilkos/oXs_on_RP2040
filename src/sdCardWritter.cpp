#include "sdCardWritter.h"

sdCardWritter::sdCardWritter() {
  m_SdCardReady = false;
  m_isInicialized = false;
  m_wasOK = false;
  m_headerPrinted = false;
  m_fileName = "";
  m_failedCounter = 0;
  m_mountFailedCounter = 0;
  m_headerWritten = false;
  sd_init_driver();
}

void sdCardWritter::setGPS(GPS *gps) { m_gps = gps; }

void sdCardWritter::setConfig(CONFIG *config) { m_config = config; }

void sdCardWritter::trySetFileName() {
  if (!m_gps->GPS_fix) {
    return;
  }

  uint8_t month, day, hour, minute, second;
  int year;
  const uint32_t mask = 255;

  second = (m_gps->gpsTime & (mask << 8)) >> 8;
  minute = (m_gps->gpsTime & (mask << 16)) >> 16;
  hour = (m_gps->gpsTime & (mask << 24)) >> 24;

  day = (m_gps->gpsDate & (mask << 8)) >> 8;
  month = (m_gps->gpsDate & (mask << 16)) >> 16;
  year = (m_gps->gpsDate & (mask << 24)) >> 24;

  char buffer[30];
  sprintf(buffer, "%02d%02d%02d_%02d%02d%02d.csv", year, month, day, hour,
          minute, second);

  m_fileName = std::string(buffer);
  m_isInicialized = true;
}

uint32_t sdCardWritter::getFailedCounter() { return m_failedCounter; }

void sdCardWritter::mountSdCard() {
  m_mountFailedCounter++;
  if (m_mountFailedCounter >= 10 && m_wasOK) {
    resetDrivers();
    m_mountFailedCounter = 0;
  }

  FRESULT fr = f_mount(&m_fs, "0:", 1);
  if (FR_OK != fr) {
    m_failedCounter++;
  } else {
    m_SdCardReady = true;
    m_mountFailedCounter = 0;
  }
}

void sdCardWritter::uMountSdCard() { f_unmount("0:"); }

bool sdCardWritter::writeRecordToSdCard() {
  if (!m_isInicialized) {
    trySetFileName();
    return false;
  }

  if (!m_SdCardReady) {
    uMountSdCard();
    mountSdCard();
    return false;
  }

  FIL fil;
  FRESULT fr = f_open(&fil, m_fileName.c_str(), FA_OPEN_APPEND | FA_WRITE);

  if (FR_OK != fr && FR_EXIST != fr) {
    m_failedCounter++;
    m_SdCardReady = false;
    return false;
  }

  if (!m_headerWritten) {
    writeHeader(&fil);
  }

  uint8_t month, day, hour, minute, second;
  int year;
  const uint32_t mask = 255;

  second = (line.gpsTime & (mask << 8)) >> 8;
  minute = (line.gpsTime & (mask << 16)) >> 16;
  hour = (line.gpsTime & (mask << 24)) >> 24;

  day = (line.gpsDate & (mask << 8)) >> 8;
  month = (line.gpsDate & (mask << 16)) >> 16;
  year = (line.gpsDate & (mask << 24)) >> 24;

  char buffer[1024];
  sprintf(buffer, "%02d;%02d;%02d;%02d;%02d;%02d;%.7f;%.7f;%.2f;%.2f;%d;%d",
          day, month, year, hour, minute, second, line.gpsLat / 10000000.0,
          line.gpsLon / 10000000.0, line.gpsAlt / 100.0,
          line.gpsSpeed * 3600.0 / 100000.0, line.sbusHold, line.sbusFailSafe);
  if (f_printf(&fil, buffer) < 0) {
    return false;
  }

  if (m_config->sdCardLogMS5611) {
    sprintf(buffer, ";%.2f;%0.2f;%d", (float)line.Ms5611Altitude / 100,
            (float)line.Ms5611Vario / 100, line.Ms5611Temperature);
    if (f_printf(&fil, buffer) < 0) {
      return false;
    }
  }

  if (m_config->sdCardLogMax6675) {
    sprintf(buffer, ";%d", line.Max6675Temperature);
    if (f_printf(&fil, buffer) < 0) {
      return false;
    }
  }

  if (f_printf(&fil, "\n") < 0) {
    return false;
  }

  fr = f_close(&fil);
  if (FR_OK != fr) {
    m_failedCounter++;
    m_SdCardReady = false;
    return false;
  } else {
    m_failedCounter = 0;
    m_wasOK = true;
  }
  return true;
}

void sdCardWritter::writeHeader(FIL *fil) {
  if (m_config == NULL) {
    return;
  }

  if (f_printf(fil, baseHeader.c_str()) < 0) {
    return;
  }

  if (m_config->sdCardLogMS5611) {
    if (f_printf(fil, Ms5611Header.c_str()) < 0) {
      return;
    }
  }

  if (m_config->sdCardLogMax6675) {
    if (f_printf(fil, Max6675Header.c_str()) < 0) {
      return;
    }
  }

  if (f_printf(fil, "\n") < 0) {
    return;
  }
  m_headerWritten = true;
}