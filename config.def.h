/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 0;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 15;       /* vert inner gap between windows */
static const unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 15;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Roboto Mono:size=10",
                                        "JoyPixels:size=10",
                                        "Material Design Icons:size=10",
                                      };
static const char startup[]         = "$HOME/.config/dwm/startup.sh";
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg        border   */
	[SchemeNorm] = { "#D5C4A1", "#202020", "#3C3836"},
	[SchemeSel]  = { "#FBF1C7", "#3C3836", "#FFAF3E"},
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL,     NULL,       NULL,       0,            0,           -1 },
//	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "[M]",      monocle },
  { "[@]",      spiral },
  { "[\\]",     dwindle },
  { "H[]",      deck },
  { "TTT",      bstack },
  { "===",      bstackhoriz },
  { "HHH",      grid },
  { "###",      nrowgrid },
  { "---",      horizgrid },
  { ":::",      gaplessgrid },
  { "|M|",      centeredmaster },
  { ">M>",      centeredfloatingmaster },
  { "><>",      NULL },    /* no layout function means floating behavior */
  { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define AltMask Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define ACMD(cmd) { "/bin/sh", "-c", cmd, NULL }

/* commands */
static char dmenumon[2]         = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]    = { "kitty", NULL };
//basic audio
#define HS "$HOME/.config/hotscripts/"
static const char *upvol[]      = ACMD("pamixer -i 5");
static const char *downvol[]    = ACMD("pamixer -d 5");
static const char *mutevol[]    = ACMD("pamixer --toggle-mute");
//alt audio
static const char *eww_upvol[]  = ACMD(HS"eww_vol.sh up");
static const char *eww_downvol[]= ACMD(HS"eww_vol.sh down");
static const char *eww_mutevol[]= ACMD(HS"eww_vol.sh mute");
//
static const char *pausetrack[] = ACMD("playerctl play-pause");
static const char *prevtrack[]  = ACMD("playerctl previous");
static const char *nexttrack[]  = ACMD("playerctl next");
static const char *brightup[]   = ACMD("xbacklight +5");
static const char *brightdown[] = ACMD("xbacklight -5");
#define THEME "$HOME/.config/rofi/launcher.rasi"
static const char *rofi_drw[]   = ACMD("rofi -show drun -modi drun,run,window -theme "THEME);
static const char *rofi_r[]     = ACMD("rofi -show run -modi run -theme "THEME);
static const char *kblayout_menu[] = ACMD(HS"kb-layouts.x11.rofi.sh");
static const char *powermenu[]     = ACMD(HS"powermenu.x11.rofi.fish");
static const char *screenshotFull[]   = ACMD(HS"screenshot.sh scrot fullToClip");
static const char *screenshotSelect[] = ACMD(HS"screenshot.sh scrot selectToClip");
static const char *screenshotWindow[] = ACMD(HS"screenshot.sh scrot windowToClip");
static const char *flamescreenshot[]  = ACMD(HS"screenshot.sh flameshot select");
#define BROWSER "brave"
static const char *openBrowser[] = ACMD(BROWSER);
static const char *openEditor[]  = ACMD("kitty -e nvim");
static const char *openYT[]      = ACMD(BROWSER" youtube.com");
static const char *notifs[]      = ACMD(HS"rofi_notif_center.sh");

