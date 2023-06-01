#include <iostream>
#include <Windows.h>
#include <winuser.h>

const int REFRESH_RATE_CHARGER_CONNECTED = 144;
const int REFRESH_RATE_CHARGER_DISCONNECTED = 60;
const int DELAY_SECONDS = 5;

int main() {
    int currentRefreshRate = 0; // Variable to store the current refresh rate

    while (true) {
        SYSTEM_POWER_STATUS powerStatus;
        if (!GetSystemPowerStatus(&powerStatus)) {
            std::cout << "Failed to retrieve power status." << std::endl;
            return 1;
        }

        bool chargerConnected = (powerStatus.ACLineStatus == 1);
        DEVMODE devMode;
        ZeroMemory(&devMode, sizeof(devMode));
        devMode.dmSize = sizeof(devMode);

        if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode) == 0) {
            std::cout << "Failed to retrieve current display settings." << std::endl;
            return 1;
        }

        if (chargerConnected) {
            if (currentRefreshRate != REFRESH_RATE_CHARGER_CONNECTED) {
                devMode.dmDisplayFrequency = REFRESH_RATE_CHARGER_CONNECTED;
                currentRefreshRate = REFRESH_RATE_CHARGER_CONNECTED;
                LONG result = ChangeDisplaySettingsEx(NULL, &devMode, NULL, CDS_UPDATEREGISTRY, NULL);
                if (result != DISP_CHANGE_SUCCESSFUL) {
                    std::cout << "Failed to change the display settings. Error code: " << result << std::endl;
                    return 1;
                }
                std::cout << "Display settings changed successfully. Refresh rate set to " << devMode.dmDisplayFrequency << "Hz." << std::endl;
            }
        } else {
            if (currentRefreshRate != REFRESH_RATE_CHARGER_DISCONNECTED) {
                devMode.dmDisplayFrequency = REFRESH_RATE_CHARGER_DISCONNECTED;
                currentRefreshRate = REFRESH_RATE_CHARGER_DISCONNECTED;
                LONG result = ChangeDisplaySettingsEx(NULL, &devMode, NULL, CDS_UPDATEREGISTRY, NULL);
                if (result != DISP_CHANGE_SUCCESSFUL) {
                    std::cout << "Failed to change the display settings. Error code: " << result << std::endl;
                    return 1;
                }
                std::cout << "Display settings changed successfully. Refresh rate set to " << devMode.dmDisplayFrequency << "Hz." << std::endl;
            }
        }

        Sleep(DELAY_SECONDS * 1000);
    }

    return 0;
}
