#include <Windows.h>
#include <intrin.h>
#include <cstdint>
#include <dxgi.h>
#include <time.h>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "rust/rust.hpp"

#include "projectile.hpp"

#include "memory/memory.hpp"
#include "memory/lazy_importer.hpp"
#include "memory/il2cpp.hpp"

#include "utils/no_crt.hpp"
#include "utils/xorstr.hpp"
#include "utils/pattern/pattern.hpp"

#include "gui/OnGUI.hpp"

#include "assets/assets.hpp"

#include "rust/unity.hpp"
#include "rust/features/player_esp.hpp"

#include "hooks.hpp"

//does some dumb nigga rly think i'm going to waste my time make a base than find one already that has features :joy: - k4io

bool has_initialized = false;

bool DllMain(uintptr_t hmodule)
{
	
	/*wchar_t* b1;
	GetEnvironmentVariable(L"WINDOWS1", b1, 65);
	wchar_t* b2;
	GetEnvironmentVariable(L"WINDOWS2", b2, 65);
	wchar_t* b3;
	GetEnvironmentVariable(L"WINDOWS3", b3, 65);

	MessageBoxW(FindWindow(0, L"Calculator"), b1, b2, 0);
	*/
	if (!has_initialized) {
		il2cpp::init();
		mem::game_assembly_base = LI_MODULE_SAFE_(_("GameAssembly.dll"));
		mem::unity_player_base = LI_MODULE_SAFE_(_("UnityPlayer.dll"));
		unity::init_unity();
		gui::init_gui();
		hooks::init_hooks();
		init_bp();
		init_projectile();
		has_initialized = true;
	} 
	il2cpp::hook(&gui::OnGUI, _("OnGUI"), _("DDraw"), _("UnityEngine"), 0);
	//il2cpp::hook(&OnProjectileUpdate, _("Update"), _("Projectile"), _(""), 0);
	il2cpp::hook(&hooks::hk_OnNetworkMessage, _("OnNetworkMessage"), _("Client"), _(""), 1);

	mem::hook_virtual_function(_("BasePlayer"), _("ClientInput"), &hooks::hk_baseplayer_ClientInput);

	mem::hook_virtual_function(_("BasePlayer"), _("BlockSprint"), &hooks::hk_blocksprint);
	//mem::hook_virtual_function(_("Client"), _("IsConnected"), &hooks::is_connected_hk, _("Network"));

	return true;
}
