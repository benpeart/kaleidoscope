Import("env")
import sys
import time
import urllib.request
import urllib.error

quiet = False


def status(msg):
    """Print status message to stderr"""
    if not quiet:
        critical(msg)


def critical(msg):
    """Print critical message to stderr"""
    sys.stderr.write("safeboot.py: ")
    sys.stderr.write(str(msg))
    sys.stderr.write("\n")


# open "/safeboot" on target to restart in SafeBoot-mode
def safeboot(source, target, env):
    upload_protocol = env.GetProjectOption("upload_protocol")
    upload_port = env.GetProjectOption("upload_port")
    if upload_protocol != "espota":
        critical("Wrong upload protocol (%s)" % upload_protocol)
        raise Exception("Wrong upload protocol!")
    
    status("Trying to activate SafeBoot on: %s" % upload_port)
    safeboot_path = env.GetProjectOption("custom_safeboot_restart_path", "/safeboot")
    
    req = urllib.request.Request("http://" + upload_port + safeboot_path, method="POST")
    try:
        urllib.request.urlopen(req, timeout=5)
    except urllib.error.URLError as e:
        critical(f"Failed to activate SafeBoot via API: {e}")
        # Device might already be in SafeBoot mode; continue checking

    status("Reboot signal sent. Waiting for SafeBoot to initialize on the network...")
    
    # Poll until SafeBoot web server responds or 15 seconds expire
    safeboot_ready = False
    for attempt in range(1, 16):
        time.sleep(1)
        try:
            # SafeBoot serves its root web page on port 80 once operational
            check_req = urllib.request.Request("http://" + upload_port + "/", method="GET")
            with urllib.request.urlopen(check_req, timeout=1) as resp:
                if resp.status == 200:
                    safeboot_ready = True
                    break
        except Exception:
            pass

    if safeboot_ready:
        status("SafeBoot online and ready. Starting OTA upload...")
        # Brief settling delay for ArduinoOTA listener socket initialization
        time.sleep(1)
    else:
        status("SafeBoot did not respond on HTTP within 15s. Proceeding with upload attempt anyway...")


env.AddPreAction("upload", safeboot)
env.AddPreAction("uploadfs", safeboot)
env.AddPreAction("uploadfsota", safeboot)