#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

extern bool btnHold;
extern int num;
extern bool sentRequest;
extern bool mailSent;
extern bool autoOnline;
extern bool reconnecting;
extern String updateModes[];
extern char * ssid;
extern char * password;
extern const char * APssid;
extern const char * APpassword;

extern String BPMS0001A;
extern String BPMS0001P;
extern String BPMS0001S;
extern String BPMS0002A;
extern String BPMS0002P;
extern String BPMS0002S;
extern String BPMS0003A;
extern String BPMS0003P;
extern String BPMS0003S;
extern String BPMS0004A;
extern String BPMS0004P;
extern String BPMS0004S;
extern String BPMS0005A;
extern String BPMS0005P;
extern String BPMS0005S;
extern String BPMS0006A;
extern String BPMS0006P;
extern String BPMS0006S;

extern String newSSID;
extern String newPassword;


extern String GOOGLE_SCRIPT_ID;

extern String payload;

extern int loadingSend;
extern int loadingSync;

extern bool calibrated;
extern bool credUpdated;
#endif


