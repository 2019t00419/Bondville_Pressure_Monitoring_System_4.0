#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

extern bool btnHold;
extern int num;
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


extern String newSSID;
extern String newPassword;


extern String GOOGLE_SCRIPT_ID;

extern String payload;

extern int loadingSend;
extern int loadingSync;

extern bool calibrated;
extern bool credUpdated;
#endif