#include "shiftview.c"
#include "push.c"
static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,             XK_d,                      spawn,          {.v = rofi_drw } },
  { MODKEY,             XK_r,                      spawn,          {.v = rofi_r } },
  { MODKEY|ShiftMask,   XK_Return,                 spawn,          {.v = termcmd } },
  { MODKEY,             XK_b,                      togglebar,      {0} },
  { MODKEY,             XK_j,                      focusstack,     {.i = +1 } },
  { MODKEY,             XK_k,                      focusstack,     {.i = -1 } },
  { MODKEY|ShiftMask,   XK_j,                      pushdown,       {0} },
  { MODKEY|ShiftMask,   XK_k,                      pushup,         {0} },
  { MODKEY,             XK_i,                      incnmaster,     {.i = +1 } },
  { MODKEY,             XK_u,                      incnmaster,     {.i = -1 } },
  { MODKEY,             XK_h,                      setmfact,       {.f = -0.05} },
  { MODKEY,             XK_l,                      setmfact,       {.f = +0.05} },
  { MODKEY,             XK_Return,                 zoom,           {0} },
  { MODKEY,             XK_Tab,                    view,           {0} },
  { MODKEY|ShiftMask,   XK_c,                      killclient,     {0} },
  { MODKEY|ControlMask, XK_t,                      setlayout,      {.v = &layouts[0]} },  //tile
  { MODKEY|ControlMask, XK_f,                      setlayout,      {.v = &layouts[13]} }, //float
  { MODKEY|ControlMask, XK_m,                      setlayout,      {.v = &layouts[1]} },  //mono
  { MODKEY|ControlMask, XK_s,                      setlayout,      {.v = &layouts[2]} },   //spiral
  { MODKEY|ControlMask, XK_w,                      setlayout,      {.v = &layouts[3]} },  //dwindle
  { MODKEY|ControlMask, XK_d,                      setlayout,      {.v = &layouts[4]} },  //deck
  { MODKEY|ControlMask, XK_b,                      setlayout,      {.v = &layouts[5]} },  //bstack
  { MODKEY|ControlMask, XK_g,                      setlayout,      {.v = &layouts[7]} },  //grid
  { MODKEY|ControlMask|ShiftMask, XK_m,            setlayout,      {.v = &layouts[11]} }, //centered master
  { MODKEY,             XK_space,                  setlayout,      {0} },
  { MODKEY|ShiftMask,   XK_space,                  togglefloating, {0} },
  { MODKEY|ShiftMask,   XK_f,                      togglefullscr,  {0} },
  { MODKEY,             XK_0,                      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,   XK_0,                      tag,            {.ui = ~0 } },
  { MODKEY,             XK_comma,                  focusmon,       {.i = -1 } },
  { MODKEY,             XK_period,                 focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,   XK_comma,                  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,   XK_period,                 tagmon,         {.i = +1 } },
  { MODKEY,             XK_o,                      incrgaps,       {.i = +1 } },
  { MODKEY|ShiftMask,   XK_o,                      incrgaps,       {.i = -1 } },
  { MODKEY,             XK_g,                      togglegaps,     {0} },
  { MODKEY|ShiftMask,   XK_g,                      defaultgaps,    {0} },
  TAGKEYS(              XK_1,                      0)
  TAGKEYS(              XK_2,                      1)
  TAGKEYS(              XK_3,                      2)
  TAGKEYS(              XK_4,                      3)
  TAGKEYS(              XK_5,                      4)
  TAGKEYS(              XK_6,                      5)
  TAGKEYS(              XK_7,                      6)
  TAGKEYS(              XK_8,                      7)
  TAGKEYS(              XK_9,                      8)
  { MODKEY|ShiftMask,   XK_q,                      quit,           {0} },
  //alt audio
  { 0,                  XF86XK_AudioMute,          spawn,          {.v = eww_mutevol } },
  { 0,                  XF86XK_AudioLowerVolume,   spawn,          {.v = eww_downvol } },
  { 0,                  XF86XK_AudioRaiseVolume,   spawn,          {.v = eww_upvol  } },
  //basic audio
  //{ 0,                  XF86XK_AudioMute,          spawn,          {.v = mutevol } },
  //{ 0,                  XF86XK_AudioLowerVolume,   spawn,          {.v = downvol } },
  //{ 0,                  XF86XK_AudioRaiseVolume,   spawn,          {.v = upvol   } },
  { 0,                  XF86XK_AudioPlay,	         spawn,          {.v = pausetrack } },
  { 0,                  XF86XK_AudioNext,    	     spawn,          {.v = nexttrack  } },
  { 0,                  XF86XK_AudioPrev,    	     spawn,          {.v = prevtrack  } },
  { MODKEY|AltMask,     XK_j,                      shiftview,      {.i = -1 } },
  { MODKEY|AltMask,     XK_k,                      shiftview,      {.i = +1 } },
  { MODKEY,             XK_m,                      focusmaster,    {0} },
  { 0,                  XF86XK_MonBrightnessUp,    spawn,          {.v = brightup } },
  { 0,                  XF86XK_MonBrightnessDown,  spawn,          {.v = brightdown } },
  { MODKEY,             XK_x,                      spawn,          {.v = powermenu } },
  { MODKEY|AltMask,     XK_l,                      spawn,          {.v = kblayout_menu} },
  { 0,                  XK_Print,                  spawn,          {.v = screenshotFull} },
  { ControlMask,        XK_Print,                  spawn,          {.v = screenshotWindow} },
  { MODKEY,             XK_Print,                  spawn,          {.v = screenshotSelect} },
  { MODKEY|ShiftMask,   XK_Print,                  spawn,          {.v = flamescreenshot} },
  { MODKEY,             XK_w,                      spawn,          {.v = openBrowser} },
  { MODKEY,             XK_e,                      spawn,          {.v = openEditor} },
  { MODKEY,             XK_y,                      spawn,          {.v = openYT} },
  { MODKEY,             XK_grave,                  spawn,          {.v = notifs} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
