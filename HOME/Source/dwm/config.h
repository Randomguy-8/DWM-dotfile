/* See LICENSE file for copyright and license details. */


#include <X11/XF86keysym.h>


/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 20;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Roboto:size=10" };
static const char dmenufont[]       = "Roboto:size=10";
static const char col_gray1[]       = "#1F1F28"; /* Background Color */
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#DCD7BA"; /* Text Color */
static const char col_gray4[]       = "#FF9E3B"; /* Icons Color */
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_gray1,  col_cyan  },
};

static const char *const autostart[] = {
    "auto-redshift", NULL,
    NULL /* terminate */
};

/* tagging */
static const char *tags[] = {"", "", "", "", ""};

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "N",      NULL },    /* first entry is default */
    { "T",      tile },
    { "M",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

/* volume control */
static const char *mutecmd[] = { "amixer", "set", "Master", "toggle", NULL };
static const char *volupcmd[] = { "amixer", "set", "Master", "2+", "unmute", NULL };
static const char *voldowncmd[] = { "amixer", "set", "Master", "2-", "unmute", NULL };

/* brightness control */
static const char *brupcmd[] = { "sudo", "xbacklight", "+1%", NULL };
static const char *brdowncmd[] = { "sudo", "xbacklight", "-1%", NULL };

/* Applications */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *texteditorcmd[]  = { "geany", NULL };
static const char *filemanagercmd[]  = { "thunar", NULL };
static const char *browsercmd[]  = { "firefox", NULL };

/* Rofi */
static const char *rofilauncher[]  = { "rofiapplauncher", NULL };
static const char *rofirunner[]  = { "roficmdrunner", NULL };
static const char *rofipowermenu[]  = { "rofipowermenu", NULL };


static Key keys[] = {
    /* Application modifier         key        function        argument */
    { Mod4Mask,                  XK_p,      spawn,          {.v = dmenucmd } },
    { Mod4Mask,                  XK_t,      spawn,          {.v = termcmd } },
    { Mod4Mask,                  XK_b,      spawn,          {.v = browsercmd } },
    { Mod4Mask,                  XK_f,      spawn,          {.v = filemanagercmd } },
    { Mod4Mask,                  XK_a,      spawn,          {.v = texteditorcmd } },

    { Mod4Mask,                  XK_F3,      spawn,          {.v = rofilauncher } },
    { Mod4Mask,                  XK_F2,      spawn,          {.v = rofirunner } },

    { 0, XF86XK_AudioMute, spawn, {.v = mutecmd } },
    { 0, XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
    { 0, XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
    { 0, XF86XK_MonBrightnessUp, spawn, {.v = brupcmd} },
    { 0, XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd} },

    /* System modifier */
    { MODKEY|ShiftMask,                         XK_v,      togglebar,      {0} },
    { Mod4Mask,                                 XK_Tab,    view,           {0} },
    
    { Mod4Mask|ShiftMask,                       XK_n,      setlayout,      {.v = &layouts[0]} },
    { Mod4Mask|ShiftMask,                       XK_t,      setlayout,      {.v = &layouts[1]} },
    { Mod4Mask|ShiftMask,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    
/*  { Mod4Mask,                       XK_space,  setlayout,      {0} },
    { Mod4Mask|ShiftMask,             XK_space,  togglefloating, {0} }, */
    
    { Mod4Mask,                       XK_0,      view,           {.ui = ~0 } },
    { Mod4Mask|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { Mod4Mask,                       XK_minus,  setgaps,        {.i = -1 } },
    { Mod4Mask,                       XK_equal,  setgaps,        {.i = +1 } },
    { Mod4Mask|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
    
/*  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } }, */
    
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)

    
    { Mod4Mask,                       XK_Up,      incnmaster,     {.i = +1 } },
    { Mod4Mask,                       XK_Down,      incnmaster,     {.i = -1 } },
    { Mod4Mask,                       XK_Left,   setmfact,       {.f = -0.05} },
    { Mod4Mask,                       XK_Right,  setmfact,       {.f = +0.05} },
    { MODKEY,                         XK_Tab,    focusstack,     {.i = +1 } },
/*  { MODKEY|ShiftMask,               XK_Tab,    focusstack,     {.i = -1 } },*/

    { MODKEY,                       XK_F4,     killclient,     {0} },
    { Mod4Mask,                     XK_q,      killclient,     {0} },
    
    { Mod4Mask,                     XK_Return,      zoom,           {0} },
    {  MODKEY|ControlMask,                  XK_Delete,      spawn,          {.v = rofipowermenu } },
    { MODKEY|ControlMask,             XK_End,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         Mod4Mask,         Button1,        movemouse,      {0} },
    { ClkClientWin,         Mod4Mask,         Button2,        togglefloating, {0} },
    { ClkClientWin,         Mod4Mask,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            Mod4Mask,         Button1,        tag,            {0} },
    { ClkTagBar,            Mod4Mask,         Button3,        toggletag,      {0} },
};

