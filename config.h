/* See LICENSE file for copyright and license details. */

/* Multimedia Keys Support */
#include <X11/XF86keysym.h>
#include "bstack.c"
#include "bstackhoriz.c"

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "xos4 Terminus:size=9" };
static const char dmenufont[]       = "xos4 Terminus:size=9";

static const char normbordercolor[] = "#282828";
static const char normbgcolor[]     = "#282828";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#eeeeee";
static const char selbgcolor[]      = "#282828";
static const char selfgcolor[]      = "#ffffff";

static const char *colors[SchemeLast][3]      = {
  /*               fg           bg           border   */
  [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
  [SchemeSel] =  { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = {
  "www",
  "im",
  "file",
  "work",
  "term",
  "misc" 

  // "\uf268 www",
  // "\uf075 im",
  // "\uf1f8 file",
  // "\uf121 work",
  // "\uf120 term",
  // "\uf188 misc" 
};

static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class      instance    title       tags mask     isfloating   monitor */

  // www
  { "Chromium",      NULL,       NULL,       1 << 0,      False,        -1 },
  { "chromium",      NULL,       NULL,       1 << 0,      False,        -1 },
  { "Firefox",       NULL,       NULL,       1 << 0,      False,        -1 },

  // im
  { "chromium", "web.skype.com", NULL,       1 << 1,      False,         1 },
  { "Skype",         NULL,       NULL,       1 << 1,      False,        -1 },
  { "TelegramDesktop", NULL,     NULL,       1 << 1,      False,        -1 },

  // file
  { "Pcmanfm",       NULL,       NULL,       1 << 2,      False,        -1 },
  { "URxvt",         NULL,       "ranger",   1 << 2,      False,        -1 },
  { "Transmission-gtk", NULL,    NULL,       1 << 2,      False,        -1 },

  // work
  { "Gvim",          NULL,       NULL,       1 << 3,      False,        -1 },
  { "URxvt",         NULL,       "vim",       1 << 3,      False,       -1 },
  { "Atom",          NULL,       NULL,       1 << 3,      False,        -1 },

  // term
  { "URxvt",         NULL,       "tmux",     1 << 4,      False,        -1 },

  // misc
  { "Gimp",          NULL,       NULL,       1 << 5,      True,         -1 },
  { "Wine",          NULL,       NULL,       1 << 5,      True,         -1 },
  { "Steam",         NULL,       NULL,       1 << 5,      True,         -1 },
  { "dota2",         NULL,       NULL,       1 << 5,      True,         -1 },

  // floats
  { "Gtk-chtheme",   NULL,       NULL,       0,           True,         -1 },
  { "File-roller",   NULL,       NULL,       0,           True,         -1 },
  { "Viewnior",      NULL,       NULL,       0,           True,         -1 },
  { "MPlayer",       NULL,       NULL,       0,           True,         -1 },
  { "mpv",           NULL,       NULL,       0,           True,         -1 },
  { "mpv",           "ytc",      NULL,       0,           True,          1 }
};

/* layout(s) */
static const float mfact     = 0.65; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[M]",      monocle },
  { "[]=",      tile },
  { "[]=",      bstackhoriz },
  { "><>",      NULL },
};

// { MODKEY,           KEY,  focusmon, {.i = MON } }, \
// { MODKEY|ShiftMask, KEY,  tagmon,   {.i = MON } }, \

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG, MON) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };

/* applications */
static const char *termcmd[]  = { "urxvt", NULL };
static const char *tmuxcmd[]  = { "urxvt","-title","tmux", "-e", "/home/cf8/bin/runtmux", NULL};
static const char *lockcmd[]  = { "slock", NULL };
static const char *gvimcmd[]  = { "urxvt","-title","vim", "-e", "vim", NULL};
static const char *fmcmd[]    = { "urxvt","-title","ranger", "-e", "ranger", NULL};

/* volume & mpd */
static const char *voldown[]    = { "/home/cf8/bin/vol", "down", NULL};
static const char *volmute[]    = { "/home/cf8/bin/vol", "mute", NULL};
static const char *volup[]      = { "/home/cf8/bin/vol", "up", NULL};
static const char *playcmd[]    = { "mpc", "toggle", NULL};
static const char *stopcmd[]    = { "mpc", "stop", NULL};
static const char *prevcmd[]    = { "mpc", "prev", NULL};
static const char *nextcmd[]    = { "mpc", "next", NULL};

static Key keys[] = {
  { MODKEY,                       XK_grave,  spawn,          SHCMD("dmenu_run") },
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = tmuxcmd } },

  /* applications */
  { 0,                            XK_Pause,  spawn,          {.v = lockcmd } },
  { MODKEY|ShiftMask,             XK_p,      spawn,          {.v = fmcmd } },
  { MODKEY|ShiftMask,             XK_g,      spawn,          {.v = gvimcmd } },

  /* volume & mpd */
  { 0,                            XF86XK_AudioLowerVolume,   spawn,          {.v = voldown } },
  { 0,                            XF86XK_AudioMute,          spawn,          {.v = volmute } },
  { 0,                            XF86XK_AudioRaiseVolume,   spawn,          {.v = volup   } },
  { 0,                            XF86XK_AudioPlay,          spawn,          {.v = playcmd } },
  { 0,                            XF86XK_AudioStop,          spawn,          {.v = stopcmd } },
  { 0,                            XF86XK_AudioPrev,          spawn,          {.v = prevcmd } },
  { 0,                            XF86XK_AudioNext,          spawn,          {.v = nextcmd } },
  { 0,                            XF86XK_WebCam,             spawn,          SHCMD("/home/cf8/bin/toggle_speaker") },

  { MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_Return, zoom,           {0} },
  { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

  { MODKEY,                       XK_a,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_s,      setlayout,      {.v = &layouts[1]} },
  { MODKEY,                       XK_d,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[3]} },

  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

  { MODKEY,                       XK_space,  togglefloating, {0} },
  TAGKEYS(                        XK_1,                      0, 0)
  TAGKEYS(                        XK_2,                      1, 0)
  TAGKEYS(                        XK_3,                      2, 0)
  TAGKEYS(                        XK_q,                      3, 0)
  TAGKEYS(                        XK_w,                      4, 0)
  TAGKEYS(                        XK_e,                      5, 0)
  {  MODKEY|ShiftMask,            XK_r,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

