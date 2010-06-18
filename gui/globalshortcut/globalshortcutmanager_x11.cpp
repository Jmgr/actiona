/*
 * globalshortcutmanager_x11.cpp - X11 implementation of global shortcuts
 * Copyright (C) 2003-2007  Justin Karneges, Michail Pishchagin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "globalshortcutmanager.h"
#include "globalshortcuttrigger.h"

#include <QWidget>
#include <QX11Info>
#include <QKeyEvent>
#include <QCoreApplication>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#ifdef KeyPress
// defined by X11 headers
const int XKeyPress   = KeyPress;
const int XKeyRelease = KeyRelease;
#undef KeyPress
#endif

class X11KeyTrigger
{
public:
	virtual ~X11KeyTrigger() {}
	virtual void activate() = 0;
	virtual bool isAccepted(int qkey) const = 0;
};

class X11KeyTriggerManager : public QObject
{
public:
	static X11KeyTriggerManager* instance()
	{
		if(!instance_)
			instance_ = new X11KeyTriggerManager();
		return instance_;
	}

	void addTrigger(X11KeyTrigger* trigger)
	{
		triggers_ << trigger;
	}

	void removeTrigger(X11KeyTrigger* trigger)
	{
		triggers_.removeAll(trigger);
	}

	struct Qt_XK_Keygroup
	{
		char num;
		int sym[3];
	};

protected:
	// reimplemented
	bool eventFilter(QObject* o, QEvent* e)
	{
		if(e->type() == QEvent::KeyPress) {
			QKeyEvent* k = static_cast<QKeyEvent*>(e);
			int qkey = k->key();
			if (k->modifiers() & Qt::ShiftModifier)
				qkey |= Qt::SHIFT;
			if (k->modifiers() & Qt::ControlModifier)
				qkey |= Qt::CTRL;
			if (k->modifiers() & Qt::AltModifier)
				qkey |= Qt::ALT;
			if (k->modifiers() & Qt::MetaModifier)
				qkey |= Qt::META;

			foreach(X11KeyTrigger* trigger, triggers_) {
				if (trigger->isAccepted(qkey)) {
					trigger->activate();
					return true;
				}
			}
		}

		return QObject::eventFilter(o, e);
	}

private:
	X11KeyTriggerManager()
		: QObject(QCoreApplication::instance())
	{
		QCoreApplication::instance()->installEventFilter(this);
	}

	static X11KeyTriggerManager* instance_;
	QList<X11KeyTrigger*> triggers_;

private:
	struct Qt_XK_Keymap
	{
		int key;
		Qt_XK_Keygroup xk;
	};

	static Qt_XK_Keymap qt_xk_table[];
	static long alt_mask;
	static long meta_mask;
	static long super_mask;
	static long hyper_mask;
	static long numlock_mask;
	static bool haveMods;

	// adapted from qapplication_x11.cpp
	static void ensureModifiers()
	{
		if (haveMods)
			return;

		Display* appDpy = QX11Info::display();
		XModifierKeymap* map = XGetModifierMapping(appDpy);
		if (map) {
			// XKeycodeToKeysym helper code adapeted from xmodmap
			int min_keycode, max_keycode, keysyms_per_keycode = 1;
			XDisplayKeycodes (appDpy, &min_keycode, &max_keycode);
			XFree(XGetKeyboardMapping (appDpy, min_keycode, (max_keycode - min_keycode + 1), &keysyms_per_keycode));
			
			int i, maskIndex = 0, mapIndex = 0;
			for (maskIndex = 0; maskIndex < 8; maskIndex++) {
				for (i = 0; i < map->max_keypermod; i++) {
					if (map->modifiermap[mapIndex]) {
						KeySym sym;
						int symIndex = 0;
						do {
							sym = XKeycodeToKeysym(appDpy, map->modifiermap[mapIndex], symIndex);
							symIndex++;
 						} while ( !sym && symIndex < keysyms_per_keycode);
						if (alt_mask == 0 && (sym == XK_Alt_L || sym == XK_Alt_R)) {
							alt_mask = 1 << maskIndex;
						}
						if (meta_mask == 0 && (sym == XK_Meta_L || sym == XK_Meta_R)) {
							meta_mask = 1 << maskIndex;
						}
						if (super_mask == 0 && (sym == XK_Super_L || sym == XK_Super_R)) {
							super_mask = 1 << maskIndex;
						}
						if (hyper_mask == 0 && (sym == XK_Hyper_L || sym == XK_Hyper_R)) {
							hyper_mask = 1 << maskIndex;
						}
						if (numlock_mask == 0 && (sym == XK_Num_Lock)) {
							numlock_mask = 1 << maskIndex;
						}
					}
					mapIndex++;
				}
			}

			XFreeModifiermap(map);

			// logic from qt source see gui/kernel/qkeymapper_x11.cpp
			if (meta_mask == 0 || meta_mask == alt_mask) {
				// no meta keys... s,meta,super,
				meta_mask = super_mask;
				if (meta_mask == 0 || meta_mask == alt_mask) {
					// no super keys either? guess we'll use hyper then
					meta_mask = hyper_mask;
				}
			}
		}
		else {
			// assume defaults
			alt_mask = Mod1Mask;
			meta_mask = Mod4Mask;
		}

		haveMods = true;
	}

public:
	static bool convertKeySequence(const QKeySequence& ks, unsigned int* _mod, Qt_XK_Keygroup* _kg)
	{
		int code = ks;
		ensureModifiers();

		unsigned int mod = 0;
		if (code & Qt::META)
			mod |= meta_mask;
		if (code & Qt::SHIFT)
			mod |= ShiftMask;
		if (code & Qt::CTRL)
			mod |= ControlMask;
		if (code & Qt::ALT)
			mod |= alt_mask;

		Qt_XK_Keygroup kg;
		kg.num = 0;
		kg.sym[0] = 0;
		code &= ~Qt::KeyboardModifierMask;

		bool found = false;
		for (int n = 0; qt_xk_table[n].key != Qt::Key_unknown; ++n) {
			if (qt_xk_table[n].key == code) {
				kg = qt_xk_table[n].xk;
				found = true;
				break;
			}
		}

		if (!found) {
			// try latin1
			if (code >= 0x20 && code <= 0x7f) {
				kg.num = 1;
				kg.sym[0] = code;
			}
		}

		if (!kg.num)
			return false;

		if (_mod)
			*_mod = mod;
		if (_kg)
			*_kg = kg;

		return true;
	}

	static QList<long> ignModifiersList()
	{
		QList<long> ret;
		if (numlock_mask) {
			ret << 0 << LockMask << numlock_mask << (LockMask | numlock_mask);
		}
		else {
			ret << 0 << LockMask;
		}
		return ret;
	}
};

X11KeyTriggerManager* X11KeyTriggerManager::instance_ = NULL;

class GlobalShortcutManager::KeyTrigger::Impl : public X11KeyTrigger
{
private:
	KeyTrigger* trigger_;
	int qkey_;

	struct GrabbedKey {
		int code;
		uint mod;
	};
	QList<GrabbedKey> grabbedKeys_;

	static bool failed;
	static int XGrabErrorHandler(Display *, XErrorEvent *)
	{
		qWarning("failed to grab key");
		failed = true;
		return 0;
	}

	void bind(int keysym, unsigned int mod)
	{
		int code = XKeysymToKeycode(QX11Info::display(), keysym);

		// don't grab keys with empty code (because it means just the modifier key)
		if (keysym && !code)
			return;

		failed = false;
		XErrorHandler savedErrorHandler = XSetErrorHandler(XGrabErrorHandler);
		WId w = QX11Info::appRootWindow();
		foreach(long mask_mod, X11KeyTriggerManager::ignModifiersList()) {
			XGrabKey(QX11Info::display(), code, mod | mask_mod, w, False, GrabModeAsync, GrabModeAsync);
			GrabbedKey grabbedKey;
			grabbedKey.code = code;
			grabbedKey.mod  = mod | mask_mod;
			grabbedKeys_ << grabbedKey;
		}
		XSync(QX11Info::display(), False);
		XSetErrorHandler(savedErrorHandler);
	}

public:
	/**
	 * Constructor registers the hotkey.
	 */
	Impl(GlobalShortcutManager::KeyTrigger* t, const QKeySequence& ks)
		: trigger_(t)
		, qkey_(ks)
	{
		X11KeyTriggerManager::instance()->addTrigger(this);

		X11KeyTriggerManager::Qt_XK_Keygroup kg;
		unsigned int mod;
		if (X11KeyTriggerManager::convertKeySequence(ks, &mod, &kg))
			for (int n = 0; n < kg.num; ++n)
				bind(kg.sym[n], mod);
	}

	/**
	 * Destructor unregisters the hotkey.
	 */
	~Impl()
	{
		X11KeyTriggerManager::instance()->removeTrigger(this);

		foreach(GrabbedKey key, grabbedKeys_)
			XUngrabKey(QX11Info::display(), key.code, key.mod, QX11Info::appRootWindow());
	}

	void activate()
	{
		emit trigger_->triggered();
	}

	bool isAccepted(int qkey) const
	{
		return qkey_ == qkey;
	}
};

