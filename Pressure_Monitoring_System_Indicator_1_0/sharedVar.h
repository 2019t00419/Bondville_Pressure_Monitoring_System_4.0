#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

extern bool btnHold;
extern int num;
extern float pressure;
extern int modeNo;
extern bool sentRequest;
extern bool mailSent;
extern bool autoOnline;
extern bool reconnecting;
extern String modes[];
extern String updateModes[];
extern char * ssid;
extern char * password;
extern const char * APssid;
extern const char * APpassword;

extern float pressureA;
extern float pressureB;
extern int sensorA;
extern int sensorB;

extern String newSSID;
extern String newPassword;
extern String newPressureA;
extern String newPressureB;
extern String newSensorA;
extern String newSensorB;


extern String GOOGLE_SCRIPT_ID;

extern String BPMSID;
extern String area;
extern int uploadDelay;
extern int emailDelay;
extern float cutoff;
extern String systemStatus;
extern String saveParam;
extern String adminMail;

extern String recipient0;
extern String recipient1;
extern String recipient2;
extern String recipient3;
extern String recipient4;

extern String payload;

extern int loadingSend;
extern int loadingSync;

extern bool calibrated;
extern bool credUpdated;
#endif


