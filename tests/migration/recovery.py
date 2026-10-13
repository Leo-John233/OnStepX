"""使用真实入口验证泊车恢复与回零组合"""
from run import method, run

PREFIX = r'''
#include <cassert>
#include <cmath>
#define ON 1
#define OFF 0
#define GOTO_FEATURE ON
#define AXIS1_SECTOR_GEAR OFF
#define AXIS2_TANGENT_ARM OFF
#define MOUNT_COORDS_MEMORY OFF
#define MOUNT_STARTUP_MODE 0
#define SA_STRICT 2
#define ALIGN_MAX_NUM_STARS 1
#define AXIS1_PEC OFF
#define AXIS1_SENSE_HOME_DIST_LIMIT 180
#define AXIS2_SENSE_HOME_DIST_LIMIT 180
#define DLF(x)
#define VLF(x)
#define DF(x)
#define D(x)
#define VF(x)
#define V(x)
#define VL(x)
using CommandError=int;
enum {CE_NONE,CE_SLEW_ERR_UNSPECIFIED,CE_SLEW_ERR_OUTSIDE_LIMITS,CE_SLEW_ERR_HARDWARE_FAULT,
 CE_SLEW_ERR_IN_STANDBY,CE_SLEW_IN_MOTION,CE_SLEW_ERR_IN_PARK,CE_NO_PARK_POSITION_SET,CE_NOT_PARKED,CE_PARKED};
enum {HS_NONE,HS_HOMING,GS_NONE=0,GS_GOTO=1,GU_NONE=0,GU_HOME_GUIDE=1,PS_UNPARKED=0,PS_PARKED=1,PS_PARKING=2};
enum {CR_MOUNT_EQU,PSS_EAST_ONLY,PIER_SIDE_NONE,PIER_SIDE_EAST,PIER_SIDE_WEST,GEM,ALTAZM,ALTALT};
constexpr double PI=3.141592653589793;
double degToRad(double x){return x*PI/180;} double arcsecToRad(double x){return degToRad(x/3600);} double radToDeg(double x){return x*180/PI;}
struct Coordinate {double h=0,d=0,a1=0,a2=0;int pierSide=PIER_SIDE_EAST;};
struct {bool trust=false,locked=true,parkAllowed=true,known=false,nvError=false;
 bool trusted(){return trust&&!locked;} bool needsHome(){return locked;} bool canReturnHome(){return known;}
 bool canRestorePark(){return parkAllowed;}
 void requireHome(){trust=false;locked=true;parkAllowed=false;}
 void completeHome(){trust=!nvError;locked=nvError;parkAllowed=false;}
} startupAuthority;
struct {bool dateIsReady=true,timeIsReady=true;bool isDateTimeReady(){return dateIsReady&&timeIsReady;}} site;
struct Axis {bool ready=true;double pos=0; bool physicalLimitReady(){return ready;}
 double getInstrumentCoordinate(){return pos;} void setBacklashSteps(int){} void setBacklash(double){}
 void setInstrumentCoordinatePark(double v){pos=v;} long getMotorPositionSteps(){return 0;}
} axis1,axis2;
struct {bool fault=false,moving=false,on=false,syncFromOnStepToEncoders=false;
 struct {struct {double axis1=0,axis2=0;} backlash;} settings;
 bool motorFault(){return fault;} bool isSlewing(){return moving;} bool isHome(){return true;}
 bool isTracking(){return on;} void tracking(bool v){on=v;} void enable(bool){} void captureNominalIndexPositions(){}
} mount;
struct {bool on=false;struct{double pastMeridianE=0,pastMeridianW=0;}settings;void enabled(bool v){on=v;}} limits;
struct {int mountType=GEM;void equToHor(Coordinate*){} void equToAa(Coordinate*){}
 void mountToInstrument(Coordinate*c,double*a,double*b){*a=c->h;*b=c->d;}} transform;
struct Nv {Nv& kv(){return *this;} template<class T>void put(int,T){}} storage;
Nv& nv(){return storage;}
struct {int state=GS_NONE;bool absoluteEncodersPresent=false,firstGoto=true;
 int request(Coordinate,int,bool){return CE_NONE;} void abort(){state=GS_NONE;}} goTo;
struct {int state=GU_NONE,starts=0; void stop(){state=GU_NONE;}
 int startHome(){++starts;return CE_NONE;}} guide;
struct Home {int state=HS_NONE,resetError=0;bool hasSense=true,failed=false,isRequestWithReset=false,wasTracking=false,abortPending=false,offsetActive=false;
 struct {struct{double senseOffset=0;}axis1,axis2;}settings;
 int request();int reset(bool=true){return resetError;} void requestAborted(){state=HS_NONE;startupAuthority.requireHome();}
 void abort(){requestAborted();} Coordinate getPosition(int){return Coordinate();}
} home;
struct Park {int state=PS_PARKED,nvKey=0;struct{bool saved=true;int state=PS_PARKED;Coordinate position;}settings;
 int restore(bool);void reset(){state=PS_UNPARKED;settings.state=state;}
} park;
'''

def check(cxx):
    bodies = method('src/telescope/mount/home/Home.cpp', 'CommandError Home::request()') + '\n' + method('src/telescope/mount/park/Park.cpp', 'CommandError Park::restore(')
    run(cxx, 'park_home_recovery', PREFIX + bodies + r'''
void fresh(){startupAuthority={};site={};axis1={};axis2={};mount={};limits={};goTo={};guide={};home={};park={};}
int main(){
 fresh();park.settings.position.h=1;assert(park.restore(false)==CE_NONE);
 assert(!startupAuthority.trusted()&&!limits.on&&park.state==PS_PARKED&&axis1.pos==1);
 assert(park.restore(true)==CE_NONE);assert(startupAuthority.trusted()&&mount.on&&park.state==PS_UNPARKED);
 fresh();startupAuthority.parkAllowed=false;assert(park.restore(true)==CE_SLEW_ERR_UNSPECIFIED);
 fresh();home.resetError=CE_SLEW_IN_MOTION;assert(park.restore(true)!=CE_NONE);assert(!startupAuthority.trusted());
 fresh();startupAuthority.nvError=true;assert(park.restore(true)!=CE_NONE);assert(!mount.on);
 fresh();axis1.ready=false;assert(park.restore(true)==CE_SLEW_ERR_OUTSIDE_LIMITS);
 fresh();home.state=HS_HOMING;assert(park.restore(true)==CE_SLEW_IN_MOTION);
 fresh();goTo.absoluteEncodersPresent=true;mount.moving=true;assert(park.restore(true)==CE_SLEW_IN_MOTION);
 fresh();assert(home.request()==CE_NONE);assert(park.state==PS_UNPARKED&&guide.starts==1&&!startupAuthority.trusted());
 fresh();home.hasSense=false;assert(home.request()==CE_SLEW_ERR_UNSPECIFIED);assert(park.state==PS_PARKED);
 fresh();home.hasSense=false;startupAuthority.known=true;assert(home.request()==CE_NONE);assert(park.state==PS_UNPARKED);
 fresh();axis2.ready=false;assert(home.request()!=CE_NONE);assert(park.state==PS_PARKED);
 fresh();startupAuthority.trust=true;startupAuthority.locked=false;assert(home.request()==CE_SLEW_ERR_IN_PARK);
}
''')