bool GlobalShortcutManager::KeyTrigger::Impl::failed;
long X11KeyTriggerManager::alt_mask  = 0;
long X11KeyTriggerManager::meta_mask = 0;
long X11KeyTriggerManager::super_mask  = 0;
long X11KeyTriggerManager::hyper_mask  = 0;
long X11KeyTriggerManager::numlock_mask  = 0;
bool X11KeyTriggerManager::haveMods  = false;

X11KeyTriggerManager::Qt_XK_Keymap
X11KeyTriggerManager::qt_xk_table[] = {
	{ Qt::Key_Escape,      {1, { XK_Escape }}},
	{ Qt::Key_Tab,         {2, { XK_Tab, XK_KP_Tab }}},
	{ Qt::Key_Backtab,     {1, { XK_ISO_Left_Tab }}},
	{ Qt::Key_Backspace,   {1, { XK_BackSpace }}},
	{ Qt::Key_Return,      {1, { XK_Return }}},
	{ Qt::Key_Enter,       {1, { XK_KP_Enter }}},
	{ Qt::Key_Insert,      {2, { XK_Insert, XK_KP_Insert }}},
	{ Qt::Key_Delete,      {3, { XK_Delete, XK_KP_Delete, XK_Clear }}},
	{ Qt::Key_Pause,       {1, { XK_Pause }}},
	{ Qt::Key_Print,       {1, { XK_Print }}},
	{ Qt::Key_SysReq,      {1, { XK_Sys_Req }}},
	{ Qt::Key_Clear,       {1, { XK_KP_Begin }}},
	{ Qt::Key_Home,        {2, { XK_Home, XK_KP_Home }}},
	{ Qt::Key_End,         {2, { XK_End, XK_KP_End }}},
	{ Qt::Key_Left,        {2, { XK_Left, XK_KP_Left }}},
	{ Qt::Key_Up,          {2, { XK_Up, XK_KP_Up }}},
	{ Qt::Key_Right,       {2, { XK_Right, XK_KP_Right }}},
	{ Qt::Key_Down,        {2, { XK_Down, XK_KP_Down }}},
	{ Qt::Key_PageUp,      {2, { XK_Prior, XK_KP_Prior }}},
	{ Qt::Key_PageDown,    {2, { XK_Next, XK_KP_Next }}},
	{ Qt::Key_Shift,       {3, { XK_Shift_L, XK_Shift_R, XK_Shift_Lock  }}},
	{ Qt::Key_Control,     {2, { XK_Control_L, XK_Control_R }}},
	{ Qt::Key_Meta,        {2, { XK_Meta_L, XK_Meta_R }}},
	{ Qt::Key_Alt,         {2, { XK_Alt_L, XK_Alt_R }}},
	{ Qt::Key_CapsLock,    {1, { XK_Caps_Lock }}},
	{ Qt::Key_NumLock,     {1, { XK_Num_Lock }}},
	{ Qt::Key_ScrollLock,  {1, { XK_Scroll_Lock }}},
	{ Qt::Key_Space,       {2, { XK_space, XK_KP_Space }}},
	{ Qt::Key_Equal,       {2, { XK_equal, XK_KP_Equal }}},
	{ Qt::Key_Asterisk,    {2, { XK_asterisk, XK_KP_Multiply }}},
	{ Qt::Key_Plus,        {2, { XK_plus, XK_KP_Add }}},
	{ Qt::Key_Comma,       {2, { XK_comma, XK_KP_Separator }}},
	{ Qt::Key_Minus,       {2, { XK_minus, XK_KP_Subtract }}},
	{ Qt::Key_Period,      {2, { XK_period, XK_KP_Decimal }}},
	{ Qt::Key_Slash,       {2, { XK_slash, XK_KP_Divide }}},
	{ Qt::Key_F1,          {1, { XK_F1 }}},
	{ Qt::Key_F2,          {1, { XK_F2 }}},
	{ Qt::Key_F3,          {1, { XK_F3 }}},
	{ Qt::Key_F4,          {1, { XK_F4 }}},
	{ Qt::Key_F5,          {1, { XK_F5 }}},
	{ Qt::Key_F6,          {1, { XK_F6 }}},
	{ Qt::Key_F7,          {1, { XK_F7 }}},
	{ Qt::Key_F8,          {1, { XK_F8 }}},
	{ Qt::Key_F9,          {1, { XK_F9 }}},
	{ Qt::Key_F10,         {1, { XK_F10 }}},
	{ Qt::Key_F11,         {1, { XK_F11 }}},
	{ Qt::Key_F12,         {1, { XK_F12 }}},
	{ Qt::Key_F13,         {1, { XK_F13 }}},
	{ Qt::Key_F14,         {1, { XK_F14 }}},
	{ Qt::Key_F15,         {1, { XK_F15 }}},
	{ Qt::Key_F16,         {1, { XK_F16 }}},
	{ Qt::Key_F17,         {1, { XK_F17 }}},
	{ Qt::Key_F18,         {1, { XK_F18 }}},
	{ Qt::Key_F19,         {1, { XK_F19 }}},
	{ Qt::Key_F20,         {1, { XK_F20 }}},
	{ Qt::Key_F21,         {1, { XK_F21 }}},
	{ Qt::Key_F22,         {1, { XK_F22 }}},
	{ Qt::Key_F23,         {1, { XK_F23 }}},
	{ Qt::Key_F24,         {1, { XK_F24 }}},
	{ Qt::Key_F25,         {1, { XK_F25 }}},
	{ Qt::Key_F26,         {1, { XK_F26 }}},
	{ Qt::Key_F27,         {1, { XK_F27 }}},
	{ Qt::Key_F28,         {1, { XK_F28 }}},
	{ Qt::Key_F29,         {1, { XK_F29 }}},
	{ Qt::Key_F30,         {1, { XK_F30 }}},
	{ Qt::Key_F31,         {1, { XK_F31 }}},
	{ Qt::Key_F32,         {1, { XK_F32 }}},
	{ Qt::Key_F33,         {1, { XK_F33 }}},
	{ Qt::Key_F34,         {1, { XK_F34 }}},
	{ Qt::Key_F35,         {1, { XK_F35 }}},
	{ Qt::Key_Super_L,     {1, { XK_Super_L }}},
	{ Qt::Key_Super_R,     {1, { XK_Super_R }}},
	{ Qt::Key_Menu,        {1, { XK_Menu }}},
	{ Qt::Key_Hyper_L,     {1, { XK_Hyper_L }}},
	{ Qt::Key_Hyper_R,     {1, { XK_Hyper_R }}},
	{ Qt::Key_Help,        {1, { XK_Help }}},
	{ Qt::Key_Direction_L, {0, { 0 }}},
	{ Qt::Key_Direction_R, {0, { 0 }}},

	{ Qt::Key_unknown,     {0, { 0 }}},
};

GlobalShortcutManager::KeyTrigger::KeyTrigger(const QKeySequence& key)
{
	d = new Impl(this, key);
}

GlobalShortcutManager::KeyTrigger::~KeyTrigger()
{
	delete d;
	d = 0;
}
