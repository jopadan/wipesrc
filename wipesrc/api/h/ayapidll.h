#ifndef	_AYAPIDLL_
#define	_AYAPIDLL_


typedef	AYERR (FAR PASCAL *AYSETDISPLAYMASK)(int, int);
typedef AYERR (FAR PASCAL *AYLOADDRIVER)(int, int);
typedef AYERR (FAR PASCAL *AYINITDRIVER)( int);
typedef AYERR (FAR PASCAL *AYDEINITDRIVER)( int);
typedef AYERR (FAR PASCAL *AYUNLOADDRIVER)( int);
typedef AYERR (FAR PASCAL *AYRESETDRIVER)( int);
typedef AYERR (FAR PASCAL *AYSETCLIPREGION)(AYRECT *, int);
typedef AYERR (FAR PASCAL *AYSETDRAWMODE)(AYDRAWMODE *, int);
typedef AYERR (FAR PASCAL *AYSETDRAWBUFFER)(int, int);
typedef AYERR (FAR PASCAL *AYSETDISPBUFFER)(int, int);
typedef int (FAR PASCAL *AYGETDISPBUFFER)( int);
typedef int (FAR PASCAL *AYGETDRAWBUFFER)( int);
typedef AYERR (FAR PASCAL *AYGETCAPS)(AYCAPS *, int);
typedef AYERR (FAR PASCAL *AYLOADVRAM)(void *, AYTEXPAGE, AYRECT *, int, int);
typedef AYERR (FAR PASCAL *AYSAVEVRAM)(void *, AYTEXPAGE, AYRECT *, int, int);
typedef AYERR (FAR PASCAL *AYMOVEVRAM)(AYTEXPAGE, AYRECT *, AYTEXPAGE, AYRECT *, int, int);
typedef AYERR (FAR PASCAL *AYRENDERPRIM)(AYPRIM *, int);
typedef AYERR (FAR PASCAL *AYRENDEROT)(AYPRIM *, int);
typedef AYERR (FAR PASCAL *AYDRAWSYNC)(int);
typedef AYERR (FAR PASCAL *AYVSYNC)( int);


#define	AYSETDISPLAYMASK_ID		1
#define	AYLOADDRIVER_ID			2
#define	AYINITDRIVER_ID			3
#define	AYDEINITDRIVER_ID		4
#define	AYUNLOADDRIVER_ID		5
#define	AYRESETDRIVER_ID		6
#define	AYSETCLIPREGION_ID		7
#define	AYSETDRAWMODE_ID		8
#define	AYSETDRAWBUFFER_ID		9
#define	AYSETDISPBUFFER_ID		10
#define	AYGETDISPBUFFER_ID		11
#define	AYGETDRAWBUFFER_ID		12
#define	AYGETCAPS_ID			13
#define	AYLOADVRAM_ID			14
#define	AYSAVEVRAM_ID			15
#define	AYMOVEVRAM_ID			16
#define	AYRENDERPRIM_ID			17
#define	AYRENDEROT_ID			18
#define	AYDRAWSYNC_ID			19
#define	AYVSYNC_ID				20







#endif