import os
import shutil


modelName=["UNDEF","LF107","DG1000","BIVOJ"]

class lineRecord:
    intUndef = -999999
    floatUndef = intUndef 
    
    date=intUndef

    def __init__(self):
        pass

    @classmethod
    def init(self,line):
        record = line.split(";")
        
        self.time = self.tryParseInt(record[0])
        
        self.CH1 = self.tryParseFloat(record[1])
        self.CH2 = self.tryParseFloat(record[2])
        self.CH3 = self.tryParseFloat(record[3])
        self.CH4 = self.tryParseFloat(record[4])
        self.CH5 = self.tryParseFloat(record[5])
        self.CH6 = self.tryParseFloat(record[6])
        self.CH7 = self.tryParseFloat(record[7])
        self.CH8 = self.tryParseFloat(record[8])
        self.CH9 = self.tryParseFloat(record[9])
        self.CH10 = self.tryParseFloat(record[10])
        self.CH11 = self.tryParseFloat(record[11])
        self.CH12 = self.tryParseFloat(record[12])
        self.CH13 = self.tryParseFloat(record[13])
        self.CH14 = self.tryParseFloat(record[14])
        self.CH15 = self.tryParseFloat(record[15])
        self.CH16 = self.tryParseFloat(record[16])
        
        self.battery = self.tryParseFloat(record[17])
        self.batteryExternal = self.tryParseFloat(record[18])
        
        self.status = self.tryParseInt(record[19])
        
        self.distance = self.tryParseInt(record[20])
        self.speed = self.tryParseInt(record[21])
        self.altitude = self.tryParseFloat(record[22])
        self.gpsSat = int(self.tryParseFloat(record[23]))
        self.latitude = self.tryParseLatitude(record[24])
        self.longtitude =  self.tryParseLongtitude(record[25])
        self.time = record[26]

        self.MS5611altitude = self.tryParseFloat(record[27])
        self.MS5611variometer = self.tryParseFloat(record[28])
        self.MS5611temperature = self.tryParseInt(record[29])

        self.modelID = self.tryParseInt(record[30])
        self.voltage = self.tryParseFloat(record[31])
        self.date = self.tryParseInt(record[32])
        self.holdFrame = self.tryParseInt(record[33])
        self.failSafe = self.tryParseInt(record[34])        

    @classmethod
    def tryParseFloat(self, value):
        try:
            floatValue = float(value.replace(" ","").replace(",","."))
            return floatValue
        except:
            return self.floatUndef
            
    @classmethod
    def tryParseInt(self, value):
        try:
            intValue = int(value)
            return intValue
        except:
            return self.intUndef
    
    @classmethod
    def tryParseLatitude(self, dms):
        try:
            dms = dms.replace("N","")
            dms = dms.replace(" ","")
            degrees = float(dms.split(":")[0])
            minutes = float(dms.split(":")[1])
            return degrees + minutes/60 
        except:
            return self.floatUndef

    @classmethod
    def tryParseLongtitude(self, dms):
        try:
            dms = dms.replace("E","")
            dms = dms.replace(" ","")
            degrees = float(dms.split(":")[0])
            minutes = float(dms.split(":")[1])
            return degrees + minutes/60 
        except:
            return self.floatUndef
        
    @classmethod
    def isDataValid(self):
        return self.date != self.intUndef and self.gpsSat != self.intUndef
    
    @classmethod
    def decompactDate(self):
        day = self.date >> 8
        month = (self.date >> 4) & 0xF
        year = 2023 + (self.date & 0xF)
        return [year,month,day]



def findAllLogsFiles():
    return [f for f in os.listdir('.') if f.endswith('.csv')]

def createFolder(folder):
    if not os.path.exists(folder):
        os.makedirs(folder)

def copyFileWithRightName(src,folder,date,time):
    filename = str(date[0]) + "-" + str(date[1]) + "-" +str(date[2])
    dst = folder + "/" + filename + "_" + time + ".csv"
    shutil.copyfile(src, dst)

def generateGPXheader(src,folder,date,time):
    filename = str(date[0]) + "-" + str(date[1]) + "-" +str(date[2])
    dst = folder + "/" + filename + "_" + time + ".gpx"
    
    out = open(dst, "w")
    out.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    out.write('<gpx version="1.1" creator="Garmin Connect"><trk>\n')
    out.write('<name>Let c. ' + time  + '</name>\n')
    out.write("<trkseg>\n")
    return out


def writeGPXpoint(GPXwriter,record):
    GPXwriter.write('<trkpt lon="'+ str(record.longtitude) + '" lat="'+ str(record.latitude) + '">\n')
    GPXwriter.write(' <ele>' + str(record.altitude) +  '</ele> \n')
    GPXwriter.write(' <speed>' + str(record.speed) +  '</speed> \n')   
    GPXwriter.write('</trkpt>\n')


def closeGPXWriter(GPXwriter):
    GPXwriter.write("</trkseg>\n")
    GPXwriter.write("</trk>\n")
    GPXwriter.write("</gpx>\n")
    GPXwriter.flush()
    GPXwriter.close()

def processLogFile(logName):
    print("Processing file:",logName)

    logRead = open(logName,"r")
    lineIndex = 0
    isCopied = False
    GPXwriter = None
    record = None
    maxSbusHold = 0
    for line in logRead:
        if lineIndex < 3:
            lineIndex+=1
            continue
        
        record = lineRecord()
        record.init(line)

        if maxSbusHold < record.holdFrame:
            maxSbusHold = record.holdFrame

        if record.isDataValid() and isCopied is False:
            createFolder(modelName[record.modelID])
            copyFileWithRightName(logName, modelName[record.modelID], record.decompactDate(), record.time)
            isCopied = True
            GPXwriter = generateGPXheader(logName, modelName[record.modelID], record.decompactDate(), record.time)
        
        if record.isDataValid():
            writeGPXpoint(GPXwriter,record)



    print(" Processing done, FailSaves:",str(record.failSafe), " MAX SBUS hold:", str(maxSbusHold))

    closeGPXWriter(GPXwriter)
    logRead.close()


for logFile in findAllLogsFiles():
    processLogFile(logFile)




