#ifndef _actions_h
#define _actions_h

void fGoToPage(uint8_t param); // fonction pour aller à une autre page // param = page où aller.
void fGoToPageAndClearMsg(uint8_t param);   // fonction pour aller à une autre page en ayant effacer le last Message // param = page où aller.
void fGoBack(uint8_t param) ;

void fHome(uint8_t param); // fonction pour traiter Home // param = page où aller.
void fUnlock(uint8_t param) ;
void fReset(uint8_t param) ;
void fCancel(uint8_t param) ;
void fPause(uint8_t param) ;
void fResume(uint8_t param) ;
void fSdFilePrint(uint8_t param ) ;
void fDist(uint8_t param) ;
void fMove(uint8_t param) ;
void fDist(uint8_t param) ; // change le bouton distance affiché de manière cyclique
void fSetXYZ(uint8_t param) ;
void fSdMove(uint8_t param) ;
void fCmd(uint8_t param) ;
void fStartUsb(uint8_t param) ;
void fStartTelnet(uint8_t param) ;
void fStopPc(uint8_t param) ;
void fLogPrev(uint8_t param) ;
void fLogNext(uint8_t param) ;
void fSdShowPrev(uint8_t param) ;
void fSdShowNext(uint8_t param) ;
void fOverSwitch (uint8_t BtnParam) ;
void fOverModify (uint8_t BtnParam) ;
void fSerial(uint8_t param) ; // activate GRBL over Serial2
void fBluetooth(uint8_t param) ; // activate GRBL over Bluetooth
void fTelnet(uint8_t param) ; // activate GRBL over Telnet
void fSdGrblMove(uint8_t param) ;  // param contient _LEFT ou _RIGTH

void fSdFilePrint(uint8_t param ) ;
void fStartSdPrinting(uint8_t param ) ;
void fStartSdGrblPrinting(uint8_t param ) ;
void fSdGrblFilePrint(uint8_t param ) ;
void fCancelGrbl(uint8_t param) ;
void fResumeGrbl(uint8_t param) ;


void fConfirmYesNo(uint8_t param ) ;
void fConfirmedYes(uint8_t param ) ;
void fConfirmedNo(uint8_t param ) ;

void handleAutoMove( uint8_t param) ;

void sendStringToGrbl(void ) ;

#endif

