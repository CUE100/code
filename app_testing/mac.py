import subprocess
import os
import time

import Quartz  # Add this import


def get_temp():
    try:
        temp_str = subprocess.check_output(["osx-cpu-temp"]).decode('utf-8')
        return float(''.join(c for c in temp_str if c.isdigit() or c == '.'))
    except:
        return 0


def is_any_app_fullscreen():
    # ... inside this function, make sure you use Quartz ...
    options = Quartz.kCGWindowListOptionOnScreenOnly
    window_list = Quartz.CGWindowListCopyWindowInfo(
        options, Quartz.kCGNullWindowID)
    # ... rest of the logic ...

    # Change AppKit to Quartz here:
    options = Quartz.kCGWindowListOptionOnScreenOnly
    window_list = Quartz.CGWindowListCopyWindowInfo(
        options, Quartz.kCGNullWindowID)

    for window in window_list:
        bounds = window.get('kCGWindowBounds')
        owner = window.get('kCGWindowOwnerName')

        if bounds and bounds['width'] == screen_w and bounds['height'] == screen_h:
            # We filter out typical system layers that are "full screen" but not apps
            if owner not in ["Wallspace", "Window Server", "Finder", "Dock"]:
                return True
    return False


def kill_wallspace():
    # Adding a check so it doesn't try to kill it if it's already closed
    os.system("pkill -x 'Wallspace'")


# Main Loop
while True:
    temp = get_temp()
    fullscreen = is_any_app_fullscreen()

    if temp > 80 or fullscreen:
        print(f"Triggered! Temp: {temp} | Fullscreen: {fullscreen}")
        kill_wallspace()

    time.sleep(5)
