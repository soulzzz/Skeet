#pragma once
#include <Winsock2.h>
#include <DMALibrary/Memory/Memory.h>
#include <Common/Data.h>
#include <Utils/KmBox.h>
#include <Utils/KmBoxNet.h>
#include <Hack/Players.h>
#include <string_view>

namespace KeyState
{
	void Init()
	{
		GameData.Keyboard = mem.GetKeyboard();

		if (!GameData.Keyboard.InitKeyboard())
		{
			MessageBoxA(NULL, "热键初始化失败，请重启主机", "提示", MB_ICONERROR | MB_OK | MB_TOPMOST);
			//Utils::Log(2, "Failed to initialize keyboard hotkeys through kernel.");
			GameData.KeyState = false;
		}
		else {
			GameData.KeyState = true;
			Utils::Log(1, U8("键盘热键初始化成功: 0x%llx", GameData.Keyboard.GetAddrss()));
		}
	}
	void Update() {
		//Utils::Log(1, "Key Thread Running...");
		while (true)
		{
			GameData.Keyboard.UpdateKeys();

			const std::pair<int, const char*> Keys[] = {
				{ GameData.Config.Menu.ShowKey, "Menu" },
				{ GameData.Config.Overlay.Quit_key, "DEAD" },
				{ GameData.Config.AimBot.Configs[0].Key, "AimBotConfig0" },
				{ GameData.Config.AimBot.Configs[1].Key, "AimBotConfig1" },
				{ VK_DELETE, "RecoverOverlay" },
				{ GameData.Config.Function.ClearKey, "Clear" },
				//{ GameData.Config.Item.GroupKey, "GroupKey" },
				{ GameData.Config.Item.GroupAKey, "GroupAKey" },
				{ GameData.Config.Item.GroupBKey, "GroupBKey" },
				{ GameData.Config.Item.GroupCKey, "GroupCKey" },
				{ GameData.Config.Item.GroupDKey, "GroupDKey" },
				{ GameData.Config.Vehicle.EnableKey, "VehicleEnable" },
				{ GameData.Config.PlayerList.MarkKey, "PlayerListMarkType" },
				{ GameData.Config.Overlay.rankList, "rankList" },
				{ GameData.Config.ESP.FocusModeKey, "FocusModeKey" },
				{ GameData.Config.AirDrop.EnableKey, "AirDropEnableKey" },
				{ GameData.Config.DeadBox.EnableKey, "DeadBoxEnableKey" },
				{ GameData.Config.Overlay.FusionModeKey, "FusionModeKey" },
				{ GameData.Config.ESP.duiyouKey, "duiyouKey" },
				{ GameData.Config.ESP.fast_aimbot_switch, "fast_aimbot_switch" },
				{ GameData.Config.ESP.DataSwitchkey, "DataSwitchkey" },
			};

			/*if (GameData.Keyboard.WasKeyPressed(VK_F2))
				GameData.Config.AimBot.Enable = !GameData.Config.AimBot.Enable;*/

			for (const auto& Key : Keys)
			{
				if (Key.first <= 0) {
					continue;
				}

				if (GameData.Keyboard.WasKeyPressed(Key.first))
				{
					const std::string_view KeyName = Key.second;
						if (KeyName == "AirDropEnableKey")
						{
							GameData.Config.AirDrop.Enable = !GameData.Config.AirDrop.Enable;
						}

				

						if (KeyName == "duiyouKey")
						{
							GameData.Config.ESP.duiyou = !GameData.Config.ESP.duiyou;
						}

						if (KeyName == "fast_aimbot_switch") {
							GameData.Config.AimBot.Enable = !GameData.Config.AimBot.Enable;
						}

						if (KeyName == "DataSwitchkey")
						{
							GameData.Config.ESP.DataSwitch = !GameData.Config.ESP.DataSwitch;
						}

						if (KeyName == "DeadBoxEnableKey")
						{
							GameData.Config.DeadBox.Enable = !GameData.Config.DeadBox.Enable;
						}

						if (KeyName == "FusionModeKey")
						{
							GameData.Config.Overlay.FusionMode = !GameData.Config.Overlay.FusionMode;
							HWND Progman = FindWindowA("Progman", NULL);
							if (GameData.Config.Overlay.FusionMode)
							{
								if (Progman)
								{
									ShowWindow(Progman, SW_HIDE);
									//ShowWindow(TrayWnd, SW_HIDE);
								}
							}
							else if (Progman)
							{
								ShowWindow(Progman, SW_SHOW);
								//ShowWindow(TrayWnd, SW_SHOW);
							}
						}

						if (KeyName == "FocusModeKey")
						{
							GameData.Config.ESP.FocusMode = !GameData.Config.ESP.FocusMode;
						}
						if (KeyName == "rankList")
						{
							GameData.Config.Window.Players = !GameData.Config.Window.Players;
						}
						
						if (KeyName == "DEAD")//  这里是结束
						{

							HWND Progman = FindWindowA("Progman", NULL);
							//HWND TrayWnd = FindWindowA("Shell_TrayWnd", NULL);
							ShowWindow(Progman, SW_SHOW);
							//ShowWindow(TrayWnd, SW_SHOW);
							
							exit(0);
							//ExitProcess(0);
							
							
						}
						if (KeyName == "Clear")
						{
							if (GameData.Config.AimBot.Connected)
							{
								if (GameData.Config.AimBot.Controller == 0)
								{
									KmBox::Clear();
									Utils::Log(1, "KMBOX Clear Success");
								}

								if (GameData.Config.AimBot.Controller == 1)
								{
									KmBoxNet::Clear();
									Utils::Log(1, "KMBOXNET Clear Success");
								}
							}

							Data::SetCacheEntitys({});
							Data::SetCachePlayers({});
							Data::SetPlayers({});
							Data::SetPlayersData({});
							Data::SetCacheVehicles({});
							Data::SetVehicles({});
							Data::SetVehiclWheels({});
							Data::SetItems({});
							Data::SetCacheDroppedItems({});
							Data::SetCacheDroppedItemGroups({});
							GameData.AimBot.Target = 0;
							GameData.AimBot.Lock = false;
							mem.RefreshAll();
						}
						

						/*if (KeyName == "GroupKey")
						{
							if (GameData.Config.Item.ShowGroup != 5)
							{
								GameData.Config.Item.Enable = 1;
								GameData.Config.Item.ShowGroup++;
							}
							else {
								GameData.Config.Item.ShowGroup = 0;
								GameData.Config.Item.Enable = 0;
							}
						}*/

						if (KeyName == "GroupAKey")
						{
							GameData.Config.Item.Enable = 1;
							//GameData.Config.Item.ShowGroup = 2;
							if (GameData.Config.Item.ShowGroups.count(1))
								GameData.Config.Item.ShowGroups.erase(1);
							else
								GameData.Config.Item.ShowGroups.insert(1);
						}

						if (KeyName == "GroupBKey")
						{
							GameData.Config.Item.Enable = 1;
							//GameData.Config.Item.ShowGroup = 3;
							if (GameData.Config.Item.ShowGroups.count(2))
								GameData.Config.Item.ShowGroups.erase(2);
							else
								GameData.Config.Item.ShowGroups.insert(2);
						}

						if (KeyName == "GroupCKey")
						{
							GameData.Config.Item.Enable = 1;
							//GameData.Config.Item.ShowGroup = 4;
							if (GameData.Config.Item.ShowGroups.count(3))
								GameData.Config.Item.ShowGroups.erase(3);
							else
								GameData.Config.Item.ShowGroups.insert(3);
						}

						if (KeyName == "GroupDKey")
						{
							GameData.Config.Item.Enable = 1;
							//GameData.Config.Item.ShowGroup = 5;
							if (GameData.Config.Item.ShowGroups.count(4))
								GameData.Config.Item.ShowGroups.erase(4);
							else
								GameData.Config.Item.ShowGroups.insert(4);
						}

						if (KeyName == "VehicleEnable")
						{
							GameData.Config.Vehicle.Enable = !GameData.Config.Vehicle.Enable;
						}
						if (KeyName == "Menu")
						{
							//Utils::Log(1, "press END key!");
							GameData.Config.Menu.Show = !GameData.Config.Menu.Show;

							if (GameData.Config.Menu.Show)
							{
								SetForegroundWindow(GameData.Config.Overlay.hWnd);
							}
							else {
								SetForegroundWindow(GetDesktopWindow());
							}
						}
						if (KeyName == "AimBotConfig0")
						{
							GameData.Config.AimBot.ConfigIndex = 0;
						}
						if (KeyName == "AimBotConfig1")
						{
							GameData.Config.AimBot.ConfigIndex = 1;
						}
				}
			}

			Sleep(3);
		}
	}
}
